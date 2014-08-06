/*
    This file is part of libkabc.

    Copyright (c) 2003 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "testlock.h"
#include "kabc/stdaddressbook.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kcmdlineargs.h>
#include <kdirwatch.h>
#include <kmessagebox.h>
#include <kdialog.h>

#include <qtreewidget.h>
#include <QLabel>
#include <QPushButton>
#include <QtCore/QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <sys/types.h>
#include <iostream>
#include <unistd.h>

using namespace KABC;

LockWidget::LockWidget(const QString &identifier)
{
    QVBoxLayout *topLayout = new QVBoxLayout(this);

    if (identifier.isEmpty()) {
        mLock = 0;
    } else {
        mLock = new Lock(identifier);

        int pid = getpid();

        QLabel *pidLabel = new QLabel(QLatin1String("Process ID: ") + QString::number(pid),
                                      this);
        topLayout->addWidget(pidLabel);

        QHBoxLayout *identifierLayout = new QHBoxLayout();
        identifierLayout->setParent(topLayout);
        topLayout->addLayout(identifierLayout);

        QLabel *resourceLabel = new QLabel(QLatin1String("Identifier:"), this);
        identifierLayout->addWidget(resourceLabel);

        QLabel *resourceIdentifier = new QLabel(identifier, this);
        identifierLayout->addWidget(resourceIdentifier);

        mStatus = new QLabel(QLatin1String("Status: Unlocked"), this);
        topLayout->addWidget(mStatus);

        QPushButton *button = new QPushButton(QLatin1String("Lock"), this);
        topLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, &LockWidget::lock);

        button = new QPushButton(QLatin1String("Unlock"), this);
        topLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, &LockWidget::unlock);
    }

    mLockView = new QTreeWidget(this);
    topLayout->addWidget(mLockView);
    QStringList headers;
    headers.append(QLatin1String("Lock File"));
    headers.append(QLatin1String("PID"));
    headers.append(QLatin1String("Locking App"));
    mLockView->setHeaderLabels(headers);

    updateLockView();

    QPushButton *quitButton = new QPushButton(QLatin1String("Quit"), this);
    topLayout->addWidget(quitButton);
    connect(quitButton, &QPushButton::clicked, this, &LockWidget::close);

    KDirWatch *watch = KDirWatch::self();
    connect(watch, SIGNAL(dirty(QString)),
            SLOT(updateLockView()));
    connect(watch, SIGNAL(created(QString)),
            SLOT(updateLockView()));
    connect(watch, SIGNAL(deleted(QString)),
            SLOT(updateLockView()));
    watch->addDir(Lock::locksDir());
    watch->startScan();
}

LockWidget::~LockWidget()
{
    delete mLock;
}

void LockWidget::updateLockView()
{
    mLockView->clear();

    QDir dir(Lock::locksDir());

    QStringList files = dir.entryList(QStringList(QLatin1String("*.lock")));

    QStringList::ConstIterator it;
    for (it = files.constBegin(); it != files.constEnd(); ++it) {
        if (*it == QLatin1String(".") || *it == QLatin1String("..")) {
            continue;
        }

        QString app;
        int pid;
        if (!Lock::readLockFile(dir.filePath(*it), pid, app)) {
            qWarning() << "Unable to open lock file '" << *it << "'";
        } else {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, *it);
            item->setText(1, QString::number(pid));
            item->setText(2, app);
            mLockView->addTopLevelItem(item);
        }
    }
}

void LockWidget::lock()
{
    if (!mLock->lock()) {
        KMessageBox::sorry(this, mLock->error());
    } else {
        mStatus->setText(QLatin1String("Status: Locked"));
    }
}

void LockWidget::unlock()
{
    if (!mLock->unlock()) {
        KMessageBox::sorry(this, mLock->error());
    } else {
        mStatus->setText(QLatin1String("Status: Unlocked"));
    }
}

int main(int argc, char **argv)
{
    KAboutData aboutData("testlock", 0, ki18n("Test libkabc Lock"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("a");
    options.add("addressbook", ki18n("Standard address book"));
    options.add("d");
    options.add("diraddressbook", ki18n("Standard address book directory resource"));
    options.add("+identifier", ki18n("Identifier of resource to be locked, e.g. filename"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    QString identifier;

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->count() == 1) {
        identifier = args->arg(0);
    } else if (args->count() != 0) {
        std::cerr << "Usage: testlock <identifier>" << std::endl;
        return 1;
    }

    if (args->isSet("addressbook")) {
        if (args->count() == 1) {
            std::cerr << "Ignoring resource identifier" << std::endl;
        }
        identifier = StdAddressBook::fileName();
    }

    if (args->isSet("diraddressbook")) {
        if (args->count() == 1) {
            std::cerr << "Ignoring resource identifier" << std::endl;
        }
        identifier = StdAddressBook::directoryName();
    }

    LockWidget mainWidget(identifier);

    mainWidget.show();

    return app.exec();
}


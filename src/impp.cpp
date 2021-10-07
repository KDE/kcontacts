/*
    This file is part of libkabc.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "impp.h"
#include "kcontacts_debug.h"

#include <KDesktopFile>

#include <QDataStream>
#include <QDirIterator>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>

using namespace KContacts;

class Q_DECL_HIDDEN Impp::Private : public QSharedData
{
public:
    Private() = default;
    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
    }

    QHash<QString, QStringList> parameters;
    QUrl address;
};

Impp::Impp()
    : d(new Private)
{
}

Impp::Impp(const Impp &other)
    : d(other.d)
{
}

Impp::Impp(const QUrl &address)
    : d(new Private)
{
    d->address = address;
}

Impp::~Impp()
{
}

bool Impp::isValid() const
{
    return !d->address.isEmpty() && !d->address.scheme().isEmpty();
}

void Impp::setAddress(const QUrl &address)
{
    d->address = address;
}

QUrl Impp::address() const
{
    return d->address;
}

QString Impp::serviceType() const
{
    return d->address.scheme();
}

QString Impp::serviceLabel() const
{
    return serviceLabel(serviceType());
}

QString Impp::serviceIcon() const
{
    return serviceIcon(serviceType());
}

bool Impp::isPreferred() const
{
    const auto it = d->parameters.constFind(QLatin1String("pref"));
    if (it != d->parameters.constEnd() && !it.value().isEmpty()) {
        return it.value().at(0) == QLatin1Char('1');
    }
    return false;
}

void Impp::setPreferred(bool preferred)
{
    if (preferred) {
        d->parameters.insert(QStringLiteral("pref"), {QStringLiteral("1")});
    } else {
        d->parameters.remove(QLatin1String("pref"));
    }
}

void Impp::setParameters(const QHash<QString, QStringList> &params)
{
    d->parameters = params;
}

QHash<QString, QStringList> Impp::parameters() const
{
    return d->parameters;
}

bool Impp::operator==(const Impp &other) const
{
    return (d->parameters == other.parameters()) && (d->address == other.address());
}

bool Impp::operator!=(const Impp &other) const
{
    return !(other == *this);
}

Impp &Impp::operator=(const Impp &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Impp::toString() const
{
    QString str = QLatin1String("Impp {\n");
    str += QStringLiteral("    type: %1\n").arg(serviceType());
    str += QStringLiteral("    address: %1\n").arg(d->address.url());
    if (!d->parameters.isEmpty()) {
        QString param;
        for (auto it = d->parameters.cbegin(), itEnd = d->parameters.cend(); it != itEnd; ++it) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

QDataStream &KContacts::operator<<(QDataStream &s, const Impp &impp)
{
    return s << impp.d->parameters << impp.d->address << (uint32_t)(0);
}

QDataStream &KContacts::operator>>(QDataStream &s, Impp &impp)
{
    int i;
    s >> impp.d->parameters >> impp.d->address >> i;
    return s;
}

static QString improtcolFile(const QString &serviceType)
{
    const auto path =
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/kcontacts/improtocols/") + serviceType + QStringLiteral(".desktop"));
    if (!path.isEmpty()) {
        return path;
    }
    return QStringLiteral(":/org.kde.kcontacts/improtocols/") + serviceType + QStringLiteral(".desktop");
}

QString Impp::serviceLabel(const QString &serviceType)
{
    const auto path = improtcolFile(serviceType);
    KDesktopFile df(path);
    return df.readName();
}

QString Impp::serviceIcon(const QString &serviceType)
{
    const auto path = improtcolFile(serviceType);
    KDesktopFile df(path);
    return df.readIcon();
}

QVector<QString> Impp::serviceTypes()
{
    QVector<QString> types;
    auto paths = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("kf5/kcontacts/improtocols"), QStandardPaths::LocateDirectory);
    paths.push_back(QStringLiteral(":/org.kde.kcontacts/improtocols/"));
    for (const auto &path : paths) {
        QDirIterator it(path, QDir::Files);
        while (it.hasNext()) {
            it.next();
            const auto fi = it.fileInfo();
            if (fi.suffix() == QLatin1String("desktop")) {
                types.push_back(fi.baseName());
            }
        }
    }

    std::sort(types.begin(), types.end());
    types.erase(std::unique(types.begin(), types.end()), types.end());
    return types;
}

#include "moc_impp.cpp"

/*
    This file is part of libkabc.

    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>
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

#include "errorhandler.h"
#include "kabc_debug.h"
#include <qdebug.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>

#include <QApplication>

using namespace KABC;

ErrorHandler::~ErrorHandler()
{
}

ConsoleErrorHandler::ConsoleErrorHandler()
    : d(0)
{
}

ConsoleErrorHandler::~ConsoleErrorHandler()
{
}

void ConsoleErrorHandler::error(const QString &msg)
{
    // no debug area is ok here
    qCCritical(KABC_LOG) << msg;
}

class GuiErrorHandler::Private
{
public:
    Private(QWidget *widget)
        : mWidget(widget)
    {
    }

    QWidget *mWidget;
};

GuiErrorHandler::GuiErrorHandler(QWidget *widget)
    : d(new Private(widget))
{
}

GuiErrorHandler::~GuiErrorHandler()
{
    delete d;
}

void GuiErrorHandler::error(const QString &msg)
{
    if (qApp) {
        KMessageBox::error(d->mWidget, msg);
    }
}

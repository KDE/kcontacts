/*
    This file is part of the KContacts framework.

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
#ifndef KCONTACTS_ERRORHANDLER_H
#define KCONTACTS_ERRORHANDLER_H

#include "kcontacts_export.h"
#include <QtCore/QString>


namespace KContacts
{

/**
  Abstract class that provides displaying of error messages.
  We need this to make the KContacts framework. gui independent on the one side
  and provide user friendly error messages on the other side.
  Use @p ConsoleErrorHandler or @p GuiErrorHandler in your
  application or provide your own ErrorHandler.
*/
class KCONTACTS_EXPORT ErrorHandler
{
public:
    /**
      Destroys the handler instance.
    */
    virtual ~ErrorHandler();

    /**
      Show error message.

      @param msg The error message to show
    */
    virtual void error(const QString &msg) = 0;
};

/**
  This class prints the error messages to stderr via kError().
*/
class KCONTACTS_EXPORT ConsoleErrorHandler : public ErrorHandler
{
public:
    /**
      Create an error handler for console output.

      Uses kError() to write the error messages.
    */
    ConsoleErrorHandler();

    /**
      Destroys the handler instance.
    */
    virtual ~ConsoleErrorHandler();

    virtual void error(const QString &msg);

private:
    class Private;
    Private *const d;

    Q_DISABLE_COPY(ConsoleErrorHandler)
};

}

#endif

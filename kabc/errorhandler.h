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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef KABC_ERRORHANDLER_H
#define KABC_ERRORHANDLER_H

#include <qstring.h>

#include <kdelibs_export.h>

namespace KABC {

/**
  Abstract class that provides displaying of error messages.
  We need this to make libkabc gui independent on the one side
  and provide user friendly error messages on the other side.
  Use @p ConsoleErrorHandler or @p GuiErrorHandler in your
  application or provide your own ErrorHandler.
*/
class KABC_EXPORT ErrorHandler
{
  public:
    /**
      Show error message.
    */
    virtual void error( const QString &msg ) = 0;
};

/**
  This class prints the error messages to stderr via kdError().
*/
class KABC_EXPORT ConsoleErrorHandler : public ErrorHandler
{
  public:
    virtual void error( const QString &msg );
};

/**
  This class shows messages boxes for every
  error message.

  \deprecated Use GuiErrorHandler instead.
*/
class KABC_EXPORT GUIErrorHandler : public ErrorHandler
{
  public:
    virtual void error( const QString &msg );
};

/**
  This class shows messages boxes for every
  error message.
*/
class KABC_EXPORT GuiErrorHandler : public ErrorHandler
{
  public:
    /**
      Create error handler.
      
      \param parent Widget which is used as parent for the error dialogs.
    */      
    GuiErrorHandler( QWidget *parent );
  
    virtual void error( const QString &msg );

  private:
    QWidget *mParent;
    
    class Private;
    Private *d;
};

}

#endif

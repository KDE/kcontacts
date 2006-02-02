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

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "errorhandler.h"

using namespace KABC;

void ConsoleErrorHandler::error( const QString &msg )
{
  // no debug area is ok here
  kError() << msg << endl;
}


GuiErrorHandler::GuiErrorHandler( QWidget *parent )
  : mParent( parent )
{
}

void GuiErrorHandler::error( const QString &msg )
{
  KMessageBox::error( mParent, msg );
}

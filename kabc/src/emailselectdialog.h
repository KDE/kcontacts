/*
  This file is part of the kabc library.
  Copyright (c) 2001 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef KABC_EMAILSELECTDIALOG_H
#define KABC_EMAILSELECTDIALOG_H

#include <kabc/kabc_export.h>
#include <kdialog.h>

namespace KABC {

class KABC_EXPORT EmailSelectDialog : public KDialog
{
  Q_OBJECT

  public:
    EmailSelectDialog( const QStringList &emails, const QString &current, QWidget *parent );
    ~EmailSelectDialog();

    QString selected();

    static QString getEmail( const QStringList &emails, const QString &current,
                             QWidget *parent );

  private:
    class Private;
    Private *const d;
};

}

#endif

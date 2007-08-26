/*
    This file is part of libkabc.
    Copyright (c) 2002 Helge Deller <deller@gmx.de>
                  2002 Lubos Lunak <llunak@suse.cz>

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

#ifndef KABC_ADDRESSLINEEDIT_H
#define KABC_ADDRESSLINEEDIT_H

#include "kabc_export.h"
#include <kcompletion.h>
#include <klineedit.h>
#include <QtCore/QObject>

class KConfig;

namespace KABC {

/**
 * A lineedit with kabc completion
 *
 * This lineedit is supposed to be used wherever the user types email addresses
 * and might want a completion. You can simply use it as a replacement for
 * KLineEdit or QLineEdit.
 *
 * You can enable or disable the lineedit at any time.
 *
 * @see AddressLineEdit::enableCompletion()
 */
class KABC_EXPORT AddressLineEdit : public KLineEdit
{
  Q_OBJECT

  public:
    explicit AddressLineEdit( QWidget *parent, bool useCompletion = true );
    virtual ~AddressLineEdit();

    /**
     * Reimplented for internal reasons.
     * @param font The font to use
     * @see KLineEdit::setFont()
     */
    virtual void setFont( const QFont &font );

    static KConfig *config();

  public Q_SLOTS:
    /**
     * Set cursor to end of line.
     */
    void cursorAtEnd();

    /**
     * Toggle completion.
     */
    void enableCompletion( bool enable );

  protected:
    /**
     * Always call AddressLineEdit::loadAddresses() as the first thing.
     * Use addAddress() to add addresses.
     */
    virtual void loadAddresses();
    void addAddress( const QString &addr );
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void dropEvent( QDropEvent *e );
    virtual void paste();
    virtual void insert( const QString &addr );
    virtual void mouseReleaseEvent( QMouseEvent *e );
    void doCompletion( bool );

  private:
    class Private;
    Private *const d;

    Q_PRIVATE_SLOT( d, void slotCompletion() )
    Q_PRIVATE_SLOT( d, void slotPopupCompletion( const QString &completion ) )
};

}

#endif

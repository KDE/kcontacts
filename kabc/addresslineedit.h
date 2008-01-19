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
    /**
     * Creates the line edit instance.
     *
     * @param parent The QWidget parent
     * @param useCompletion Whether to use address completion.
     *                      See enableCompletion()
     */
    explicit AddressLineEdit( QWidget *parent, bool useCompletion = true );

    /**
     * Destroys the instance.
     */
    virtual ~AddressLineEdit();

    /**
     * Reimplemented for internal reasons.
     * @param font The font to use
     * @see KLineEdit::setFont()
     */
    virtual void setFont( const QFont &font );

  public Q_SLOTS:
    /**
     * Set cursor to end of line.
     */
    void cursorAtEnd();

    /**
     * Toggle completion.
     *
     * @param enable When @c true address completion will be enabled, when
     *               @c false it will be disabled
     */
    void enableCompletion( bool enable );

  protected:
    /**
     * Always call AddressLineEdit::loadAddresses() as the first thing.
     * Use addAddress() to add addresses.
     */
    virtual void loadAddresses();

    /**
     * Adds a new address to the line edit.
     *
     * Adds the given string to the completion handler and additionally the
     * email part if the string contains name + address in the angle bracket
     * notation.
     *
     * @param addr The address to add
     */
    void addAddress( const QString &addr );

    /**
     * Handles KDE completion short cuts
     *
     * @param e The key event to check
     *
     * @see KStandardShortcut::SubstringCompletion
     * @see KStandardShortcut::TextCompletion
     */
    virtual void keyPressEvent( QKeyEvent *e );

    /**
     * Handles drop events.
     *
     * Creates a list of addresses separated by @c ',' from a "URI List" drop.
     * Enables smart paste for anything else before relaying the event to
     * the base class.
     * See insert() for information on smart paste.
     *
     * @param e The drop event
     *
     * @see enableCompletion()
     */
    virtual void dropEvent( QDropEvent *e );

    /**
     * Pastes the clipboard content.
     *
     * Enables smart paste if completion is enabled.
     * See insert() for information on smart paste.
     *
     * @see enableCompletion()
     */
    virtual void paste();

    /**
     * Inserts the given string.
     *
     * If smart paste is enabled, the text will be parsed for possible
     * email address parts, i.e.
     * either a mailto: URI or "spam protected" like "developer at kde dot org"
     *
     * @param addr The string to insert
     */
    virtual void insert( const QString &addr );

    /**
     * Enables smart paste for X11 middle mouse text paste if completion is
     * enabled.
     *
     * See insert() for information about smart paste.
     *
     * @param e The mouse release event
     *
     * @see enableCompletion()
     */
    virtual void mouseReleaseEvent( QMouseEvent *e );

    /**
     * Triggers looking for a completion of the address or the last
     * address if there are already more than one separated by @c ','
     */
    void doCompletion( bool );

  private:
    class Private;
    Private *const d;

    Q_PRIVATE_SLOT( d, void slotCompletion() )
    Q_PRIVATE_SLOT( d, void slotPopupCompletion( const QString &completion ) )
};

}

#endif

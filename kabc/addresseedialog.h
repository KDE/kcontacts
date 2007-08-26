/*
    This file is part of libkabc.
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

#ifndef KABC_ADDRESSEEDIALOG_H
#define KABC_ADDRESSEEDIALOG_H

#include "addressbook.h"

#include <kdialog.h>
#include <klineedit.h>

#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtGui/QTreeWidgetItem>

namespace KABC {

/**
  @short Special ListViewItem, that is used by the AddresseeDialog.
*/
class KABC_EXPORT AddresseeItem : public QTreeWidgetItem
{
  public:

    /**
      Type of column
    */
    enum Columns {
      Name = 0, /**< Addressee name. */
      Email = 1 /**< Addressee email */
    };

    /**
      Constructor.

      @param parent    The parent listview.
      @param addressee The associated addressee.
    */
    AddresseeItem( QTreeWidget *parent, const Addressee &addressee );

    /**
      Destroys the item.
    */
    ~AddresseeItem();

    /**
      Returns the addressee.
    */
    Addressee addressee() const;

    /**
      Method used by QListView to sort the items.
    */
    virtual QString key( int column, bool ascending ) const;

  private:
    class Private;
    Private *const d;

    Q_DISABLE_COPY( AddresseeItem )
};

/**
  @short Dialog for selecting address book entries.

  This class provides a dialog for selecting entries from the standard KDE
  address book. Use the getAddressee() function to open a modal dialog,
  returning an address book entry.

  In the dialog you can select an entry from the list with the mouse or type in
  the first letters of the name or email address you are searching for. The
  entry matching best is automatically selected. Use double click, pressing
  return or pressing the ok button to return the selected addressee to the
  application.
*/
class KABC_EXPORT AddresseeDialog : public KDialog
{
    Q_OBJECT

  public:
    /**
      Construct addressbook entry select dialog.

      @param parent parent widget
      @param multiple if true, indicates a multiple selection.
    */
    explicit AddresseeDialog( QWidget *parent = 0, bool multiple = false );

    /**
      Destructor.
    */
    virtual ~AddresseeDialog();

    /**
      Return the address chosen.

      If it is a multiple select, this will return only the first address chosen
    */
    Addressee addressee() const;

    /**
      Return the list of addresses chosen
    */
    Addressee::List addressees() const;

    /**
      Select a single address book entry.

      Open addressee select dialog and return the entry selected by the user.
      If the user doesn't select an entry or presses cancel, the returned
      addressee is empty.

      @param parent The QWidget parent for the dialog.
    */
    static Addressee getAddressee( QWidget *parent );

    /**
      Select multiple address book entries.

      Open addressee select dialog and return the entries selected by the user.
      If the user doesn't select an entry or presses cancel, the returned
      addressee list is empty.

      @param parent The QWidget parent for the dialog.
    */
    static Addressee::List getAddressees( QWidget *parent );

  private:
    class Private;
    Private *const d;

    Q_PRIVATE_SLOT( d, void addressBookChanged() )
    Q_PRIVATE_SLOT( d, void selectItem( const QString & ) )
    Q_PRIVATE_SLOT( d, void updateEdit() )
    Q_PRIVATE_SLOT( d, void addSelected( QTreeWidgetItem * ) )
    Q_PRIVATE_SLOT( d, void removeSelected() )
};

}
#endif

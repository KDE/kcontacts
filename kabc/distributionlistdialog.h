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

#ifndef KABC_DISTRIBUTIONLISTDIALOG_H
#define KABC_DISTRIBUTIONLISTDIALOG_H

#include <QtGui/QWidget>

#include <kdialog.h>
#include "kabc_export.h"


namespace KABC {

class AddressBook;
class DistributionListEditorWidget;

/**
  @short Frontend to create distribution lists

  Creating a new DistributionListDialog does automatically
  load all addressees and distribution lists from the config
  files. The changes will be saved when clicking the 'OK'
  button.

  Example:

  \code
  KABC::DistributionListDialog *dlg = new
          KABC::DistributionListDialog( KABC::StdAddressBook::self(), this );

  dlg->exec();
  \endcode
*/
class KABC_EXPORT DistributionListDialog : public KDialog
{
    Q_OBJECT

  public:
    /**
      Constructor.

      @param ab     The addressbook, the addressees should be used from
      @param parent The parent widget
    */
    explicit DistributionListDialog( AddressBook *ab, QWidget *parent = 0 );

    /**
      Destructor.
    */
    virtual ~DistributionListDialog();

  private:
    class Private;
    Private* const d;
};

/**
  @short Helper class
*/
class KABC_EXPORT EmailSelector : public KDialog
{
  public:
    EmailSelector( const QStringList &emails, const QString &current, QWidget *parent = 0 );
    ~EmailSelector();

    QString selected() const;

    static QString getEmail( const QStringList &emails, const QString &current, QWidget *parent = 0 );

  private:
    class Private;
    Private* const d;
};

/**
  @short Helper class
*/
class KABC_EXPORT DistributionListEditorWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit DistributionListEditorWidget( AddressBook *addressBook, QWidget *parent = 0 );
    virtual ~DistributionListEditorWidget();

  private:
    class Private;
    Private* const d;

    Q_PRIVATE_SLOT( d, void newList() )
    Q_PRIVATE_SLOT( d, void editList() )
    Q_PRIVATE_SLOT( d, void removeList() )
    Q_PRIVATE_SLOT( d, void addEntry() )
    Q_PRIVATE_SLOT( d, void removeEntry() )
    Q_PRIVATE_SLOT( d, void changeEmail() )
    Q_PRIVATE_SLOT( d, void updateEntryView() )
    Q_PRIVATE_SLOT( d, void updateAddresseeView() )
    Q_PRIVATE_SLOT( d, void updateNameCombo() )
    Q_PRIVATE_SLOT( d, void slotSelectionEntryViewChanged() )
    Q_PRIVATE_SLOT( d, void slotSelectionAddresseeViewChanged() )
    Q_PRIVATE_SLOT( d, void save() )
};

}
#endif

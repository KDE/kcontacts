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

#include "addresseedialog.h"
#ifndef KDEPIM_NO_KRESOURCES
#include "stdaddressbook.h"
#endif

#include <qdebug.h>
#include <klocalizedstring.h>

#include <QtCore/QPointer>
#include <QtCore/QRegExp>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>

using namespace KABC;

class AddresseeItem::Private
{
  public:
    Addressee mAddressee;
};

AddresseeItem::AddresseeItem( QTreeWidget *parent, const Addressee &addressee ) :
  QTreeWidgetItem( parent ), d( new Private )
{
  d->mAddressee = addressee;

  setText( Name, addressee.realName() );
  setText( Email, addressee.preferredEmail() );
}

AddresseeItem::~AddresseeItem()
{
  delete d;
}

Addressee AddresseeItem::addressee() const
{
  return d->mAddressee;
}

QString AddresseeItem::key( int column, bool ) const
{
  if ( column == Email ) {
    QString value = text( Email );
    const QRegExp emailRe( QLatin1String( "<\\S*>" ) );
    int match = emailRe.indexIn( value );
    if ( match > -1 ) {
      value = value.mid( match + 1, emailRe.matchedLength() - 2 );
    }

    return value.toLower();
  }

  return text( column ).toLower();
}

class AddresseeDialog::Private
{
  public:
    Private( bool multiple )
      : mMultiple( multiple )
    {
    }

    void addressBookChanged();
    void selectItem( const QString & );
    void updateEdit();
    void addSelected( QTreeWidgetItem *item );
    void removeSelected();

    void loadAddressBook();
    void addCompletionItem( const QString &str, QTreeWidgetItem *item );

    bool mMultiple;

    QTreeWidget *mAddresseeList;
    KLineEdit *mAddresseeEdit;

    QTreeWidget *mSelectedList;

#ifndef KDEPIM_NO_KRESOURCES
    AddressBook *mAddressBook;
#endif

    QHash<QString, QTreeWidgetItem*> mItemDict;
    QHash<QString, QTreeWidgetItem*> mSelectedDict;
};

AddresseeDialog::AddresseeDialog( QWidget *parent, bool multiple )
  : KDialog( parent ), d( new Private( multiple ) )
{
  setCaption( i18nc( "@title:window", "Select Addressee" ) );
  setButtons( Ok | Cancel );
  setDefaultButton( Ok );

  QWidget *topWidget = new QWidget( this );
  setMainWidget( topWidget );

  QBoxLayout *topLayout = new QHBoxLayout( topWidget );
  QBoxLayout *listLayout = new QVBoxLayout;
  topLayout->addLayout( listLayout );

  d->mAddresseeList = new QTreeWidget( topWidget );
  d->mAddresseeList->setColumnCount( 2 );
  QStringList headerTitles;
  headerTitles << i18nc( "@title:column addressee name", "Name" )
               << i18nc( "@title:column addressee email", "Email" );
  d->mAddresseeList->setHeaderItem( new QTreeWidgetItem( headerTitles ) );
  listLayout->addWidget( d->mAddresseeList );
  connect( d->mAddresseeList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
           SLOT(accept()) );
  connect( d->mAddresseeList, SIGNAL(itemSelectionChanged()),
           SLOT(updateEdit()) );

  d->mAddresseeEdit = new KLineEdit( topWidget );
  d->mAddresseeEdit->setCompletionMode( KGlobalSettings::CompletionAuto );
  connect( d->mAddresseeEdit->completionObject(), SIGNAL(match(QString)),
           SLOT(selectItem(QString)) );
  d->mAddresseeEdit->setFocus();
  d->mAddresseeEdit->completionObject()->setIgnoreCase( true );
  listLayout->addWidget( d->mAddresseeEdit );

  setInitialSize( QSize( 450, 300 ) );

  if ( d->mMultiple ) {
    QBoxLayout *selectedLayout = new QVBoxLayout;
    topLayout->addLayout( selectedLayout );

    QGroupBox *selectedGroup =
      new QGroupBox( i18nc( "@title:group selected addressees", "Selected" ), topWidget );
    QHBoxLayout *groupLayout = new QHBoxLayout;
    selectedGroup->setLayout( groupLayout );
    selectedLayout->addWidget( selectedGroup );

    d->mSelectedList = new QTreeWidget( selectedGroup );
    groupLayout->addWidget( d->mSelectedList );
    d->mSelectedList->setColumnCount( 2 );
    QStringList headerTitles;
    headerTitles << i18nc( "@title:column addressee name", "Name" )
                 << i18nc( "@title:column addressee email", "Email" );
    d->mSelectedList->setHeaderItem( new QTreeWidgetItem( headerTitles ) );

    connect( d->mSelectedList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
             SLOT(removeSelected()) );

    QPushButton *unselectButton =
      new QPushButton( i18nc( "@action:button unselect addressee", "Unselect" ), selectedGroup );
    selectedLayout->addWidget( unselectButton );
    connect( unselectButton, SIGNAL(clicked()), SLOT(removeSelected()) );

    connect( d->mAddresseeList, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
             SLOT(addSelected(QTreeWidgetItem*)) );

    setInitialSize( QSize( 650, 350 ) );
  }

#ifndef KDEPIM_NO_KRESOURCES
  d->mAddressBook = StdAddressBook::self( true );
  connect( d->mAddressBook, SIGNAL(addressBookChanged(AddressBook*)),
           SLOT(addressBookChanged()) );
  connect( d->mAddressBook, SIGNAL(loadingFinished(Resource*)),
           SLOT(addressBookChanged()) );
#endif

  d->loadAddressBook();
}

AddresseeDialog::~AddresseeDialog()
{
  delete d;
}

Addressee AddresseeDialog::addressee() const
{
  AddresseeItem *aItem = 0;

  if ( d->mMultiple ) {
    aItem = dynamic_cast<AddresseeItem *>( d->mSelectedList->topLevelItem( 0 ) );
  } else {
    QList<QTreeWidgetItem*> selected = d->mAddresseeList->selectedItems();
    if ( !selected.isEmpty() ) {
      aItem = dynamic_cast<AddresseeItem *>( selected.at( 0 ) );
    }
  }

  if ( aItem ) {
    return aItem->addressee();
  }
  return Addressee();
}

Addressee::List AddresseeDialog::addressees() const
{
  Addressee::List al;
  AddresseeItem *aItem = 0;

  if ( d->mMultiple ) {
    const int numberOfTopItem( d->mSelectedList->topLevelItemCount() );
    for ( int i = 0; i < numberOfTopItem; ++i ) {
      aItem = dynamic_cast<AddresseeItem *>( d->mSelectedList->topLevelItem( i ) );
      if ( aItem ) {
        al.append( aItem->addressee() );
      }
    }
  } else {
    QList<QTreeWidgetItem*> selected = d->mAddresseeList->selectedItems();
    if ( !selected.isEmpty() ) {
      aItem = dynamic_cast<AddresseeItem *>( selected.at( 0 ) );
    }
    if ( aItem ) {
      al.append( aItem->addressee() );
    }
  }

  return al;
}

Addressee AddresseeDialog::getAddressee( QWidget *parent )
{
  Addressee contact;

  QPointer<AddresseeDialog> dlg = new AddresseeDialog( parent );
  if ( dlg->exec() && dlg ) {
    contact = dlg->addressee();
  }

  delete dlg;

  return contact;
}

Addressee::List AddresseeDialog::getAddressees( QWidget *parent )
{
  Addressee::List contacts;

  QPointer<AddresseeDialog> dlg = new AddresseeDialog( parent, true );
  if ( dlg->exec() && dlg ) {
    contacts = dlg->addressees();
  }

  delete dlg;

  return contacts;
}

void AddresseeDialog::Private::loadAddressBook()
{
  mAddresseeList->clear();
  mItemDict.clear();
  mAddresseeEdit->completionObject()->clear();

#ifndef KDEPIM_NO_KRESOURCES
  AddressBook::Iterator it;
  for ( it = mAddressBook->begin(); it != mAddressBook->end(); ++it ) {
    AddresseeItem *item = new AddresseeItem( mAddresseeList, ( *it ) );
    addCompletionItem( ( *it ).realName(), item );
    addCompletionItem( ( *it ).preferredEmail(), item );
  }
#endif
}

void AddresseeDialog::Private::addCompletionItem( const QString &str, QTreeWidgetItem *item )
{
  if ( str.isEmpty() ) {
    return;
  }

  mItemDict.insert( str, item );
  mAddresseeEdit->completionObject()->addItem( str );
}

void AddresseeDialog::Private::selectItem( const QString &str )
{
  if ( str.isEmpty() ) {
    return;
  }

  QTreeWidgetItem *item = mItemDict.value( str, 0 );
  if ( item ) {
    mAddresseeList->blockSignals( true );
    mAddresseeList->setItemSelected( item, true );
    mAddresseeList->scrollToItem( item );
    mAddresseeList->blockSignals( false );
  }
}

void AddresseeDialog::Private::updateEdit()
{
  QList<QTreeWidgetItem*> selected = mAddresseeList->selectedItems();
  if ( selected.isEmpty() ) {
    return;
  }
  QTreeWidgetItem *item = selected.at( 0 );
  mAddresseeEdit->setText( item->text( 0 ) );
  mAddresseeEdit->setSelection( 0, item->text( 0 ).length() );
}

void AddresseeDialog::Private::addSelected( QTreeWidgetItem *item )
{
  AddresseeItem *addrItem = dynamic_cast<AddresseeItem *>( item );
  if ( !addrItem ) {
    return;
  }

  Addressee a = addrItem->addressee();

  QTreeWidgetItem *selectedItem = mSelectedDict.value( a.uid(), 0 );
  if ( !selectedItem ) {
    selectedItem = new AddresseeItem( mSelectedList, a );
    mSelectedDict.insert( a.uid(), selectedItem );
  }
}

void AddresseeDialog::Private::removeSelected()
{
  QList<QTreeWidgetItem*> selected = mSelectedList->selectedItems();
  if ( selected.isEmpty() ) {
    return;
  }
  AddresseeItem *addrItem = dynamic_cast<AddresseeItem *>( selected.at( 0 ) );
  if ( !addrItem ) {
    return;
  }

  mSelectedDict.remove( addrItem->addressee().uid() );
  delete addrItem;
}

void AddresseeDialog::Private::addressBookChanged()
{
  loadAddressBook();
}

#include "moc_addresseedialog.cpp"

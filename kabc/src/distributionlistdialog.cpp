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

#include "distributionlistdialog.h"
#include "distributionlist.h"
#include "addressbook.h"
#include "addresseedialog.h"

#include <kinputdialog.h>
#include <klocalizedstring.h>
#include <qdebug.h>
#include <kmessagebox.h>
#include <kcombobox.h>

#include <QtCore/QPointer>
#include <QTreeWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>

using namespace KABC;

DistributionListDialog::DistributionListDialog( AddressBook *addressBook, QWidget *parent )
  : KDialog( parent ), d( 0 )
{
  setModal( true );
  setCaption( i18n( "Configure Distribution Lists" ) );
  setButtons( Ok );
  setDefaultButton( Ok );
  showButtonSeparator( true );

  DistributionListEditorWidget *editor = new DistributionListEditorWidget( addressBook, this );
  setMainWidget( editor );

  connect( this, SIGNAL(okClicked()), editor, SLOT(save()) );
}

DistributionListDialog::~DistributionListDialog()
{
}

class EmailSelector::Private
{
  public:
    QButtonGroup *mButtonGroup;
    QMap<QWidget *, QString> mEmailMap;
};

EmailSelector::EmailSelector( const QStringList &emails, const QString &current, QWidget *parent )
  : KDialog( parent ), d( new Private )
{
  setCaption( i18n( "Select Email Address" ) );
  setButtons( Ok );
  setDefaultButton( Ok );

  QFrame *topFrame = new QFrame( this );
  setMainWidget( topFrame );

  QBoxLayout *topLayout = new QVBoxLayout( topFrame );

  QGroupBox *box = new QGroupBox( i18n( "Email Addresses" ) );
  d->mButtonGroup = new QButtonGroup( box );
  topLayout->addWidget( box );

  QVBoxLayout *layout = new QVBoxLayout;

  QStringList::ConstIterator it;
  QStringList::ConstIterator end( emails.end() );
  for ( it = emails.begin(); it != end; ++it ) {
    QRadioButton *button = new QRadioButton( *it, box );
    d->mButtonGroup->addButton( button );
    d->mEmailMap.insert( button, *it );
    layout->addWidget( button );
    if ( ( *it ) == current ) {
      button->setChecked( true );
    }
  }
  layout->addStretch( 1 );
  box->setLayout( layout );
}

EmailSelector::~EmailSelector()
{
  delete d;
}

QString EmailSelector::selected() const
{
  QAbstractButton *button = d->mButtonGroup->checkedButton();
  if ( !button ) {
    return QString();
  }

  return d->mEmailMap[button];
}

QString EmailSelector::getEmail( const QStringList &emails, const QString &current,
                                 QWidget *parent )
{
  QString email;

  QPointer<EmailSelector> dlg = new EmailSelector( emails, current, parent );
  if ( dlg->exec() && dlg ) {
    email = dlg->selected();
  }

  delete dlg;

  return email;
}

class EntryItem : public QTreeWidgetItem
{
  public:
    EntryItem( QTreeWidget *parent, const Addressee &addressee,
               const QString &email=QString() ) :
      QTreeWidgetItem( parent ),
      mAddressee( addressee ),
      mEmail( email )
    {
      setText( 0, addressee.realName() );
      if ( email.isEmpty() ) {
        setText( 1, addressee.preferredEmail() );
        setText( 2, i18nc( "this the preferred email address", "Yes" ) );
      } else {
        setText( 1, email );
        setText( 2, i18nc( "this is not the preferred email address", "No" ) );
      }
    }

    Addressee addressee() const
    {
      return mAddressee;
    }

    QString email() const
    {
      return mEmail;
    }

  private:
    Addressee mAddressee;
    QString mEmail;
};

class DistributionListEditorWidget::Private
{
  public:
    Private( AddressBook *addressBook, DistributionListEditorWidget *parent )
      : mParent( parent ), mAddressBook( addressBook )
    {
    }

    ~Private()
    {
    }

    void newList();
    void editList();
    void removeList();
    void addEntry();
    void removeEntry();
    void changeEmail();
    void updateEntryView();
    void updateAddresseeView();
    void updateNameCombo();
    void slotSelectionEntryViewChanged();
    void slotSelectionAddresseeViewChanged();
    void save();

    DistributionListEditorWidget *mParent;
    KComboBox *mNameCombo;
    QLabel *mListLabel;
    QTreeWidget *mEntryView;
    QTreeWidget *mAddresseeView;

    AddressBook *mAddressBook;
    QPushButton *mNewButton, *mEditButton, *mRemoveButton;
    QPushButton *mChangeEmailButton, *mRemoveEntryButton, *mAddEntryButton;
};

DistributionListEditorWidget::DistributionListEditorWidget( AddressBook *addressBook,
                                                            QWidget *parent )
  : QWidget( parent ), d( new Private( addressBook, this ) )
{
  qDebug();

  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *nameLayout = new QHBoxLayout();
  topLayout->addLayout( topLayout );

  d->mNameCombo = new KComboBox( this );
  nameLayout->addWidget( d->mNameCombo );
  connect( d->mNameCombo, SIGNAL(activated(int)), SLOT(updateEntryView()) );

  d->mNewButton = new QPushButton( i18n( "New List..." ), this );
  nameLayout->addWidget( d->mNewButton );
  connect( d->mNewButton, SIGNAL(clicked()), SLOT(newList()) );

  d->mEditButton = new QPushButton( i18n( "Rename List..." ), this );
  nameLayout->addWidget( d->mEditButton );
  connect( d->mEditButton, SIGNAL(clicked()), SLOT(editList()) );

  d->mRemoveButton = new QPushButton( i18n( "Remove List" ), this );
  nameLayout->addWidget( d->mRemoveButton );
  connect( d->mRemoveButton, SIGNAL(clicked()), SLOT(removeList()) );

  QGridLayout *gridLayout = new QGridLayout();
  topLayout->addLayout( gridLayout );
  gridLayout->setColumnStretch( 1, 1 );

  QLabel *listLabel = new QLabel( i18n( "Available addresses:" ), this );
  gridLayout->addWidget( listLabel, 0, 0 );

  d->mListLabel = new QLabel( this );
  gridLayout->addWidget( d->mListLabel, 0, 0, 1, 2 );

  d->mAddresseeView = new QTreeWidget( this );
  d->mAddresseeView->setColumnCount( 2 );
  QStringList labels;
  labels << i18nc( "@title:column addressee name", "Name" )
         << i18nc( "@title:column addressee preferred email", "Preferred Email" );
  d->mAddresseeView->setHeaderLabels( labels );
  gridLayout->addWidget( d->mAddresseeView, 1, 0 );
  connect( d->mAddresseeView, SIGNAL(itemSelectionChanged()),
           SLOT(slotSelectionAddresseeViewChanged()) );
  connect( d->mAddresseeView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
           SLOT(addEntry()) );

  d->mAddEntryButton = new QPushButton( i18n( "Add Entry" ), this );
  d->mAddEntryButton->setEnabled( false );
  gridLayout->addWidget( d->mAddEntryButton, 2, 0 );
  connect( d->mAddEntryButton, SIGNAL(clicked()), SLOT(addEntry()) );

  d->mEntryView = new QTreeWidget( this );
  QStringList entryLabels;
  entryLabels << i18nc( "@title:column addressee name", "Name" )
              << i18nc( "@title:column addressee preferred email", "Email" )
              << i18nc( "@title:column use preferred email", "Use Preferred" );
  d->mEntryView->setEnabled( false );
  gridLayout->addWidget( d->mEntryView, 1, 1, 1, 2 );
  connect( d->mEntryView, SIGNAL(itemSelectionChanged()),
           SLOT(slotSelectionEntryViewChanged()) );

  d->mChangeEmailButton = new QPushButton( i18n( "Change Email..." ), this );
  gridLayout->addWidget( d->mChangeEmailButton, 2, 1 );
  connect( d->mChangeEmailButton, SIGNAL(clicked()), SLOT(changeEmail()) );

  d->mRemoveEntryButton = new QPushButton( i18n( "Remove Entry" ), this );
  gridLayout->addWidget( d->mRemoveEntryButton, 2, 2 );
  connect( d->mRemoveEntryButton, SIGNAL(clicked()), SLOT(removeEntry()) );

  d->updateAddresseeView();
  d->updateNameCombo();
}

DistributionListEditorWidget::~DistributionListEditorWidget()
{
  delete d;
}

void DistributionListEditorWidget::Private::save()
{
  // FIXME new distribution list handling
  // do we need extra save?
  //mManager->save();
}

void DistributionListEditorWidget::Private::slotSelectionEntryViewChanged()
{
  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  bool state = selected.count() > 0;
  mChangeEmailButton->setEnabled( state );
  mRemoveEntryButton->setEnabled( state );
}

void DistributionListEditorWidget::Private::newList()
{
  bool ok;
  QString name = KInputDialog::getText( i18n( "New Distribution List" ),
    i18n( "Please enter &name:" ), QString(), &ok );
  if ( !ok ) {
    return;
  }

  mAddressBook->createDistributionList( name );

  mNameCombo->clear();
  mNameCombo->addItems( mAddressBook->allDistributionListNames() );
  mNameCombo->setCurrentIndex( mNameCombo->count() - 1 );

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::Private::editList()
{
  QString oldName = mNameCombo->currentText();
  bool ok;
  QString name = KInputDialog::getText( i18n( "Distribution List" ),
    i18n( "Please change &name:" ), oldName, &ok );
  if ( !ok ) {
    return;
  }

  DistributionList *list = mAddressBook->findDistributionListByName( oldName );
  if ( list ) {
    list->setName( name );
  }

  mNameCombo->clear();
  mNameCombo->addItems( mAddressBook->allDistributionListNames() );
  mNameCombo->setCurrentIndex( mNameCombo->count() - 1 );

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::Private::removeList()
{
  int result = KMessageBox::warningContinueCancel( mParent,
      i18n( "Delete distribution list '%1'?",  mNameCombo->currentText() ),
      QString(), KStandardGuiItem::del() );

  if ( result != KMessageBox::Continue ) {
    return;
  }

  DistributionList *list = mAddressBook->findDistributionListByName( mNameCombo->currentText() );
  if ( list ) {
    // FIXME new distribution list handling
    // list should be deleted, no?
    mAddressBook->removeDistributionList( list );
    mNameCombo->removeItem( mNameCombo->currentIndex() );
  }

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::Private::addEntry()
{
  QList<QTreeWidgetItem*> selected = mAddresseeView->selectedItems();
  if ( selected.count() == 0 ) {
    qDebug() << "No addressee selected.";
    return;
  }
  AddresseeItem *addresseeItem =
    static_cast<AddresseeItem *>( selected.at( 0 ) );

  DistributionList *list = mAddressBook->findDistributionListByName( mNameCombo->currentText() );
  if ( !list ) {
    qDebug() << "No dist list '" << mNameCombo->currentText() << "'";
    return;
  }

  list->insertEntry( addresseeItem->addressee() );
  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::Private::removeEntry()
{
  DistributionList *list = mAddressBook->findDistributionListByName( mNameCombo->currentText() );
  if ( !list ) {
    return;
  }

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  if ( selected.count() == 0 ) {
    return;
  }

  EntryItem *entryItem =
      static_cast<EntryItem *>( selected.at( 0 ) );

  list->removeEntry( entryItem->addressee(), entryItem->email() );
  delete entryItem;
}

void DistributionListEditorWidget::Private::changeEmail()
{
  DistributionList *list = mAddressBook->findDistributionListByName( mNameCombo->currentText() );
  if ( !list ) {
    return;
  }

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  if ( selected.count() == 0 ) {
    return;
  }

  EntryItem *entryItem =
      static_cast<EntryItem *>( selected.at( 0 ) );

  const QString email = EmailSelector::getEmail( entryItem->addressee().emails(),
                                           entryItem->email(), mParent );
  list->removeEntry( entryItem->addressee(), entryItem->email() );
  list->insertEntry( entryItem->addressee(), email );

  updateEntryView();
}

void DistributionListEditorWidget::Private::updateEntryView()
{
  if ( mNameCombo->currentText().isEmpty() ) {
    mListLabel->setText( i18n( "Selected addressees:" ) );
  } else {
    mListLabel->setText( i18n( "Selected addresses in '%1':",
                           mNameCombo->currentText() ) );
  }

  mEntryView->clear();

  DistributionList *list = mAddressBook->findDistributionListByName( mNameCombo->currentText() );
  if ( !list ) {
    mEditButton->setEnabled( false );
    mRemoveButton->setEnabled( false );
    mChangeEmailButton->setEnabled( false );
    mRemoveEntryButton->setEnabled( false );
    mAddresseeView->setEnabled( false );
    mEntryView->setEnabled( false );
    return;
  } else {
    mEditButton->setEnabled( true );
    mRemoveButton->setEnabled( true );
    mAddresseeView->setEnabled( true );
    mEntryView->setEnabled( true );
  }

  DistributionList::Entry::List entries = list->entries();
  DistributionList::Entry::List::ConstIterator it;
  DistributionList::Entry::List::ConstIterator end( entries.constEnd() );
  for ( it = entries.constBegin(); it != end; ++it ) {
    new EntryItem( mEntryView, ( *it ).addressee(), ( *it ).email() );
  }

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  bool state = ( selected.count() != 0 );

  mChangeEmailButton->setEnabled( state );
  mRemoveEntryButton->setEnabled( state );
}

void DistributionListEditorWidget::Private::updateAddresseeView()
{
  mAddresseeView->clear();

  AddressBook::ConstIterator it;
  AddressBook::ConstIterator end( mAddressBook->constEnd() );
  for ( it = mAddressBook->constBegin(); it != end; ++it ) {
    new AddresseeItem( mAddresseeView, *it );
  }
}

void DistributionListEditorWidget::Private::updateNameCombo()
{
  mNameCombo->addItems( mAddressBook->allDistributionListNames() );

  updateEntryView();
}

void DistributionListEditorWidget::Private::slotSelectionAddresseeViewChanged()
{
  QList<QTreeWidgetItem*> selected = mAddresseeView->selectedItems();
  bool state = ( selected.count() != 0 );
  mAddEntryButton->setEnabled( state && !mNameCombo->currentText().isEmpty() );
}

#include "moc_distributionlistdialog.cpp"

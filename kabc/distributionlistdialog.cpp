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

#include <QTreeWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <kinputdialog.h>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>

#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>

#include "addressbook.h"
#include "addresseedialog.h"
#include "distributionlist.h"

#include "distributionlistdialog.h"
#include "distributionlistdialog.moc"

using namespace KABC;

DistributionListDialog::DistributionListDialog( AddressBook *addressBook, QWidget *parent)
    : KDialog( parent )
{
  setModal( true );
  setCaption( i18n("Configure Distribution Lists") );
  setButtons( Ok );
  setDefaultButton( Ok );
  enableButtonSeparator( true );

  mEditor = new DistributionListEditorWidget( addressBook, this );
  setMainWidget( mEditor );

  connect( this, SIGNAL( okClicked() ), mEditor, SLOT( save() ) );
}

DistributionListDialog::~DistributionListDialog()
{
}

// TODO KDE4: Add d-pointer to EmailSelector, make sEmailMap a member variable
static QMap<QWidget*,QString> *sEmailMap = 0;

EmailSelector::EmailSelector( const QStringList &emails, const QString &current,
                                      QWidget *parent ) :
  KDialog( parent )
{
  setCaption( i18n("Select Email Address") );
  setButtons( Ok );
  setDefaultButton( Ok );
              
  if (!sEmailMap)
     sEmailMap = new QMap<QWidget*,QString>();

  QFrame *topFrame = new QFrame( this );
  setMainWidget( topFrame );

  QBoxLayout *topLayout = new QVBoxLayout( topFrame );

  QGroupBox *box = new QGroupBox( i18n("Email Addresses") );
  mButtonGroup = new QButtonGroup( box );
  topLayout->addWidget( box );

  QStringList::ConstIterator it;
  for ( it = emails.begin(); it != emails.end(); ++it ) {
    QRadioButton *button = new QRadioButton( *it, box );
    mButtonGroup->addButton( button );
    sEmailMap->insert( button, *it );
    if ( (*it) == current ) {
      button->setChecked( true );
    }
  }
}

QString EmailSelector::selected()
{
  QAbstractButton *button = mButtonGroup->checkedButton();
  if ( button ) return (*sEmailMap)[button];
  return QString();
}

QString EmailSelector::getEmail( const QStringList &emails, const QString &current,
                                     QWidget *parent )
{
  EmailSelector *dlg = new EmailSelector( emails, current, parent );
  dlg->exec();

  QString result = dlg->selected();

  delete dlg;

  return result;
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
        setText( 2, i18n("Yes") );
      } else {
        setText( 1, email );
        setText( 2, i18n("No") );
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

DistributionListEditorWidget::DistributionListEditorWidget( AddressBook *addressBook, QWidget *parent) :
  QWidget( parent ),
  mAddressBook( addressBook )
{
  kDebug(5700) << "DistributionListEditor()" << endl;

  QBoxLayout *topLayout = new QVBoxLayout( this );
  topLayout->setSpacing( KDialog::spacingHint() );

  QBoxLayout *nameLayout = new QHBoxLayout() ;
  topLayout->addLayout( topLayout );

  mNameCombo = new QComboBox( this );
  nameLayout->addWidget( mNameCombo );
  connect( mNameCombo, SIGNAL( activated( int ) ), SLOT( updateEntryView() ) );

  mNewButton = new QPushButton( i18n("New List..."), this );
  nameLayout->addWidget( mNewButton );
  connect( mNewButton, SIGNAL( clicked() ), SLOT( newList() ) );

  mEditButton = new QPushButton( i18n("Rename List..."), this );
  nameLayout->addWidget( mEditButton );
  connect( mEditButton, SIGNAL( clicked() ), SLOT( editList() ) );

  mRemoveButton = new QPushButton( i18n("Remove List"), this );
  nameLayout->addWidget( mRemoveButton );
  connect( mRemoveButton, SIGNAL( clicked() ), SLOT( removeList() ) );

  QGridLayout *gridLayout = new QGridLayout();
  topLayout->addLayout( gridLayout );
  gridLayout->setColumnStretch(1, 1);

  QLabel *listLabel = new QLabel( i18n("Available addresses:"), this );
  gridLayout->addWidget( listLabel, 0, 0 );

  mListLabel = new QLabel( this );
  gridLayout->addWidget( mListLabel, 0, 0, 1, 2 );

  mAddresseeView = new QTreeWidget( this );
  mAddresseeView->setColumnCount( 2 );
  QStringList labels;
  labels << i18n("Name") << i18n("Preferred Email");
  mAddresseeView->setHeaderLabels( labels );
  gridLayout->addWidget( mAddresseeView, 1, 0 );
  connect( mAddresseeView, SIGNAL( itemSelectionChanged() ),
           SLOT( slotSelectionAddresseeViewChanged() ) );
  connect( mAddresseeView, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ),
           SLOT( addEntry() ) );

  mAddEntryButton = new QPushButton( i18n("Add Entry"), this );
  mAddEntryButton->setEnabled(false);
  gridLayout->addWidget( mAddEntryButton, 2, 0 );
  connect( mAddEntryButton, SIGNAL( clicked() ), SLOT( addEntry() ) );

  mEntryView = new QTreeWidget( this );
  QStringList entryLabels;
  entryLabels << i18n("Name") << i18n("Email") << i18n("Use Preferred");
  mEntryView->setEnabled(false);
  gridLayout->addWidget( mEntryView, 1, 1, 1, 2 );
  connect( mEntryView, SIGNAL( itemSelectionChanged() ),
           SLOT( slotSelectionEntryViewChanged() ) );

  mChangeEmailButton = new QPushButton( i18n("Change Email..."), this );
  gridLayout->addWidget( mChangeEmailButton, 2, 1 );
  connect( mChangeEmailButton, SIGNAL( clicked() ), SLOT( changeEmail() ) );

  mRemoveEntryButton = new QPushButton( i18n("Remove Entry"), this );
  gridLayout->addWidget( mRemoveEntryButton, 2, 2 );
  connect( mRemoveEntryButton, SIGNAL( clicked() ), SLOT( removeEntry() ) );

  mManager = new DistributionListManager( mAddressBook );
  mManager->load();

  updateAddresseeView();
  updateNameCombo();
}

DistributionListEditorWidget::~DistributionListEditorWidget()
{
  kDebug(5700) << "~DistributionListEditor()" << endl;

  delete mManager;
}

void DistributionListEditorWidget::save()
{
  mManager->save();
}

void DistributionListEditorWidget::slotSelectionEntryViewChanged()
{
  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  bool state = selected.count() > 0;
  mChangeEmailButton->setEnabled(state);
  mRemoveEntryButton->setEnabled(state);
}

void DistributionListEditorWidget::newList()
{
  bool ok;
  QString name = KInputDialog::getText( i18n( "New Distribution List" ),
    i18n( "Please enter &name:" ), QString(), &ok );
  if (!ok) return;

  new DistributionList( mManager, name );

  mNameCombo->clear();
  mNameCombo->addItems( mManager->listNames() );
  mNameCombo->setCurrentIndex( mNameCombo->count() - 1 );

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::editList()
{
  QString oldName = mNameCombo->currentText();
  bool ok;
  QString name = KInputDialog::getText( i18n( "Distribution List" ),
    i18n( "Please change &name:" ), oldName, &ok );
  if (!ok) return;

  DistributionList *list = mManager->list( oldName );
  list->setName( name );

  mNameCombo->clear();
  mNameCombo->addItems( mManager->listNames() );
  mNameCombo->setCurrentIndex( mNameCombo->count() - 1 );

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::removeList()
{
  int result = KMessageBox::warningContinueCancel( this,
      i18n("Delete distribution list '%1'?" ,  mNameCombo->currentText() ),
      QString(), KStdGuiItem::del() );

  if ( result != KMessageBox::Continue ) return;

  mManager->remove( mManager->list( mNameCombo->currentText() ) );
  mNameCombo->removeItem( mNameCombo->currentIndex() );

  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::addEntry()
{
  QList<QTreeWidgetItem*> selected = mAddresseeView->selectedItems();
  if ( selected.count() == 0 ) {
    kDebug(5700) << "DLE::addEntry(): No addressee selected." << endl;
    return;
  }
  AddresseeItem *addresseeItem =
    static_cast<AddresseeItem *>( selected.at( 0 ) );

  DistributionList *list = mManager->list( mNameCombo->currentText() );
  if ( !list ) {
    kDebug(5700) << "DLE::addEntry(): No dist list '" << mNameCombo->currentText() << "'" << endl;
    return;
  }

  list->insertEntry( addresseeItem->addressee() );
  updateEntryView();
  slotSelectionAddresseeViewChanged();
}

void DistributionListEditorWidget::removeEntry()
{
  DistributionList *list = mManager->list( mNameCombo->currentText() );
  if ( !list ) return;

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  if ( selected.count() == 0 ) return;
  EntryItem *entryItem =
      static_cast<EntryItem *>( selected.at( 0 ) );

  list->removeEntry( entryItem->addressee(), entryItem->email() );
  delete entryItem;
}

void DistributionListEditorWidget::changeEmail()
{
  DistributionList *list = mManager->list( mNameCombo->currentText() );
  if ( !list ) return;

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  if ( selected.count() == 0 ) return;
  EntryItem *entryItem =
      static_cast<EntryItem *>( selected.at( 0 ) );

  QString email = EmailSelector::getEmail( entryItem->addressee().emails(),
                                           entryItem->email(), this );
  list->removeEntry( entryItem->addressee(), entryItem->email() );
  list->insertEntry( entryItem->addressee(), email );

  updateEntryView();
}

void DistributionListEditorWidget::updateEntryView()
{
  if ( mNameCombo->currentText().isEmpty() ) {
    mListLabel->setText( i18n("Selected addressees:") );
  } else {
    mListLabel->setText( i18n("Selected addresses in '%1':",
                           mNameCombo->currentText() ) );
  }

  mEntryView->clear();

  DistributionList *list = mManager->list( mNameCombo->currentText() );
  if ( !list ) {
    mEditButton->setEnabled(false);
    mRemoveButton->setEnabled(false);
    mChangeEmailButton->setEnabled(false);
    mRemoveEntryButton->setEnabled(false);
    mAddresseeView->setEnabled(false);
    mEntryView->setEnabled(false);
    return;
  } else {
    mEditButton->setEnabled(true);
    mRemoveButton->setEnabled(true);
    mAddresseeView->setEnabled(true);
    mEntryView->setEnabled(true);
  }

  DistributionList::Entry::List entries = list->entries();
  DistributionList::Entry::List::ConstIterator it;
  for ( it = entries.begin(); it != entries.end(); ++it ) {
    new EntryItem( mEntryView, (*it).addressee, (*it).email );
  }

  QList<QTreeWidgetItem*> selected = mEntryView->selectedItems();
  bool state = ( selected.count() != 0 );

  mChangeEmailButton->setEnabled(state);
  mRemoveEntryButton->setEnabled(state);
}

void DistributionListEditorWidget::updateAddresseeView()
{
  mAddresseeView->clear();

  AddressBook::Iterator it;
  for ( it = mAddressBook->begin(); it != mAddressBook->end(); ++it ) {
    new AddresseeItem( mAddresseeView, *it );
  }
}

void DistributionListEditorWidget::updateNameCombo()
{
  mNameCombo->addItems( mManager->listNames() );

  updateEntryView();
}

void DistributionListEditorWidget::slotSelectionAddresseeViewChanged()
{
  QList<QTreeWidgetItem*> selected = mAddresseeView->selectedItems();
  bool state = (selected.count() != 0 );
  mAddEntryButton->setEnabled( state && !mNameCombo->currentText().isEmpty());
}

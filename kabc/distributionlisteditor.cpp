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


#include <QButtonGroup>
#include <QComboBox>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>

#include <kdebug.h>
#include <kinputdialog.h>
#include <klocale.h>

#include "addressbook.h"
#include "addresseedialog.h"
#include "distributionlist.h"

#include "distributionlisteditor.h"
#include "distributionlisteditor.moc"

using namespace KABC;

EmailSelectDialog::EmailSelectDialog( const QStringList &emails, const QString &current,
                                      QWidget *parent ) :
  KDialog( parent )
{
  setCaption( i18n("Select Email Address") );
  setButtons( Ok );
  setDefaultButton( Ok );

  QFrame *topFrame = new QFrame( this );
  setMainWidget( topFrame );

  QBoxLayout *topLayout = new QVBoxLayout( topFrame );
  QGroupBox *box = new QGroupBox( i18n("Email Addresses") );
  mButtonGroup = new QButtonGroup( box );
  mButtonGroup->setExclusive( true );
  topLayout->addWidget( box );

  QStringList::ConstIterator it;
  for ( it = emails.begin(); it != emails.end(); ++it ) {
    QRadioButton *button = new QRadioButton( *it, box );
    mButtonGroup->addButton( button );
    if ( (*it) == current ) {
      button->setChecked( true );
    }
  }
}

QString EmailSelectDialog::selected()
{
  QAbstractButton *button = mButtonGroup->checkedButton();
  if ( button ) return button->text();
  return QString();
}

QString EmailSelectDialog::getEmail( const QStringList &emails, const QString &current,
                                     QWidget *parent )
{
  EmailSelectDialog *dlg = new EmailSelectDialog( emails, current, parent );
  dlg->exec();

  QString result = dlg->selected();

  delete dlg;

  return result;
}

class EditEntryItem : public QTreeWidgetItem
{
  public:
    EditEntryItem( QTreeWidget *parent, const Addressee &addressee,
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

/*
    This file is part of libkabc.
    Copyright (c) 2002 Helge Deller <deller@gmx.de>
                  2002 Lubos Lunak <llunak@suse.cz>
                  2001,2003 Carsten Pfeiffer <pfeiffer@kde.org>
                  2001 Waldo Bastian <bastian@kde.org>

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

#include "addresslineedit.h"

#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtCore/QObject>
#include <QtCore/QRegExp>

#include <kcompletionbox.h>
#include <kconfig.h>
#include <kcursor.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kstandardshortcut.h>

#include "stdaddressbook.h"

//=============================================================================
//
//   Class  AddressLineEdit
//
//=============================================================================

using namespace KABC;

class AddressLineEdit::Private
{
  public:
    Private( AddressLineEdit *parent )
      : mParent( parent ),
        mCompletionInitialized( false ),
        mSmartPaste( false )
    {
      init();
    }

    void init();
    QStringList addresses();
    QStringList removeMailDupes( const QStringList &adrs );

    void slotCompletion() { mParent->doCompletion( false ); }
    void slotPopupCompletion( const QString &completion );

    AddressLineEdit *mParent;
    QString mPreviousAddresses;
    bool mUseCompletion;
    bool mCompletionInitialized;
    bool mSmartPaste;

    static bool sAddressesDirty;
    static bool initialized;
};
bool AddressLineEdit::Private::initialized = false;
K_GLOBAL_STATIC( KCompletion, sCompletion )

void AddressLineEdit::Private::init()
{
  if ( !Private::initialized ) {
      Private::initialized = true;
      sCompletion->setOrder( KCompletion::Sorted );
      sCompletion->setIgnoreCase( true );
  }

  if ( mUseCompletion && !mCompletionInitialized ) {
      mParent->setCompletionObject( sCompletion, false ); // we handle it ourself
      mParent->connect( mParent, SIGNAL( completion( const QString& ) ),
                        mParent, SLOT( slotCompletion() ) );

      KCompletionBox *box = mParent->completionBox();
      mParent->connect( box, SIGNAL( highlighted( const QString& ) ),
                        mParent, SLOT( slotPopupCompletion( const QString& ) ) );
      mParent->connect( box, SIGNAL( userCancelled( const QString& ) ),
                        SLOT( userCancelled( const QString& ) ) );

      mCompletionInitialized = true; // don't connect muliple times. That's
                                      // ugly, tho, better have completionBox()
                                      // virtual in KDE 4
      // Why? This is only called once. Why should this be called more
      // than once? And why was this protected?
  }
}

QStringList AddressLineEdit::Private::addresses()
{
  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) ); // loading might take a while

  QStringList result;
  QString space( " " );
  QRegExp needQuotes( "[^ 0-9A-Za-z\\x0080-\\xFFFF]" );
  QString endQuote( "\" " );
  QString addr, email;

  KABC::AddressBook *addressBook = KABC::StdAddressBook::self();
  KABC::AddressBook::Iterator it;
  for ( it = addressBook->begin(); it != addressBook->end(); ++it ) {
    QStringList emails = (*it).emails();

    QString n = (*it).prefix() + space +
                (*it).givenName() + space +
                (*it).additionalName() + space +
                (*it).familyName() + space +
                (*it).suffix();

    n = n.simplified();

    QStringList::ConstIterator mit;

    for ( mit = emails.begin(); mit != emails.end(); ++mit ) {
      email = *mit;
      if ( !email.isEmpty() ) {
        if ( n.isEmpty() || ( email.indexOf( '<' ) != -1 ) ) {
          addr.clear();
        } else { /* do we really need quotes around this name ? */
          if ( n.indexOf( needQuotes ) != -1 ) {
            addr = '"' + n + endQuote;
          } else {
            addr = n + space;
          }
        }

        if ( !addr.isEmpty() && ( email.indexOf( '<' ) == -1 ) &&
             ( email.indexOf( '>' ) == -1 ) &&
             ( email.indexOf( ',' ) == -1 ) ) {
          addr += '<' + email + '>';
        } else {
          addr += email;
        }
        addr = addr.trimmed();
        result.append( addr );
      }
    }
  }

  result += addressBook->allDistributionListNames();

  QApplication::restoreOverrideCursor();

  return result;
}

QStringList AddressLineEdit::Private::removeMailDupes( const QStringList &addrs )
{
  QStringList src( addrs );
  qSort( src );

  QString last;
  for ( QStringList::Iterator it = src.begin(); it != src.end(); ) {
    if ( *it == last ) {
      it = src.erase( it );
      continue; // dupe
    }

    last = *it;
    ++it;
  }

  return src;
}

void AddressLineEdit::Private::slotPopupCompletion( const QString &completion )
{
  mParent->setText( mPreviousAddresses + completion );
  mParent->cursorAtEnd();
}

bool AddressLineEdit::Private::sAddressesDirty = false;

AddressLineEdit::AddressLineEdit( QWidget *parent, bool useCompletion )
  : KLineEdit( parent ), d( new Private( this ) )
{
  d->mUseCompletion = useCompletion;

  // Whenever a new AddressLineEdit is created (== a new composer is created),
  // we set a dirty flag to reload the addresses upon the first completion.
  // The address completions are shared between all AddressLineEdits.
  // Is there a signal that tells us about addressbook updates?
  if ( d->mUseCompletion ) {
    d->sAddressesDirty = true;
  }
}

//-----------------------------------------------------------------------------
AddressLineEdit::~AddressLineEdit()
{
  delete d;
}

//-----------------------------------------------------------------------------

void AddressLineEdit::setFont( const QFont &font )
{
  KLineEdit::setFont( font );
  if ( d->mUseCompletion ) {
    completionBox()->setFont( font );
  }
}

//-----------------------------------------------------------------------------
void AddressLineEdit::keyPressEvent( QKeyEvent *event )
{
  bool accept = false;

  if ( KStandardShortcut::shortcut( KStandardShortcut::SubstringCompletion ).
       contains( event->key() | event->modifiers() ) ) {
    doCompletion( true );
    accept = true;
  } else if ( KStandardShortcut::shortcut( KStandardShortcut::TextCompletion ).
              contains( event->key() | event->modifiers() ) ) {
    int len = text().length();

    if ( len == cursorPosition() ) { // at End?
      doCompletion( true );
      accept = true;
    }
  }

  if ( !accept ) {
    KLineEdit::keyPressEvent( event );
  }
}

void AddressLineEdit::mouseReleaseEvent( QMouseEvent *event )
{
  if ( d->mUseCompletion && ( event->button() == Qt::MidButton ) ) {
    d->mSmartPaste = true;
    KLineEdit::mouseReleaseEvent( event );
    d->mSmartPaste = false;
    return;
  }

  KLineEdit::mouseReleaseEvent( event );
}

void AddressLineEdit::insert( const QString &oldText )
{
  if ( !d->mSmartPaste ) {
    KLineEdit::insert( oldText );
    return;
  }

  QString newText = oldText.trimmed();
  if ( newText.isEmpty() ) {
    return;
  }

  // remove newlines in the to-be-pasted string as well as an eventual
  // mailto: protocol
  newText.replace( QRegExp( "\r?\n" ), ", " );
  if ( newText.startsWith( "mailto:" ) ) {
    KUrl u( newText );
    newText = u.path();
  } else if ( newText.indexOf( " at " ) != -1 ) {
    // Anti-spam stuff
    newText.replace( " at ", "@" );
    newText.replace( " dot ", "." );
  } else if ( newText.indexOf( "(at)" ) != -1 ) {
    newText.replace( QRegExp( "\\s*\\(at\\)\\s*" ), "@" );
  }

  QString contents = text();
  int start_sel = 0;
  int end_sel = 0;
  int pos = cursorPosition();
  if ( !selectedText().isEmpty() ) {
    // Cut away the selection.
    if ( pos > end_sel ) {
      pos -= ( end_sel - start_sel );
    } else if ( pos > start_sel ) {
      pos = start_sel;
    }
    contents = contents.left( start_sel ) + contents.right( end_sel + 1 );
  }

  int eot = contents.length();
  while ( ( eot > 0 ) && contents[ eot - 1 ].isSpace() ) {
    eot--;
  }

  if ( eot == 0 ) {
    contents.clear();
  } else if ( pos >= eot ) {
    if ( contents[ eot - 1 ] == ',' ) {
      eot--;
    }
    contents.truncate( eot );
    contents += ", ";
    pos = eot+2;
  }

  contents = contents.left( pos ) + newText + contents.mid( pos );
  setText( contents );
  setCursorPosition( pos + newText.length() );
}

void AddressLineEdit::paste()
{
  if ( d->mUseCompletion ) {
    d->mSmartPaste = true;
  }

  KLineEdit::paste();
  d->mSmartPaste = false;
}

//-----------------------------------------------------------------------------
void AddressLineEdit::cursorAtEnd()
{
  setCursorPosition( text().length() );
}

//-----------------------------------------------------------------------------
void AddressLineEdit::enableCompletion( bool enable )
{
  d->mUseCompletion = enable;
}

//-----------------------------------------------------------------------------
void AddressLineEdit::doCompletion( bool ctrlT )
{
  if ( !d->mUseCompletion ) {
      return;
  }

  QString prevAddr;

  QString s( text() );
  int n = s.lastIndexOf( ',' );

  if ( n >= 0 ) {
    n++; // Go past the ","

    int len = s.length();

    // Increment past any whitespace...
    while ( n < len && s[ n ].isSpace() ) {
      n++;
    }

    prevAddr = s.left( n );
    s = s.mid( n, 255 ).trimmed();
  }

  if ( d->sAddressesDirty ) {
    loadAddresses();
  }

  if ( ctrlT ) {
    QStringList completions = sCompletion->substringCompletion( s );
    if ( completions.count() > 1 ) {
      d->mPreviousAddresses = prevAddr;
      setCompletedItems( completions );
    } else if ( completions.count() == 1 ) {
      setText( prevAddr + completions.first() );
    }

    cursorAtEnd();
    return;
  }

  KGlobalSettings::Completion mode = completionMode();

  switch ( mode ) {
    case KGlobalSettings::CompletionPopupAuto:
    {
      if ( s.isEmpty() ) {
        break;
      }
    }
    case KGlobalSettings::CompletionPopup:
    {
      d->mPreviousAddresses = prevAddr;
      QStringList items = sCompletion->allMatches( s );
      items += sCompletion->allMatches( "\"" + s );
      items += sCompletion->substringCompletion( '<' + s );
      int beforeDollarCompletionCount = items.count();

      if ( s.indexOf( ' ' ) == -1 ) { // one word, possibly given name
        items += sCompletion->allMatches( "$$" + s );
      }

      if ( !items.isEmpty() ) {
        if ( items.count() > beforeDollarCompletionCount ) {
          // remove the '$$whatever$' part
          for ( QStringList::Iterator it = items.begin();
                it != items.end(); ++it ) {
            int pos = (*it).indexOf( '$', 2 );
            if ( pos < 0 ) { // ???
              continue;
            }
            (*it) = (*it).mid( pos + 1 );
          }
        }

        items = d->removeMailDupes( items );

        // We do not want KLineEdit::setCompletedItems to perform text
        // completion (suggestion) since it does not know how to deal
        // with providing proper completions for different items on the
        // same line, e.g. comma-separated list of email addresses.
        bool autoSuggest = ( mode != KGlobalSettings::CompletionPopupAuto );
        setCompletedItems( items, autoSuggest );

        if ( !autoSuggest ) {
          int index = items.first().indexOf( s );
          QString newText = prevAddr + items.first().mid( index );
          //kDebug() << "OLD TEXT:" << text();
          //kDebug() << "NEW TEXT:" << newText;
          setUserSelection( false );
          setCompletedText( newText, true );
        }
      }

      break;
    }

    case KGlobalSettings::CompletionShell:
    {
      QString match = sCompletion->makeCompletion( s );
      if ( !match.isNull() && match != s ) {
        setText( prevAddr + match );
        cursorAtEnd();
      }
      break;
    }

    case KGlobalSettings::CompletionMan: // Short-Auto in fact
    case KGlobalSettings::CompletionAuto:
    {
      if ( !s.isEmpty() ) {
        QString match = sCompletion->makeCompletion( s );
        if ( !match.isNull() && match != s ) {
          setCompletedText( prevAddr + match );
        }

        break;
      }
    }
    case KGlobalSettings::CompletionNone:
    default: // fall through
        break;
  }
}

//-----------------------------------------------------------------------------
void AddressLineEdit::loadAddresses()
{
  sCompletion->clear();
  d->sAddressesDirty = false;

  const QStringList addrs = d->addresses();
  for ( QStringList::ConstIterator it = addrs.begin(); it != addrs.end(); ++it ) {
    addAddress( *it );
  }
}

void AddressLineEdit::addAddress( const QString &addr )
{
  sCompletion->addItem( addr );

  int pos = addr.indexOf( '<' );
  if ( pos >= 0 ) {
    ++pos;
    int pos2 = addr.indexOf( pos, '>' );
    if ( pos2 >= 0 ) {
      sCompletion->addItem( addr.mid( pos, pos2 - pos ) );
    }
  }
}

//-----------------------------------------------------------------------------
void AddressLineEdit::dropEvent( QDropEvent *event )
{
  const KUrl::List uriList = KUrl::List::fromMimeData( event->mimeData() );
  if ( !uriList.isEmpty() ) {
    QString ct = text();
    KUrl::List::ConstIterator it = uriList.begin();
    for ( ; it != uriList.end(); ++it ) {
      if ( !ct.isEmpty() ) {
        ct.append( ", " );
      }

      KUrl u( *it );
      if ( (*it).protocol() == "mailto" ) {
        ct.append( (*it).path() );
      } else {
        ct.append( (*it).url() );
      }
    }
    setText( ct );
    setModified( true );
  } else {
    if ( d->mUseCompletion ) {
       d->mSmartPaste = true;
    }

    KLineEdit::dropEvent( event );
    d->mSmartPaste = false;
  }
}

#include "addresslineedit.moc"

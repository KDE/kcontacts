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
#include <QtCore/QTimer>

#include <kcompletionbox.h>
#include <kconfig.h>
#include <kcursor.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <kstaticdeleter.h>
#include <kstandardshortcut.h>

#include "distributionlist.h"
#include "stdaddressbook.h"


//=============================================================================
//
//   Class  AddressLineEdit
//
//=============================================================================


using namespace KABC;

KCompletion * AddressLineEdit::s_completion = 0L;
bool AddressLineEdit::s_addressesDirty = false;

static KStaticDeleter<KCompletion> completionDeleter;

AddressLineEdit::AddressLineEdit(QWidget* parent,
		bool useCompletion)
    : KLineEdit(parent)
{
  m_useCompletion = useCompletion;
  m_completionInitialized = false;
  m_smartPaste = false;

  init();

  // Whenever a new AddressLineEdit is created (== a new composer is created),
  // we set a dirty flag to reload the addresses upon the first completion.
  // The address completions are shared between all AddressLineEdits.
  // Is there a signal that tells us about addressbook updates?
  if (m_useCompletion)
    s_addressesDirty = true;
}


//-----------------------------------------------------------------------------
void AddressLineEdit::init()
{
  if ( !s_completion ) {
      completionDeleter.setObject( s_completion, new KCompletion() );
      s_completion->setOrder( KCompletion::Sorted );
      s_completion->setIgnoreCase( true );
  }

  if ( m_useCompletion && !m_completionInitialized )
  {
      setCompletionObject( s_completion, false ); // we handle it ourself
      connect( this, SIGNAL( completion(const QString&)),
               this, SLOT(slotCompletion() ));

      KCompletionBox *box = completionBox();
      connect( box, SIGNAL( highlighted( const QString& )),
               this, SLOT( slotPopupCompletion( const QString& ) ));
      connect( box, SIGNAL( userCancelled( const QString& )),
               SLOT( userCancelled( const QString& )));

      m_completionInitialized = true; // don't connect muliple times. That's
                                      // ugly, tho, better have completionBox()
                                      // virtual in KDE 4
      // Why? This is only called once. Why should this be called more
      // than once? And why was this protected?
  }
}

//-----------------------------------------------------------------------------
AddressLineEdit::~AddressLineEdit()
{
}

//-----------------------------------------------------------------------------

void AddressLineEdit::setFont( const QFont& font )
{
    KLineEdit::setFont( font );
    if ( m_useCompletion )
        completionBox()->setFont( font );
}

//-----------------------------------------------------------------------------
void AddressLineEdit::keyPressEvent(QKeyEvent *e)
{
    bool accept = false;

    if (KStandardShortcut::shortcut(KStandardShortcut::SubstringCompletion).contains(e->key() | e->modifiers()))
    {
        doCompletion(true);
        accept = true;
    }
    else if (KStandardShortcut::shortcut(KStandardShortcut::TextCompletion).contains(e->key() | e->modifiers()))
    {
        int len = text().length();

        if (len == cursorPosition()) // at End?
        {
            doCompletion(true);
            accept = true;
        }
    }

    if ( !accept )
        KLineEdit::keyPressEvent( e );
}

void AddressLineEdit::mouseReleaseEvent( QMouseEvent * e )
{
   if (m_useCompletion && (e->button() == Qt::MidButton))
   {
      m_smartPaste = true;
      KLineEdit::mouseReleaseEvent(e);
      m_smartPaste = false;
      return;
   }
   KLineEdit::mouseReleaseEvent(e);
}

void AddressLineEdit::insert(const QString &t)
{
    if (!m_smartPaste)
    {
       KLineEdit::insert(t);
       return;
    }
    QString newText = t.trimmed();
    if (newText.isEmpty())
       return;

    // remove newlines in the to-be-pasted string as well as an eventual
    // mailto: protocol
    newText.replace( QRegExp("\r?\n"), ", " );
    if ( newText.startsWith( "mailto:" ) )
    {
      KUrl u(newText);
      newText = u.path();
    }
    else if (newText.indexOf(" at ") != -1)
    {
       // Anti-spam stuff
       newText.replace( " at ", "@" );
       newText.replace( " dot ", "." );
    }
    else if (newText.indexOf("(at)") != -1)
    {
      newText.replace( QRegExp("\\s*\\(at\\)\\s*"), "@" );
    }

    QString contents = text();
    int start_sel = 0;
    int end_sel = 0;
    int pos = cursorPosition();
    if (!selectedText().isEmpty())
    {
       // Cut away the selection.
       if (pos > end_sel)
          pos -= (end_sel - start_sel);
       else if (pos > start_sel)
          pos = start_sel;
       contents = contents.left(start_sel) + contents.right(end_sel+1);
    }

    int eot = contents.length();
    while ((eot > 0) && contents[eot-1].isSpace()) eot--;
    if (eot == 0)
    {
       contents.clear();
    }
    else if (pos >= eot)
    {
       if (contents[eot-1] == ',')
          eot--;
       contents.truncate(eot);
       contents += ", ";
       pos = eot+2;
    }

    contents = contents.left(pos)+newText+contents.mid(pos);
    setText(contents);
    setCursorPosition(pos+newText.length());
}

void AddressLineEdit::paste()
{
    if (m_useCompletion)
       m_smartPaste = true;
    KLineEdit::paste();
    m_smartPaste = false;
}

//-----------------------------------------------------------------------------
void AddressLineEdit::cursorAtEnd()
{
    setCursorPosition( text().length() );
}

//-----------------------------------------------------------------------------
void AddressLineEdit::enableCompletion(bool enable)
{
  m_useCompletion = enable;
}

//-----------------------------------------------------------------------------
void AddressLineEdit::doCompletion(bool ctrlT)
{
    if ( !m_useCompletion )
        return;

    QString prevAddr;

    QString s(text());
    int n = s.lastIndexOf(',');

    if (n >= 0)
    {
        n++; // Go past the ","

        int len = s.length();

        // Increment past any whitespace...
        while ( n < len && s[n].isSpace() )
          n++;

        prevAddr = s.left(n);
        s = s.mid(n,255).trimmed();
    }

    if ( s_addressesDirty )
        loadAddresses();

    if ( ctrlT )
    {
        QStringList completions = s_completion->substringCompletion( s );
        if (completions.count() > 1) {
            m_previousAddresses = prevAddr;
            setCompletedItems( completions );
        }
        else if (completions.count() == 1)
            setText(prevAddr + completions.first());

        cursorAtEnd();
        return;
    }

    KGlobalSettings::Completion  mode = completionMode();

    switch ( mode )
    {
        case KGlobalSettings::CompletionPopupAuto:
        {
            if (s.isEmpty())
                break;
        }
        case KGlobalSettings::CompletionPopup:
        {
            m_previousAddresses = prevAddr;
            QStringList items = s_completion->allMatches( s );
            items += s_completion->allMatches( "\"" + s );
            items += s_completion->substringCompletion( '<' + s );
            int beforeDollarCompletionCount = items.count();

            if ( s.indexOf( ' ' ) == -1 ) // one word, possibly given name
                items += s_completion->allMatches( "$$" + s );

            if ( !items.isEmpty() )
            {
                if ( items.count() > beforeDollarCompletionCount )
                {
                    // remove the '$$whatever$' part
                    for ( QStringList::Iterator it = items.begin();
                         it != items.end();
                         ++it )
                    {
                        int pos = (*it).indexOf( '$', 2 );
                        if ( pos < 0 ) // ???
                            continue;
                        (*it)=(*it).mid( pos + 1 );
                    }
                }

                items = removeMailDupes( items );

                // We do not want KLineEdit::setCompletedItems to perform text
                // completion (suggestion) since it does not know how to deal
                // with providing proper completions for different items on the
                // same line, e.g. comma-separated list of email addresses.
                bool autoSuggest = (mode != KGlobalSettings::CompletionPopupAuto);
                setCompletedItems( items, autoSuggest );

                if (!autoSuggest)
                {
                    int index = items.first().indexOf( s );
                    QString newText = prevAddr + items.first().mid( index );
                    //kDebug() << "OLD TEXT: " << text() << endl;
                    //kDebug() << "NEW TEXT: " << newText << endl;
                    setUserSelection(false);
                    setCompletedText(newText,true);
                }
            }

            break;
        }

        case KGlobalSettings::CompletionShell:
        {
            QString match = s_completion->makeCompletion( s );
            if ( !match.isNull() && match != s )
            {
                setText( prevAddr + match );
                cursorAtEnd();
            }
            break;
        }

        case KGlobalSettings::CompletionMan: // Short-Auto in fact
        case KGlobalSettings::CompletionAuto:
        {
            if (!s.isEmpty())
            {
                QString match = s_completion->makeCompletion( s );
                if ( !match.isNull() && match != s )
                {
                  QString adds = prevAddr + match;
                  setCompletedText( adds );
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
void AddressLineEdit::slotPopupCompletion( const QString& completion )
{
    setText( m_previousAddresses + completion );
    cursorAtEnd();
}

//-----------------------------------------------------------------------------
void AddressLineEdit::loadAddresses()
{
    s_completion->clear();
    s_addressesDirty = false;

    QStringList adrs = addresses();
    for ( QStringList::ConstIterator it = adrs.begin(); it != adrs.end(); ++it)
        addAddress( *it );
}

void AddressLineEdit::addAddress( const QString& adr )
{
    s_completion->addItem( adr );
    int pos = adr.indexOf( '<' );
    if ( pos >= 0 )
    {
        ++pos;
        int pos2 = adr.indexOf( pos, '>' );
        if ( pos2 >= 0 )
            s_completion->addItem( adr.mid( pos, pos2 - pos ));
    }
}

QStringList AddressLineEdit::removeMailDupes( const QStringList& adrs )
{
    QStringList src = adrs;
    qSort( src );
    QString last;
    for ( QStringList::Iterator it = src.begin(); it != src.end(); ) {
        if ( *it == last )
        {
            it = src.erase( it );
            continue; // dupe
        }
        last = *it;
        ++it;
    }
    return src;
}

//-----------------------------------------------------------------------------
void AddressLineEdit::dropEvent(QDropEvent *e)
{
  KUrl::List uriList = KUrl::List::fromMimeData( e->mimeData() );
  if (!uriList.isEmpty())
  {
    QString ct = text();
    KUrl::List::Iterator it = uriList.begin();
    for (; it != uriList.end(); ++it)
    {
      if (!ct.isEmpty()) ct.append(", ");
      KUrl u(*it);
      if ((*it).protocol() == "mailto")
          ct.append( (*it).path() );
      else
          ct.append( (*it).url() );
    }
    setText(ct);
    setModified( true );
  }
  else {
    if (m_useCompletion)
       m_smartPaste = true;
    QLineEdit::dropEvent(e);
    m_smartPaste = false;
  }
}


QStringList AddressLineEdit::addresses()
{
  QApplication::setOverrideCursor( Qt::waitCursor ); // loading might take a while

  QStringList result;
  QString space(" ");
  QRegExp needQuotes("[^ 0-9A-Za-z\\x0080-\\xFFFF]");
  QString endQuote("\" ");
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
      if (!email.isEmpty()) {
        if (n.isEmpty() || (email.indexOf( '<' ) != -1))
          addr.clear();
        else { /* do we really need quotes around this name ? */
                if (n.indexOf(needQuotes) != -1)
            addr = '"' + n + endQuote;
          else
            addr = n + space;
        }

        if (!addr.isEmpty() && (email.indexOf( '<' ) == -1)
            && (email.indexOf( '>' ) == -1)
            && (email.indexOf( ',' ) == -1))
          addr += '<' + email + '>';
        else
          addr += email;
        addr = addr.trimmed();
        result.append( addr );
      }
    }
  }

  KABC::DistributionListManager manager( addressBook );
  manager.load();
  result += manager.listNames();

  QApplication::restoreOverrideCursor();

  return result;
}

#include "addresslineedit.moc"

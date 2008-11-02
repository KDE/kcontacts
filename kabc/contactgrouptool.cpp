/*
    This file is part of libkabc.
    Copyright (c) 2008 Tobias Koenig <tokoe@kde.org>

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

#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QDebug>

#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

#include "contactgroup.h"

#include "contactgrouptool.h"

using namespace KABC;


class XmlContactGroupWriter : public QXmlStreamWriter
{
    public:
        XmlContactGroupWriter( const ContactGroup &group );

        void write( QIODevice *device );

    private:
        void writeReference( const ContactGroup::Reference& );
        void writeData( const ContactGroup::Data& );

        ContactGroup mGroup;
};

XmlContactGroupWriter::XmlContactGroupWriter( const ContactGroup &group )
    : mGroup( group )
{
    setAutoFormatting( true );
}

void XmlContactGroupWriter::write( QIODevice *device )
{
    setDevice( device );

    writeStartDocument();

    writeStartElement( "contactGroup" );
    writeAttribute( "uid", mGroup.id() );
    writeAttribute( "name", mGroup.name() );

    for ( uint i = 0; i < mGroup.referencesCount(); ++i ) {
        writeReference( mGroup.reference( i ) );
    }

    for ( uint i = 0; i < mGroup.dataCount(); ++i ) {
        writeData( mGroup.data( i ) );
    }

    writeEndElement();

    writeEndDocument();
}

void XmlContactGroupWriter::writeReference( const ContactGroup::Reference &reference )
{
    writeStartElement( "contactReference" );
    writeAttribute( "uid", reference.uid() );
    if ( !reference.preferredEmail().isEmpty() )
        writeAttribute( "preferredEmail", reference.preferredEmail() );

    // TODO: customs

    writeEndElement();
}

void XmlContactGroupWriter::writeData( const ContactGroup::Data &data )
{
    writeStartElement( "contactData" );
    writeAttribute( "name", data.name() );
    writeAttribute( "email", data.email() );

    // TODO: customs

    writeEndElement();
}

class XmlContactGroupReader : public QXmlStreamReader
{
    public:
        XmlContactGroupReader( ContactGroup *group );

        bool read( QIODevice *device );

    private:
        bool readReference();
        bool readData();

        ContactGroup *mGroup;
};

XmlContactGroupReader::XmlContactGroupReader( ContactGroup *group )
    : mGroup( group )
{
  Q_ASSERT_X( group != 0, "XmlContactGroupReader", "Passed an invalid group" );
}

bool XmlContactGroupReader::read( QIODevice *device )
{
  setDevice( device );

  int depth = 0;

  while ( !atEnd() ) {
    readNext();

    if ( isStartElement() ) {
      ++depth;
      if ( depth == 1 ) {
        if ( name() == QLatin1String( "contactGroup" ) ) {
          const QXmlStreamAttributes elementAttributes = attributes();
          const QStringRef uid = elementAttributes.value( "uid" );
          if ( uid.isEmpty() ) {
            raiseError( "ContactGroup is missing a uid" );
            continue;
          }

          const QStringRef name = elementAttributes.value( "name" );
          if ( name.isEmpty() ) {
            raiseError( "ContactGroup is missing a name" );
            continue;
          }

          mGroup->setId( uid.toString() );
          mGroup->setName( name.toString() );
        } else {
          raiseError( "The document does not describe a ContactGroup" );
        }
      } else if ( depth == 2 ) {
        if ( name() == QLatin1String( "contactData" ) ) {
          const QXmlStreamAttributes elementAttributes = attributes();
          const QStringRef email = elementAttributes.value( "email" );
          if ( email.isEmpty() ) {
            raiseError( "ContactData is missing an email address" );
            continue;
          }

          const QStringRef name = elementAttributes.value( "name" );
          if ( name.isEmpty() ) {
            raiseError( "ContactData is missing a name" );
            continue;
          }

          ContactGroup::Data data( name.toString(), email.toString() );
          mGroup->append( data );
        } else if ( name() == QLatin1String( "contactReference" ) ) {
          const QXmlStreamAttributes elementAttributes = attributes();
          const QStringRef uid = elementAttributes.value( "uid" );
          if ( uid.isEmpty() ) {
            raiseError( "ContactReference is missing a uid" );
            continue;
          }

          ContactGroup::Reference reference( uid.toString() );
          mGroup->append( reference );
        } else {
          raiseError( "The document does not describe a ContactGroup" );
        }
      } else {
        raiseError( "The document does not describe a ContactGroup" );
      }
    } else if ( isEndElement() ) {
      --depth;
    }
  }

  return !error();
}

bool ContactGroupTool::convertFromXml( QIODevice *device, ContactGroup &group, QString *errorMessage )
{
    Q_UNUSED( errorMessage )

    XmlContactGroupReader reader( &group );

    bool ok = reader.read( device );

    if ( !ok && errorMessage != 0 )
      *errorMessage = reader.errorString();

    return ok;
}

bool ContactGroupTool::convertToXml( const ContactGroup &group, QIODevice *device, QString *errorMessage )
{
    Q_UNUSED( errorMessage )

    XmlContactGroupWriter writer( group );
    writer.write( device );

    return true;
}

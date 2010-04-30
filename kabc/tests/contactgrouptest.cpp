/*
  This file is part of libkabc.
  Copyright (c) 2008 Kevin Krammer <kevin.krammer@gmx.at>

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

#include <qtest_kde.h>

#include "kabc/contactgroup.h"
#include "kabc/contactgrouptool.h"

#include <QBuffer>

using namespace KABC;

class ContactGroupTest : public QObject
{
  Q_OBJECT

  private Q_SLOTS:
    void contactGroupContactReference();
    void contactGroupContactGroupReference();
    void contactGroupData();
    void contactGroup();
    void testGroupRoundTrip();
    void testGroupListRoundTrip();
};

QTEST_KDEMAIN( ContactGroupTest, NoGUI )

void ContactGroupTest::contactGroupContactReference()
{
  const QLatin1String uid( "MyReferenceId" );
  const QLatin1String preferredEMail( "tokoe@kde.org" );
  const QLatin1String customKey( "MyCustomKey" );
  const QLatin1String customValue( "MyCustomValue" );

  // uid test
  {
    ContactGroup::ContactReference ref( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // uid test
  {
    ContactGroup::ContactReference ref;
    ref.setUid( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // preferredEmail test
  {
    ContactGroup::ContactReference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    QCOMPARE( ref.preferredEmail(), preferredEMail );
  }

  // custom test
  {
    ContactGroup::ContactReference ref( uid );
    ref.insertCustom( customKey, customValue );
    QCOMPARE( ref.custom( customKey ), customValue );
    ref.removeCustom( customKey );
    QCOMPARE( ref.custom( customKey ), QString() );
  }

  // assignment test
  {
    ContactGroup::ContactReference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    ref.insertCustom( customKey, customValue );

    ContactGroup::ContactReference ref2( ref );
    QCOMPARE( ref.uid(), ref2.uid() );
    QCOMPARE( ref.preferredEmail(), ref2.preferredEmail() );
    QCOMPARE( ref.custom( customKey ), ref2.custom( customKey ) );

    QVERIFY( ref == ref2 );
  }

  // const test
  {
    ContactGroup::ContactReference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    ref.insertCustom( customKey, customValue );

    const ContactGroup::ContactReference constRef( ref );
    constRef.uid();
    constRef.preferredEmail();
    constRef.custom( customKey );
  }
}

void ContactGroupTest::contactGroupContactGroupReference()
{
  const QLatin1String uid( "MyReferenceId" );
  const QLatin1String customKey( "MyCustomKey" );
  const QLatin1String customValue( "MyCustomValue" );

  // uid test
  {
    ContactGroup::ContactGroupReference ref( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // uid test
  {
    ContactGroup::ContactGroupReference ref;
    ref.setUid( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // custom test
  {
    ContactGroup::ContactGroupReference ref( uid );
    ref.insertCustom( customKey, customValue );
    QCOMPARE( ref.custom( customKey ), customValue );
    ref.removeCustom( customKey );
    QCOMPARE( ref.custom( customKey ), QString() );
  }

  // assignment test
  {
    ContactGroup::ContactGroupReference ref( uid );
    ref.insertCustom( customKey, customValue );

    ContactGroup::ContactGroupReference ref2( ref );
    QCOMPARE( ref.uid(), ref2.uid() );
    QCOMPARE( ref.custom( customKey ), ref2.custom( customKey ) );

    QVERIFY( ref == ref2 );
  }

  // const test
  {
    ContactGroup::ContactGroupReference ref( uid );
    ref.insertCustom( customKey, customValue );

    const ContactGroup::ContactGroupReference constRef( ref );
    constRef.uid();
    constRef.custom( customKey );
  }
}

void ContactGroupTest::contactGroupData()
{
  const QLatin1String name( "Tobias Koenig" );
  const QLatin1String email( "tokoe@kde.org" );
  const QLatin1String customKey( "MyCustomKey" );
  const QLatin1String customValue( "MyCustomValue" );

  // name/email test
  {
    ContactGroup::Data data( name, email );
    QCOMPARE( data.name(), name );
    QCOMPARE( data.email(), email );
  }

  // name test
  {
    ContactGroup::Data data;
    data.setName( name );
    QCOMPARE( data.name(), name );
  }

  // email test
  {
    ContactGroup::Data data;
    data.setEmail( email );
    QCOMPARE( data.email(), email );
  }

  // custom test
  {
    ContactGroup::Data data( name, email );
    data.insertCustom( customKey, customValue );
    QCOMPARE( data.custom( customKey ), customValue );
    data.removeCustom( customKey );
    QCOMPARE( data.custom( customKey ), QString() );
  }

  // assignment test
  {
    ContactGroup::Data data( name, email );
    data.insertCustom( customKey, customValue );

    ContactGroup::Data data2( data );
    QCOMPARE( data.name(), data2.name() );
    QCOMPARE( data.email(), data2.email() );
    QCOMPARE( data.custom( customKey ), data2.custom( customKey ) );

    QVERIFY( data == data2 );
  }

  // const test
  {
    ContactGroup::Data data( name, email );
    data.insertCustom( customKey, customValue );

    const ContactGroup::Data constData( data );
    constData.name();
    constData.email();
    constData.custom( customKey );
  }
}

void ContactGroupTest::contactGroup()
{
  const QLatin1String groupName( "MyGroupName" );
  const QLatin1String groupId( "MyGroupID" );
  const QLatin1String name( "Tobias Koenig" );
  const QLatin1String email( "tokoe@kde.org" );
  const QLatin1String uid( "MyUid" );

  // name test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.name(), groupName );
  }

  // id test
  {
    ContactGroup group( groupName );
    group.setId( groupId );
    QCOMPARE( group.id(), groupId );
  }

  // contact reference test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.contactReferenceCount(), (unsigned int)0 );

    ContactGroup::ContactReference ref( uid );
    ref.setPreferredEmail( email );

    group.append( ref );
    QCOMPARE( group.contactReferenceCount(), (unsigned int)1 );

    const ContactGroup::ContactReference ref2 = group.contactReference( 0 );
    QCOMPARE( ref, ref2 );

    group.remove( ref );
    QCOMPARE( group.contactReferenceCount(), (unsigned int)0 );
  }

  // contact group reference test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.contactGroupReferenceCount(), (unsigned int)0 );

    ContactGroup::ContactGroupReference ref( uid );

    group.append( ref );
    QCOMPARE( group.contactGroupReferenceCount(), (unsigned int)1 );

    const ContactGroup::ContactGroupReference ref2 = group.contactGroupReference( 0 );
    QCOMPARE( ref, ref2 );

    group.remove( ref );
    QCOMPARE( group.contactGroupReferenceCount(), (unsigned int)0 );
  }

  // data test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.dataCount(), (unsigned int)0 );

    ContactGroup::Data data( name, email );

    group.append( data );
    QCOMPARE( group.dataCount(), (unsigned int)1 );

    const ContactGroup::Data data2 = group.data( 0 );
    QCOMPARE( data, data2 );

    group.remove( data );
    QCOMPARE( group.dataCount(), (unsigned int)0 );
  }

  // mimetype test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.mimeType(), QLatin1String( "application/x-vnd.kde.contactgroup" ) );
  }
}

void ContactGroupTest::testGroupRoundTrip()
{
  // TODO should also test empty group

  ContactGroup group( QLatin1String( "TestGroup" ) );
  group.append( ContactGroup::ContactReference( QLatin1String( "Xggdjetw" ) ) );
  group.append( ContactGroup::ContactGroupReference( QLatin1String( "aaXggdjetw" ) ) );
  group.append( ContactGroup::Data( QLatin1String( "Tobias Koenig" ),
                                    QLatin1String( "tokoe@kde.org" ) ) );
  group.append( ContactGroup::Data( QLatin1String( "Kevin Krammer" ),
                                    QLatin1String( "kevin.krammer@gmx.at" ) ) );

  QBuffer buffer;
  buffer.open( QIODevice::WriteOnly );

  QString errorMessage;
  bool result = ContactGroupTool::convertToXml( group, &buffer, &errorMessage );

  QVERIFY( result );
  QVERIFY( errorMessage.isEmpty() );
  buffer.close();
  QVERIFY( buffer.size() > 0 );
  buffer.open( QIODevice::ReadOnly );

  ContactGroup group2;
  result = ContactGroupTool::convertFromXml( &buffer, group2, &errorMessage );
  QVERIFY( result );
  QVERIFY( errorMessage.isEmpty() );
  QCOMPARE( group, group2 );
}

void ContactGroupTest::testGroupListRoundTrip()
{
  // TODO should also test empty list

  ContactGroup::List list;

  ContactGroup group1( QLatin1String( "TestGroup1" ) );
  group1.append( ContactGroup::ContactReference( QLatin1String( "Xggdjetw" ) ) );
  group1.append( ContactGroup::Data( QLatin1String( "Tobias Koenig" ),
                                     QLatin1String( "tokoe@kde.org" ) ) );
  group1.append( ContactGroup::Data( QLatin1String( "Kevin Krammer" ),
                                     QLatin1String( "kevin.krammer@gmx.at" ) ) );

  list.append( group1 );

  ContactGroup group2( QLatin1String( "TestGroup2" ) );
  group2.append( ContactGroup::ContactReference( QLatin1String( "Xggdjetw" ) ) );
  group2.append( ContactGroup::Data( QLatin1String( "Tobias Koenig" ),
                                     QLatin1String( "tokoe@kde.org" ) ) );
  group2.append( ContactGroup::Data( QLatin1String( "Kevin Krammer" ),
                                     QLatin1String( "kevin.krammer@gmx.at" ) ) );

  list.append( group2 );

  QBuffer buffer;
  buffer.open( QIODevice::WriteOnly );

  QString errorMessage;
  bool result = ContactGroupTool::convertToXml( list, &buffer, &errorMessage );

  QVERIFY( result );
  QVERIFY( errorMessage.isEmpty() );
  buffer.close();
  QVERIFY( buffer.size() > 0 );

  buffer.open( QIODevice::ReadOnly );

  ContactGroup::List list2;
  result = ContactGroupTool::convertFromXml( &buffer, list2, &errorMessage );
  QVERIFY( result );
  QVERIFY( errorMessage.isEmpty() );
  QVERIFY( list2.size() == 2 );
  QCOMPARE( list2[0], group1 );
  QCOMPARE( list2[1], group2 );
}

#include "contactgrouptest.moc"

// kate: space-indent on; indent-width 2; replace-tabs on;

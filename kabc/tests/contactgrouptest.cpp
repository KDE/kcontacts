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
    void contactGroupReference();
    void contactGroupData();
    void contactGroup();
    void subgroup();
    void testGroupRoundTrip();
    void testGroupListRoundTrip();
};

QTEST_KDEMAIN( ContactGroupTest, NoGUI )

void ContactGroupTest::contactGroupReference()
{
  const QString uid( "MyReferenceId" );
  const QString preferredEMail( "tokoe@kde.org" );
  const QString customKey( "MyCustomKey" );
  const QString customValue( "MyCustomValue" );

  // uid test
  {
    ContactGroup::Reference ref( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // uid test
  {
    ContactGroup::Reference ref;
    ref.setUid( uid );
    QCOMPARE( ref.uid(), uid );
  }

  // preferredEmail test
  {
    ContactGroup::Reference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    QCOMPARE( ref.preferredEmail(), preferredEMail );
  }

  // custom test
  {
    ContactGroup::Reference ref( uid );
    ref.insertCustom( customKey, customValue );
    QCOMPARE( ref.custom( customKey ), customValue );
    ref.removeCustom( customKey );
    QCOMPARE( ref.custom( customKey ), QString() );
  }

  // assignment test
  {
    ContactGroup::Reference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    ref.insertCustom( customKey, customValue );

    ContactGroup::Reference ref2( ref );
    QCOMPARE( ref.uid(), ref2.uid() );
    QCOMPARE( ref.preferredEmail(), ref2.preferredEmail() );
    QCOMPARE( ref.custom( customKey ), ref2.custom( customKey ) );

    QVERIFY( ref == ref2 );
  }

  // const test
  {
    ContactGroup::Reference ref( uid );
    ref.setPreferredEmail( preferredEMail );
    ref.insertCustom( customKey, customValue );

    const ContactGroup::Reference constRef( ref );
    constRef.uid();
    constRef.preferredEmail();
    constRef.custom( customKey );
  }
}

void ContactGroupTest::contactGroupData()
{
  const QString name( "Tobias Koenig" );
  const QString email( "tokoe@kde.org" );
  const QString customKey( "MyCustomKey" );
  const QString customValue( "MyCustomValue" );

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
  const QString groupName( "MyGroupName" );
  const QString groupId( "MyGroupID" );
  const QString name( "Tobias Koenig" );
  const QString email( "tokoe@kde.org" );
  const QString uid( "MyUid" );

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

  // reference test
  {
    ContactGroup group( groupName );
    QCOMPARE( group.referencesCount(), (unsigned int)0 );

    ContactGroup::Reference ref( uid );
    ref.setPreferredEmail( email );

    group.append( ref );
    QCOMPARE( group.referencesCount(), (unsigned int)1 );

    const ContactGroup::Reference ref2 = group.reference( 0 );
    QCOMPARE( ref, ref2 );

    group.remove( ref );
    QCOMPARE( group.referencesCount(), (unsigned int)0 );
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

void ContactGroupTest::subgroup()
{
  ContactGroup subGroup( "SubGroup" );
  subGroup.append( ContactGroup::Reference( "334Fds" ) );
  subGroup.append( ContactGroup::Data( "John Doe", "doe@kde.org" ) );

  ContactGroup group( "Toplevel" );
  QCOMPARE( group.referencesCount(), (unsigned int)0 );
  QCOMPARE( group.dataCount(), (unsigned int)0 );
  QCOMPARE( group.subgroupCount(), (unsigned int)0 );

  group.append( ContactGroup::Reference( "Xggdjetw" ) );
  group.append( ContactGroup::Data( "Tobias Koenig", "tokoe@kde.org" ) );
  group.append( ContactGroup::Data( "Kevin Krammer", "kevin.krammer@gmx.at" ) );
  group.append( subGroup );

  QCOMPARE( group.referencesCount(), (unsigned int)1 );
  QCOMPARE( group.dataCount(), (unsigned int)2 );
  QCOMPARE( group.subgroupCount(), (unsigned int)1 );
  QCOMPARE( group.count(), (unsigned int)3 );

  const ContactGroup subGroup2 = group.subgroup( 0 );
  QCOMPARE( subGroup, subGroup2 );

  group.remove( subGroup );
  QCOMPARE( group.subgroupCount(), (unsigned int)0 );
}

void ContactGroupTest::testGroupRoundTrip()
{
  // TODO should also test empty group

  ContactGroup subGroup( "SubGroup" );
  subGroup.append( ContactGroup::Reference( "334Fds" ) );
  subGroup.append( ContactGroup::Data( "John Doe", "doe@kde.org" ) );

  ContactGroup group( "TestGroup" );
  group.append( ContactGroup::Reference( "Xggdjetw" ) );
  group.append( ContactGroup::Data( "Tobias Koenig", "tokoe@kde.org" ) );
  group.append( ContactGroup::Data( "Kevin Krammer", "kevin.krammer@gmx.at" ) );
  group.append( subGroup );

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

  ContactGroup group1( "TestGroup1" );
  group1.append( ContactGroup::Reference( "Xggdjetw" ) );
  group1.append( ContactGroup::Data( "Tobias Koenig", "tokoe@kde.org" ) );
  group1.append( ContactGroup::Data( "Kevin Krammer", "kevin.krammer@gmx.at" ) );

  list.append( group1 );

  ContactGroup group2( "TestGroup2" );
  group2.append( ContactGroup::Reference( "Xggdjetw" ) );
  group2.append( ContactGroup::Data( "Tobias Koenig", "tokoe@kde.org" ) );
  group2.append( ContactGroup::Data( "Kevin Krammer", "kevin.krammer@gmx.at" ) );

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

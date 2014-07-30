/*
  This file is part of libkabc.
  Copyright (c) 2008 Tobias Koenig <tokoe@kde.org>
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

#include "contactgrouptool.h"
#include "contactgroup.h"

#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtCore/QDebug>

#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

using namespace KABC;

class XmlContactGroupWriter : public QXmlStreamWriter
{
public:
    XmlContactGroupWriter();

    void write(const ContactGroup &group, QIODevice *device);
    void write(const QList<ContactGroup> &groupLis, QIODevice *device);

private:
    void writeGroup(const ContactGroup &group);
    void writeContactReference(const ContactGroup::ContactReference &);
    void writeContactGroupReference(const ContactGroup::ContactGroupReference &);
    void writeData(const ContactGroup::Data &);
};

XmlContactGroupWriter::XmlContactGroupWriter()
{
    setAutoFormatting(true);
}

void XmlContactGroupWriter::write(const ContactGroup &group, QIODevice *device)
{
    setDevice(device);

    writeStartDocument();

    writeGroup(group);

    writeEndDocument();
}

void XmlContactGroupWriter::write(const QList<ContactGroup> &groupList, QIODevice *device)
{
    setDevice(device);

    writeStartDocument();

    writeStartElement(QLatin1String("contactGroupList"));

    foreach (const ContactGroup &group, groupList) {
        writeGroup(group);
    }

    writeEndElement();

    writeEndDocument();
}

void XmlContactGroupWriter::writeGroup(const ContactGroup &group)
{
    writeStartElement(QLatin1String("contactGroup"));
    writeAttribute(QLatin1String("uid"), group.id());
    writeAttribute(QLatin1String("name"), group.name());

    for (uint i = 0; i < group.contactReferenceCount(); ++i) {
        writeContactReference(group.contactReference(i));
    }

    for (uint i = 0; i < group.contactGroupReferenceCount(); ++i) {
        writeContactGroupReference(group.contactGroupReference(i));
    }

    for (uint i = 0; i < group.dataCount(); ++i) {
        writeData(group.data(i));
    }

    writeEndElement();
}

void XmlContactGroupWriter::writeContactReference(const ContactGroup::ContactReference &reference)
{
    writeStartElement(QLatin1String("contactReference"));
    writeAttribute(QLatin1String("uid"), reference.uid());
    writeAttribute(QLatin1String("gid"), reference.gid());
    if (!reference.preferredEmail().isEmpty()) {
        writeAttribute(QLatin1String("preferredEmail"), reference.preferredEmail());
    }

    // TODO: customs

    writeEndElement();
}

void XmlContactGroupWriter::writeContactGroupReference(
    const ContactGroup::ContactGroupReference &reference)
{
    writeStartElement(QLatin1String("contactGroupReference"));
    writeAttribute(QLatin1String("uid"), reference.uid());

    // TODO: customs

    writeEndElement();
}

void XmlContactGroupWriter::writeData(const ContactGroup::Data &data)
{
    writeStartElement(QLatin1String("contactData"));
    writeAttribute(QLatin1String("name"), data.name());
    writeAttribute(QLatin1String("email"), data.email());

    // TODO: customs

    writeEndElement();
}

class XmlContactGroupReader : public QXmlStreamReader
{
public:
    XmlContactGroupReader();

    bool read(QIODevice *device, ContactGroup &group);
    bool read(QIODevice *device, QList<ContactGroup> &groupList);

private:
    bool readGroup(ContactGroup &group);
    bool readContactReference(ContactGroup::ContactReference &reference);
    bool readContactGroupReference(ContactGroup::ContactGroupReference &reference);
    bool readData(ContactGroup::Data &data);
};

XmlContactGroupReader::XmlContactGroupReader()
{
}

bool XmlContactGroupReader::read(QIODevice *device, ContactGroup &group)
{
    setDevice(device);

    while (!atEnd()) {
        readNext();
        if (isStartElement()) {
            if (name() == QLatin1String("contactGroup")) {
                return readGroup(group);
            } else {
                raiseError(QLatin1String("The document does not describe a ContactGroup"));
            }
        }
    }

    return error() == NoError;
}

bool XmlContactGroupReader::read(QIODevice *device, QList<ContactGroup> &groupList)
{
    setDevice(device);

    int depth = 0;

    while (!atEnd()) {
        readNext();
        if (isStartElement()) {
            ++depth;
            if (depth == 1) {
                if (name() == QLatin1String("contactGroupList")) {
                    continue;
                } else {
                    raiseError(QLatin1String("The document does not describe a list of ContactGroup"));
                }
            } else if (depth == 2) {
                if (name() == QLatin1String("contactGroup")) {
                    ContactGroup group;
                    if (!readGroup(group)) {
                        return false;
                    }

                    groupList.append(group);
                } else {
                    raiseError(QLatin1String("The document does not describe a list of ContactGroup"));
                }
            }
        }

        if (isEndElement()) {
            --depth;
        }
    }

    return error() == NoError;
}

bool XmlContactGroupReader::readGroup(ContactGroup &group)
{
    const QXmlStreamAttributes elementAttributes = attributes();
    const QStringRef uid = elementAttributes.value(QLatin1String("uid"));
    if (uid.isEmpty()) {
        raiseError(QLatin1String("ContactGroup is missing a uid"));
        return false;
    }

    const QStringRef groupName = elementAttributes.value(QLatin1String("name"));
    if (groupName.isEmpty()) {
        raiseError(QLatin1String("ContactGroup is missing a name"));
        return false;
    }

    group.setId(uid.toString());
    group.setName(groupName.toString());

    while (!atEnd()) {
        readNext();
        if (isStartElement()) {
            if (name() == QLatin1String("contactData")) {
                ContactGroup::Data data;
                if (!readData(data)) {
                    return false;
                }
                group.append(data);
            } else if (name() == QLatin1String("contactReference")) {
                ContactGroup::ContactReference reference;
                if (!readContactReference(reference)) {
                    return false;
                }
                group.append(reference);
            } else if (name() == QLatin1String("contactGroupReference")) {
                ContactGroup::ContactGroupReference reference;
                if (!readContactGroupReference(reference)) {
                    return false;
                }
                group.append(reference);
            } else {
                raiseError(QLatin1String("The document does not describe a ContactGroup"));
            }
        }

        if (isEndElement()) {
            if (name() == QLatin1String("contactGroup")) {
                return true;
            }
        }
    }

    return false;
}

bool XmlContactGroupReader::readData(ContactGroup::Data &data)
{
    const QXmlStreamAttributes elementAttributes = attributes();
    const QStringRef email = elementAttributes.value(QLatin1String("email"));
    if (email.isEmpty()) {
        raiseError(QLatin1String("ContactData is missing an email address"));
        return false;
    }

    const QStringRef name = elementAttributes.value(QLatin1String("name"));

    data.setName(name.toString());
    data.setEmail(email.toString());

    return true;
}

bool XmlContactGroupReader::readContactReference(ContactGroup::ContactReference &reference)
{
    const QXmlStreamAttributes elementAttributes = attributes();
    const QStringRef uid = elementAttributes.value(QLatin1String("uid"));
    const QStringRef gid = elementAttributes.value(QLatin1String("gid"));
    if (uid.isEmpty() && gid.isEmpty()) {
        raiseError(QLatin1String("ContactReference is missing both uid and gid"));
        return false;
    }
    const QStringRef preferredEmail = elementAttributes.value(QLatin1String("preferredEmail"));

    reference.setUid(uid.toString());
    reference.setGid(gid.toString());
    reference.setPreferredEmail(preferredEmail.toString());

    return true;
}

bool XmlContactGroupReader::readContactGroupReference(
    ContactGroup::ContactGroupReference &reference)
{
    const QXmlStreamAttributes elementAttributes = attributes();
    const QStringRef uid = elementAttributes.value(QLatin1String("uid"));
    if (uid.isEmpty()) {
        raiseError(QLatin1String("ContactGroupReference is missing a uid"));
        return false;
    }

    reference.setUid(uid.toString());

    return true;
}

bool ContactGroupTool::convertFromXml(QIODevice *device, ContactGroup &group,
                                      QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    XmlContactGroupReader reader;

    bool ok = reader.read(device, group);

    if (!ok && errorMessage != 0) {
        *errorMessage = reader.errorString();
    }

    return ok;
}

bool ContactGroupTool::convertToXml(const ContactGroup &group, QIODevice *device,
                                    QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    XmlContactGroupWriter writer;
    writer.write(group, device);

    return true;
}

bool ContactGroupTool::convertFromXml(QIODevice *device, QList<ContactGroup> &groupList,
                                      QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    XmlContactGroupReader reader;

    bool ok = reader.read(device, groupList);

    if (!ok && errorMessage != 0) {
        *errorMessage = reader.errorString();
    }

    return ok;
}

bool ContactGroupTool::convertToXml(const QList<ContactGroup> &groupList,
                                    QIODevice *device, QString *errorMessage)
{
    Q_UNUSED(errorMessage);

    XmlContactGroupWriter writer;
    writer.write(groupList, device);

    return true;
}

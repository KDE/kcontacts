/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_CONTACTGROUP_H
#define KCONTACTS_CONTACTGROUP_H

#include <QList>
#include <QMetaType>
#include <QSharedDataPointer>

#include "kcontacts_export.h"

class QString;

namespace KContacts
{
/*!
 * \class KContacts::ContactGroup
 * \inheaderfile KContacts/ContactGroup
 * \inmodule KContacts
 *
 * \brief This class represents a group of contacts.
 *
 * It can contain two types of contacts, either a reference
 * or data.
 * The reference entry is just an unique identifier which
 * identifies the real contact in the system.
 * The data entry contains a name and an email address.
 *
 * \since 4.3
 */
class KCONTACTS_EXPORT ContactGroup
{
public:
    /*!
     * \class KContacts::ContactGroup::ContactReference
     * \inheaderfile KContacts/ContactGroup
     * \inmodule KContacts
     *
     * This class represents a contact reference
     */
    class KCONTACTS_EXPORT ContactReference
    {
    public:
        /*!
         * A list of contact references.
         */
        typedef QList<ContactReference> List;

        /*!
         * Creates an empty contact reference.
         */
        ContactReference();

        /*!
         * Creates a contact reference from an \a other reference.
         */
        ContactReference(const ContactReference &other);

        /*!
         * Creates a contact reference for the given contact \a uid.
         */
        ContactReference(const QString &uid);

        /*!
         * Destroys the contact reference.
         */
        ~ContactReference();

        /*!
         * Sets the contact uid of the contact reference.
         *
         * \a uid identifier of the contact to reference
         *
         * \note That is the Akonadi Item ID of the contact that
         *       is referenced here.
         */
        void setUid(const QString &uid);

        /*!
         * Returns the contact uid of the contact reference.
         *
         * \note That is the Akonadi Item ID of the contact that
         *       is referenced here.
         */
        [[nodiscard]] QString uid() const;

        /*!
         * Sets the contact gid of the contact reference.
         *
         * \a gid globally unique identifier of the contact to reference
         * \since 4.12
         */
        void setGid(const QString &gid);

        /*!
         * Returns the contact GID of the contact reference.
         * \since 4.12
         */
        [[nodiscard]] QString gid() const;

        /*!
         * Sets the preferred email address.
         */
        void setPreferredEmail(const QString &email);

        /*!
         * Returns the preferred email address, or an empty string
         * if no preferred email address is set.
         */
        [[nodiscard]] QString preferredEmail() const;

        /*!
         * Inserts a custom entry.
         * If an entry with the same \a key already exists, it is
         * overwritten.
         *
         * \a key The unique key.
         *
         * \a value The value.
         */
        void insertCustom(const QString &key, const QString &value);

        /*!
         * Removes the custom entry with the given \a key.
         */
        void removeCustom(const QString &key);

        /*!
         * Returns the value for the given \a key, or an empty string
         * if the entry for that key does not exists.
         */
        [[nodiscard]] QString custom(const QString &key) const;

        /*!
         * \internal
         */
        ContactReference &operator=(const ContactReference &other);

        /*!
         * \internal
         */
        [[nodiscard]] bool operator==(const ContactReference &other) const;

    private:
        class ContactReferencePrivate;
        QSharedDataPointer<ContactReferencePrivate> d;
    };

    /*!
     * \class KContacts::ContactGroup::ContactGroupReference
     * \inheaderfile KContacts/ContactGroup
     * \inmodule KContacts
     *
     * \brief This class represents a contact group reference.
     */
    class KCONTACTS_EXPORT ContactGroupReference
    {
    public:
        /*!
         * A list of contact group references.
         */
        typedef QList<ContactGroupReference> List;

        /*!
         * Creates an empty contact group reference.
         */
        ContactGroupReference();

        /*!
         * Creates a contact group reference from an \a other reference.
         */
        ContactGroupReference(const ContactGroupReference &other);

        /*!
         * Creates a contact group reference for the given contact group \a uid.
         */
        ContactGroupReference(const QString &uid);

        /*!
         * Destroys the contact group reference.
         */
        ~ContactGroupReference();

        /*!
         * Sets the contact group uid of the contact group reference.
         */
        void setUid(const QString &uid);

        /*!
         * Returns the contact group uid of the contact group reference.
         */
        QString uid() const;

        /*!
         * Inserts a custom entry.
         * If an entry with the same \a key already exists, it is
         * overwritten.
         *
         * \a key The unique key.
         *
         * \a value The value.
         */
        void insertCustom(const QString &key, const QString &value);

        /*!
         * Removes the custom entry with the given \a key.
         */
        void removeCustom(const QString &key);

        /*!
         * Returns the value for the given \a key, or an empty string
         * if the entry for that key does not exists.
         */
        QString custom(const QString &key) const;

        /*!
         * \internal
         */
        ContactGroupReference &operator=(const ContactGroupReference &other);

        /*!
         * \internal
         */
        bool operator==(const ContactGroupReference &other) const;

    private:
        class ContactGroupReferencePrivate;
        QSharedDataPointer<ContactGroupReferencePrivate> d;
    };

    /*!
     * \class KContacts::ContactGroup::Data
     * \inheaderfile KContacts/ContactGroup
     * \inmodule KContacts
     *
     * This class represents a contact data object
     */
    class KCONTACTS_EXPORT Data
    {
    public:
        /*!
         * A list of contact data.
         */
        typedef QList<Data> List;

        /*!
         * Creates an empty contact data object.
         */
        Data();

        /*!
         * Creates a contact data object from an \a other data object.
         */
        Data(const Data &other);

        /*!
         * Creates a contact data object with the given \a name and \a email address.
         */
        Data(const QString &name, const QString &email);

        /*!
         * Destroys the contact data object.
         */
        ~Data();

        /*!
         * Sets the \a name of the contact data object.
         */
        void setName(const QString &name);

        /*!
         * Returns the name of the contact data object.
         */
        [[nodiscard]] QString name() const;

        /*!
         * Sets the \a email address of the contact data object.
         */
        void setEmail(const QString &email);

        /*!
         * Returns the email address of the contact data object.
         */
        [[nodiscard]] QString email() const;

        /*!
         * Inserts a custom entry.
         * If an entry with the same \a key already exists, it is
         * overwritten.
         *
         * \a key The unique key.
         *
         * \a value The value.
         */
        void insertCustom(const QString &key, const QString &value);

        /*!
         * Removes the custom entry with the given \a key.
         */
        void removeCustom(const QString &key);

        /*!
         * Returns the value for the given \a key, or an empty string
         * if the entry for that key does not exists.
         */
        [[nodiscard]] QString custom(const QString &key) const;

        /*!
         * \internal
         */
        Data &operator=(const Data &other);

        /*!
         * \internal
         */
        [[nodiscard]] bool operator==(const Data &other) const;

    private:
        class DataPrivate;
        QSharedDataPointer<DataPrivate> d;
    };

    /*!
     * A list of contact groups.
     */
    typedef QList<ContactGroup> List;

    /*!
     * Creates an empty contact group.
     */
    ContactGroup();

    /*!
     * Creates a contact group from an \a other group.
     */
    ContactGroup(const ContactGroup &other);

    /*!
     * Creates a contact group with the given name.
     */
    ContactGroup(const QString &name);

    /*!
     * Destroys the contact group.
     */
    ~ContactGroup();

    /*!
     * Sets the unique \a id of the contact group.
     */
    void setId(const QString &id);

    /*!
     * Returns the unique id of the contact group.
     */
    [[nodiscard]] QString id() const;

    /*!
     * Sets the i18n'd \a name of the contact group.
     */
    void setName(const QString &name);

    /*!
     * Returns the i18n'd name of the contact group.
     */
    [[nodiscard]] QString name() const;

    /*!
     * Returns the number of contacts in this group.
     * That includes the contact references and contact data.
     */
    [[nodiscard]] int count() const;

    /*!
     * Returns the number of contact references in this group.
     */
    [[nodiscard]] int contactReferenceCount() const;

    /*!
     * Returns the number of group references in this group.
     */
    [[nodiscard]] int contactGroupReferenceCount() const;

    /*!
     * Returns the number of contact data objects in this group.
     */
    [[nodiscard]] int dataCount() const;

    /*!
     * Returns the contact reference at the given \a index.
     */
    [[nodiscard]] ContactReference &contactReference(int index);

    /*!
     * Returns the contact reference at the given \a index.
     */
    const ContactReference &contactReference(int index) const;

    /*!
     * Returns the contact group reference at the given \a index.
     */
    ContactGroupReference &contactGroupReference(int index);

    /*!
     * Returns the contact group reference at the given \a index.
     */
    const ContactGroupReference &contactGroupReference(int index) const;

    /*!
     * Returns the contact data object at the given \a index.
     */
    Data &data(int index);

    /*!
     * Returns the contact data object at the given \a index.
     */
    const Data &data(int index) const;

    /*!
     * Appends a new contact \a reference to the contact group.
     */
    void append(const ContactReference &reference);

    /*!
     * Appends a new contact group \a reference to the contact group.
     */
    void append(const ContactGroupReference &reference);

    /*!
     * Appends a new contact \a data object to the contact group.
     */
    void append(const Data &data);

    /*!
     * Removes the given contact \a reference from the contact group.
     */
    void remove(const ContactReference &reference);

    /*!
     * Removes the given contact group \a reference from the contact group.
     */
    void remove(const ContactGroupReference &reference);

    /*!
     * Removes the given contact \a data object from the contact group.
     */
    void remove(const Data &data);

    /*!
     * Removes all contact references from the contact group.
     */
    void removeAllContactReferences();

    /*!
     * Removes all contact group references from the contact group.
     */
    void removeAllContactGroupReferences();

    /*!
     * Removes all contact data from the contact group.
     */
    void removeAllContactData();

    /*!
     * \internal
     */
    ContactGroup &operator=(const ContactGroup &other);

    /*!
     * \internal
     */
    [[nodiscard]] bool operator==(const ContactGroup &other) const;

    /*!
     * Returns the MIME type used for Contact Groups
     */
    static QString mimeType();

private:
    class Private;
    QSharedDataPointer<Private> d;
};
}
Q_DECLARE_TYPEINFO(KContacts::ContactGroup::ContactGroupReference, Q_RELOCATABLE_TYPE);
Q_DECLARE_TYPEINFO(KContacts::ContactGroup::ContactReference, Q_RELOCATABLE_TYPE);

#define KCONTACTS_CONTACTGROUP_METATYPE_DEFINED
Q_DECLARE_METATYPE(KContacts::ContactGroup)

#endif

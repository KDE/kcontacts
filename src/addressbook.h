// This file is part of the KContacts framework.
// SPDX-FileCopyrightText: 2023 g10Code GmbH
// SPDX-FileContributor: Carl Schwan <carlschwan@gnupg.com>
// SPDX-License-Identifier: LGPL-2.0-or-later

#ifndef KCONTACTS_ADDRESSBOOK_H
#define KCONTACTS_ADDRESSBOOK_H

#include "kcontacts_export.h"

#include <KContacts/Addressee>
#include <KContacts/Namespace>
#include <QIcon>

namespace KContacts
{

/**
 * @brief An abstract collection of addressee.
 *
 * This class provides a collection of signal making it easy to connect it to a
 * QAbstractListModel.
 *
 * @since 6.0
 */
class KCONTACTS_EXPORT AddressBook : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QList<Addressee> addresses READ addressees NOTIFY addresseesChanged)
    Q_PROPERTY(KContacts::AccessMode accessMode READ accessMode WRITE setAccessMode NOTIFY accessModeChanged)
    Q_PROPERTY(bool isLoading READ isLoading WRITE setIsLoading NOTIFY isLoadingChanged)

public:
    AddressBook(QObject *parent = nullptr);
    ~AddressBook();

    using Ptr = QSharedPointer<AddressBook>;

    /**
     * The list of addressee.
     */
    virtual Addressee::List addressees() const = 0;

    /**
     * The user-visible name for this calendar.
     * @see setName()
     */
    QString name() const;

    /**
     * Set the user-visible name for this calendar.
     * @see name()
     */
    void setName(const QString &name);

    /**
     * This calendar's icon.
     * @see setIcon()
     */
    QIcon icon() const;

    /**
     * Set this calendar's icon.
     * @see icon()
     */
    void setIcon(const QIcon &icon);

    /**
     * This address book's AccessMode, i.e. whether it is writable or read-only.
     * Defaults to ReadWrite.
     * @see setAccessMode()
     */
    AccessMode accessMode() const;

    /**
     * Set this address book's AccessMode, i.e. whether it is writable or read-only.
     * @see accessMode()
     */
    void setAccessMode(const AccessMode mode);

    /**
     * Returns whether this address book is loading.
     * @see setIsLoading()
     */
    bool isLoading() const;

    /**
     * Set whether this address book is loading.
     * @see isLoading()
     */
    void setIsLoading(bool isLoading);

    /**
     * Inserts an Addressee into the addressee book.
     *
     * @note Depending on the implementation this can happen asynchronously.
     *
     * @param addressee is the Addressee to insert.
     * @return true if the Addressee was successfully inserted; false otherwise.
     * @see deleteAddressee()
     */
    virtual bool addAddressee(const Addressee &addressee) = 0;

    /**
     * Remove an Addressee into the address book.
     *
     * @note Depending on the implementation this can happen asynchronously.
     *
     * @param addressee is the Addressee to remove.
     * @return true if the Addressee was successfully removed; false otherwise.
     * @see deleteAddressee()
     */
    virtual bool removeAddressee(const Addressee &addressee) = 0;

    /**
     * Update an Addressee from the address book.
     *
     * @note Depending on the implementation this can happen asynchronously.
     *
     * @param addressee is the Addressee to updated.
     * @return true if the Addressee was successfully updated; false otherwise.
     * @see deleteAddressee()
     */
    virtual bool updateAddressee(const Addressee &addressee) = 0;

Q_SIGNALS:
    void addresseesChanged();
    void nameChanged();
    void iconChanged();
    void accessModeChanged();
    void isLoadingChanged();

    /**
     * This signal is emitted just before addressees are inserted into the address book.
     * The new addressees will be positioned between start and end inclusive.
     *
     * @sa QAbstractItemModel::rowsAboutToBeInserted
     */
    void addresseesAboutToBeInserted(int start, int end);

    /**
     * This signal is emitted after addressees are inserted into the address book.
     * The new addressees will be positioned between start and end inclusive.
     *
     * @sa QAbstractItemModel::rowsInserted
     */
    void addresseesInserted();

    /**
     * This signal is emitted just before addressees are removed into the address book.
     * The addressees that will be removed are those between first and last inclusive
     *
     * @sa QAbstractItemModel::rowsAboutToBeRemoved
     */
    void addresseesAboutToBeRemoved(int start, int end);

    /**
     * This signal is emitted after addressees are removed into the address book.
     * The removed addressees are those between first and last inclusive
     *
     * @sa QAbstractItemModel::rowsRemoved
     */
    void addresseesRemoved();

    /**
     * This signal is emitted just before the address book's internal state has been invalidated.
     * @sa QAbstractItemModel::beginResetModel
     */
    void addresseesAboutToBeReset();

    /**
     * This signal is emitted after the address book's internal state has been invalidated.
     * @sa QAbstractItemModel::beginResetModel
     */
    void addresseesReset();

    /**
     * This signal is emitted whenever the data in an existing addressees changes.
     * The updated addressees are those between start and end inclusive.
     *
     * @sa QAbstractItemModel::dataChanged
     */
    void addresseesChanged(int start, int end);

    /**
     * Emitted when an error occur when adding, removing or updating an addressee.
     */
    void errorOccurred(const QString &errorMessage);

private:
    class Private;
    std::unique_ptr<Private> d;
};

}

#endif // KCONTACTS_ADDRESSBOOK_H
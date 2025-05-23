/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2002 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCONTACTS_SOUND_H
#define KCONTACTS_SOUND_H

#include "kcontacts_export.h"
#include <QSharedDataPointer>
#include <QString>

namespace KContacts
{
/*!
 * \class KContacts::Sound
 * \inheaderfile KContacts/Sound
 * \inmodule KContacts
 *
 * \brief Class that holds a Sound clip for a contact.
 *
 * The sound can be played doing something like this:
 *
 * \code
 *   KTempFile tmp;
 *   if ( sound.isIntern() ) {
 *     tmp.file()->write( sound.data() );
 *     tmp.close();
 *     KAudioPlayer::play( tmp.name() );
 *   } else if( !sound.url().isEmpty() ) {
 *     QString tmpFile;
 *     if ( !KIO::NetAccess::download( QUrl( themeURL.url() ), tmpFile, 0 ) ) {
 *       KMessageBox::error( 0,
 *                           KIO::NetAccess::lastErrorString(),
 *                           i18n( "Failed to download sound file" ),
 *                           KMessageBox::Notify
 *                         );
 *       return;
 *     }
 *     KAudioPlayer::play( tmpFile );
 *   }
 * \endcode
 *
 * Unfortunately, KAudioPlayer::play is ASync, so to delete the temporary file
 * the best you can really do is set a timer.
 */
class KCONTACTS_EXPORT Sound
{
    friend KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &, const Sound &);
    friend KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &, Sound &);

public:
    /*!
     * Creates an empty sound object.
     */
    Sound();

    /*!
     * Creates a sound object for the given url.
     *
     * \a url A url that describes the position of the sound file.
     */
    Sound(const QString &url);

    /*!
     * Creates a sound object for the given data.
     *
     * \a data The raw data of the sound.
     */
    Sound(const QByteArray &data);

    Sound(const Sound &other);

    ~Sound();

    /*!
     */
    typedef QList<Sound> List;

    /*!
     * Assignment operator.
     *
     * \a other The sound object to assign to \c this
     */
    Sound &operator=(const Sound &other);

    /*!
     * Equality operator.
     *
     * \a other The object to compare with
     *
     * Returns \c true if the two objects are equal, otherwise \c false
     */
    Q_REQUIRED_RESULT bool operator==(const Sound &other) const;

    /*!
     * Not-Equal operator.
     *
     * \a other The object to compare with
     *
     * Returns \c true if the two objects are not equal, otherwise \c false
     */
    Q_REQUIRED_RESULT bool operator!=(const Sound &other) const;

    /*!
     * Sets a URL for the location of the sound file. When using this
     * function, isIntern() will return \c false until you use
     * setData().
     *
     * \a url The location URL of the sound file.
     */
    void setUrl(const QString &url);

    /*!
     * Returns true, if the sound object is empty.
     */
    Q_REQUIRED_RESULT bool isEmpty() const;

    /*!
     * Sets the raw data of the sound. When using this function,
     * isIntern() will return 'true' until you use setUrl().
     *
     * \a data The raw data of the sound.
     */
    void setData(const QByteArray &data);

    /*!
     * Returns whether the sound is described by a URL (extern) or
     * by the raw data (intern).
     *
     * When this method returns \c true you can use data() to
     * get the raw data. Otherwise you can request the URL of this
     * sound by url() and load the raw data from that location.
     */
    Q_REQUIRED_RESULT bool isIntern() const;

    /*!
     * Returns the location URL of this sound.
     */
    Q_REQUIRED_RESULT QString url() const;

    /*!
     * Returns the raw data of this sound.
     */
    Q_REQUIRED_RESULT QByteArray data() const;

    /*!
     * Returns string representation of the sound.
     */
    Q_REQUIRED_RESULT QString toString() const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

/*!
 * \relates KContacts::Sound
 *
 * Serializes the \a sound object into the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator<<(QDataStream &stream, const Sound &sound);

/*!
 * \relates KContacts::Sound
 *
 * Initializes the \a sound object from the \a stream.
 */
KCONTACTS_EXPORT QDataStream &operator>>(QDataStream &stream, Sound &sound);
}
Q_DECLARE_TYPEINFO(KContacts::Sound, Q_RELOCATABLE_TYPE);
#endif

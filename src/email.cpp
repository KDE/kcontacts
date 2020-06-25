/*
    This file is part of the KContacts framework.
    SPDX-FileCopyrightText: 2015-2019 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "email.h"

#include <QMap>
#include <QStringList>
#include <QDataStream>

using namespace KContacts;

class Q_DECL_HIDDEN Email::Private : public QSharedData
{
public:
    Private()
    {
    }

    Private(const Private &other)
        : QSharedData(other)
    {
        parameters = other.parameters;
        mail = other.mail;
    }

    QMap<QString, QStringList> parameters;
    QString mail;
};

Email::Email()
    : d(new Private)
{
}

Email::Email(const QString &mail)
    : d(new Private)
{
    d->mail = mail;
}

Email::Email(const Email &other)
    : d(other.d)
{
}

Email::~Email()
{
}

QMap<QString, QStringList> Email::parameters() const
{
    return d->parameters;
}

bool Email::operator==(const Email &other) const
{
    return (d->parameters == other.parameters()) && (d->mail == other.mail());
}

bool Email::operator!=(const Email &other) const
{
    return !(other == *this);
}

Email &Email::operator=(const Email &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

QString Email::toString() const
{
    QString str = QLatin1String("Email {\n");
    str += QStringLiteral("    mail: %1\n").arg(d->mail);
    if (!d->parameters.isEmpty()) {
        QString param;
        QMap<QString, QStringList>::const_iterator it = d->parameters.constBegin();
        const QMap<QString, QStringList>::const_iterator end = d->parameters.constEnd();
        while (it != end) {
            param += QStringLiteral("%1 %2").arg(it.key(), it.value().join(QLatin1Char(',')));
            ++it;
        }
        str += QStringLiteral("    parameters: %1\n").arg(param);
    }
    str += QLatin1String("}\n");
    return str;
}

void Email::setParameters(const QMap<QString, QStringList> &params)
{
    d->parameters = params;
}

void Email::setEmail(const QString &mail)
{
    d->mail = mail;
}

QString Email::mail() const
{
    return d->mail;
}

bool Email::isValid() const
{
    return !d->mail.isEmpty();
}

struct email_type_name
{
    const char* name;
    Email::Type type;
};

static const email_type_name email_type_names[] = {
    { "HOME", Email::Home },
    { "WORK", Email::Work },
    { "OTHER", Email::Other }
};

Email::Type KContacts::Email::type() const
{
    const auto it = d->parameters.constFind(QLatin1String("type"));
    if (it == d->parameters.end()) {
        return Unknown;
    }

    Type type = Unknown;
    for (const auto &s : it.value()) {
        const auto it = std::find_if(std::begin(email_type_names), std::end(email_type_names), [s](const email_type_name &t) {
            return QLatin1String(t.name) == s;
        });
        if (it != std::end(email_type_names)) {
            type |= (*it).type;
        }
    }
    return type;
}

void Email::setType(Type type)
{
    const auto oldType = this->type();

    auto types = d->parameters.value(QLatin1String("type"));
    for (const auto &t : email_type_names) {
        if (((type ^ oldType) & t.type) == 0) {
            continue; // no change
        }

        if (type & t.type) {
            types.push_back(QLatin1String(t.name));
        } else {
            types.removeAll(QLatin1String(t.name));
        }
    }

    d->parameters.insert(QLatin1String("type"), types);
}

bool Email::isPreferred() const
{
    auto it = d->parameters.constFind(QLatin1String("pref"));
    if (it != d->parameters.end() && !it.value().isEmpty()) {
        return it.value().at(0) == QLatin1Char('1');
    }

    it = d->parameters.constFind(QLatin1String("type"));
    if (it != d->parameters.end()) {
        return it.value().contains(QLatin1String("PREF"), Qt::CaseInsensitive);
    }

    return false;
}

void Email::setPreferred(bool preferred)
{
    if (preferred == isPreferred()) {
        return;
    }

    auto types = d->parameters.value(QLatin1String("type"));
    if (!preferred) {
        d->parameters.remove(QLatin1String("pref"));
        types.removeAll(QLatin1String("PREF"));
    } else {
        types.push_back(QLatin1String("PREF"));
    }
    d->parameters.insert(QLatin1String("type"), types);
}

QDataStream &KContacts::operator<<(QDataStream &s, const Email &email)
{
    return s << email.d->parameters << email.d->mail;
}

QDataStream &KContacts::operator>>(QDataStream &s, Email &email)
{
    s >> email.d->parameters >> email.d->mail;
    return s;
}

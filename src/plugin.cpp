/*
    This file is part of the KContacts framework..
    Copyright (c) 2002 Tobias Koenig <tokoe@kde.org>

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

#include "plugin.h"

using namespace KContacts;

class Plugin::Private
{
public:
    QString mType;
    QString mNameLabel;
    QString mDescriptionLabel;
};

Plugin::Plugin()
    : d(new Private)
{
}

Plugin::~Plugin()
{
    delete d;
}

void Plugin::setType(const QString &type)
{
    d->mType = type;
}

QString Plugin::type() const
{
    return d->mType;
}

void Plugin::setNameLabel(const QString &label)
{
    d->mNameLabel = label;
}

QString Plugin::nameLabel() const
{
    return d->mNameLabel;
}

void Plugin::setDescriptionLabel(const QString &label)
{
    d->mDescriptionLabel = label;
}

QString Plugin::descriptionLabel() const
{
    return d->mDescriptionLabel;
}

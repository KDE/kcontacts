/*
    This file is part of libvcard.
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
    the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <VCardGeoValue.h>

#include <VCardValue.h>

#include <kdebug.h>

using namespace VCARD;

GeoValue::GeoValue()
	:	Value()
{
}

GeoValue::GeoValue(const GeoValue & x)
	:	Value(x), latitude_(x.latitude_), longitude_(x.longitude_)
{
}

GeoValue::GeoValue(const QCString & s)
	:	Value(s)
{
}

	GeoValue &
GeoValue::operator = (GeoValue & x)
{
	if (*this == x) return *this;

  latitude_ = x.latitude_;
  longitude_ = x.longitude_;

	Value::operator = (x);
	return *this;
}

	GeoValue &
GeoValue::operator = (const QCString & s)
{
	Value::operator = (s);
	return *this;
}

	bool
GeoValue::operator == (GeoValue & x)
{
	x.parse();

  if ( latitude_ != x.latitude_ ) return false;
  if ( longitude_ != x.longitude_ ) return false;

	return true;
}

GeoValue::~GeoValue()
{
}

	GeoValue *
GeoValue::clone()
{
	return new GeoValue( *this );
}

	void
GeoValue::_parse()
{
  int semiColon = strRep_.find( ";" );

  if ( semiColon == -1 ) // invalid
    return;

  latitude_ = strRep_.left( semiColon ).toFloat();
  longitude_ = strRep_.mid( semiColon + 1, strRep_.length() - semiColon ).toFloat();
}

	void
GeoValue::_assemble()
{
  strRep_.sprintf( "%.6f;%.6f", latitude_, longitude_ );
}

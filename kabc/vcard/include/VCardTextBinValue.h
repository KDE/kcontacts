/*
	libvcard - vCard parsing library for vCard version 3.0
	
	Copyright (C) 1999 Rik Hemsley rik@kde.org
	
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef  TEXTBINVALUE_H
#define  TEXTBINVALUE_H

#include <qcstring.h>

#include <VCardValue.h>

namespace VCARD
{

class TextBinValue : public Value
{
	
#include "TextBinValue-generated.h"

  TextBinValue *clone();

  bool isBinary() { parse(); return mIsBinary_; }
  QByteArray data() { parse(); return mData_; }
  QString url() { parse(); return mUrl_; }

  void setData( const QByteArray &data )
  {
    mData_ = data;
    mIsBinary_ = true;
    assembled_ = false;
  }

  void setUrl( const QString &url )
  {
    mUrl_ = url;
    mIsBinary_ = false;
    assembled_ = false;
  }

	private:
    int mIsBinary_;
    QByteArray mData_;
    QString mUrl_;
};

}

#endif

#! /usr/bin/env bash
cd scripts && perl ./makeaddressee && cd ..
$XGETTEXT *.cpp -o $podir/kabc.pot
rm -f addressee.cpp addressee.h field.cpp

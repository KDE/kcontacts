#!/bin/bash -eu
#
# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-FileCopyrightText: 2026 Carl Schwan <carl@carlschwan.eu>
# SPDX-License-Identifier: LGPL-2.0-or-later

apt-get update
apt-get install -y ninja-build gperf gettext iso-codes zip
pip3 install cmake

git clone --depth 1 --branch dev https://code.qt.io/qt/qtbase.git "$SRC/qtbase"
for project in extra-cmake-modules kcodecs kconfig ki18n; do
    git clone --depth 1 --branch master "https://invent.kde.org/frameworks/$project.git" "$SRC/$project"
done

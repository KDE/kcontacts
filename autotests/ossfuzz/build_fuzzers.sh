#!/bin/bash -eu
#
# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: LGPL-2.0-or-later

export PATH="$WORK/bin:$PATH"
if [[ $FUZZING_ENGINE == "afl" ]]; then
    export LDFLAGS="-fuse-ld=lld"
fi

cd $SRC/extra-cmake-modules
cmake . -G Ninja \
    -DBUILD_TESTING=OFF \
    -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

cd $SRC/qtbase
./configure -platform linux-clang-libc++ -prefix $WORK -static -opensource -confirm-license -debug \
    -qt-pcre -qt-zlib -no-opengl \
    -no-glib -no-icu -no-feature-sql -no-feature-network -no-feature-dbus -no-feature-printsupport -no-feature-widgets
ninja install -j$(nproc)

for project in kcodecs kconfig ki18n kcontacts; do
    project_options=()
    case "$project" in
        kconfig) project_options=(-DKCONFIG_USE_GUI=OFF -DKCONFIG_USE_QML=OFF -DUSE_DBUS=OFF) ;;
        ki18n) project_options=(-DBUILD_WITH_QML=OFF -DKI18N_EMBEDDED_ISO_CODES_CACHE=ON) ;;
        kcontacts) project_options=(-DUSE_QML=OFF -DBUILD_FUZZERS=ON) ;;
    esac
    cmake -S "$SRC/$project" -B "$WORK/$project-build" -G Ninja \
        -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="$WORK" \
        -DKF_SKIP_PO_PROCESSING=ON \
        "${project_options[@]}"
    cmake --build "$WORK/$project-build" --target install -j"$(nproc)"
done

# Copy the fuzzer
cp "$WORK/kcontacts-build/bin/fuzzers/kcontacts_fuzzer" "$OUT/kcontacts_fuzzer"
# Create a seed corpus
find "$SRC/kcontacts/autotests" -name '*.vcf' -print | zip -jq "$OUT/kcontacts_fuzzer_seed_corpus.zip" -@
# Copy the dictionary
cp "$SRC/kcontacts/autotests/ossfuzz/kcontacts_fuzzer.dict" "$OUT/kcontacts_fuzzer.dict"

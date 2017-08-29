#!/usr/bin/env bash

set -e

if [ -e lib/libsqlite3.a ] && [ -e include/sqlite3.h ]; then
    echo sqlite3 already complied, skipping >&2
    exit 0
fi

mkdir -p include lib
curl https://www.sqlite.org/src/tarball/sqlite.tar.gz | tar xz
pushd sqlite
# TODO: remove unused features and link against libmusl
./configure --disable-tcl
make
cp .libs/libsqlite3.a ../lib
cp sqlite3.h ../include
popd
rm -rf sqlite

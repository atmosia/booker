#!/usr/bin/env bash

set -e

curl https://www.sqlite.org/src/tarball/sqlite.tar.gz | tar xz
pushd sqlite
# TODO: remove unused features and link against libmusl
./configure --disable-tcl
make
cp .libs/libsqlite3.a sqlite3.h ..
popd
rm -rf sqlite

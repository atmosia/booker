# booker [![Build Status](https://travis-ci.org/atmosia/booker.svg?branch=master)](https://travis-ci.org/atmosia/booker) [![Coverage Status](https://coveralls.io/repos/github/atmosia/booker/badge.svg?branch=master)](https://coveralls.io/github/atmosia/booker?branch=master)

Open source POS software

# Building

Each project should have a `Makefile` in its build directory, all of which
use `config.mk` in the root directory for common build options. If a static
version of `libsqlite3` is not present on your system then your can run
`build_static_sqlite.sh` to fetch and build one.

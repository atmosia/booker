# booker [![Build Status](https://travis-ci.org/atmosia/booker.svg?branch=master)](https://travis-ci.org/atmosia/booker) [![Coverage Status](https://coveralls.io/repos/github/atmosia/booker/badge.svg?branch=master)](https://coveralls.io/github/atmosia/booker?branch=master)

Open source POS software

# Building

tl;dr Run `make test` in the checkout.

There is a central `Makefile` which can be used to build the whole project.
Each project should have a `Makefile` in its build directory, all of which
use `config.mk` in the root directory for common build options. If a static
version of `libsqlite3` is not present on your system then your can run
`build_static_sqlite.sh` to fetch and build one.

# Contributing

We are attempting to adhere to the
[suckless philosophy](http://suckless.org/philosophy)
and
[coding style](http://suckless.org/coding_style).
You can normally find one of us on `#booker` on `irc.freenode.net`.

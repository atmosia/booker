# booker version
VERSION = 0.1
APP_PREFIX = booker

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 \
	   -DVERSION=\"${VERSION}\"
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os \
	 ${CPPFLAGS} -g -Wextra
LDFLAGS = -ldl

# compiler
CC = gcc

-include ../local.mk

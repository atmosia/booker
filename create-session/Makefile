# booker-create-session: start a session for booker

APP = create-session

SRC = main.c
EXTERN_OBJ = "../common/db.o" "../common/eprintf.o" "../common/path.o" \
	     "../common/session.o"

include ../config.mk
include ../default.mk

LDFLAGS += -lsqlite3

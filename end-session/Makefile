# booker-end-session: end a session for booker

APP = end-session

SRC = main.c
EXTERN_OBJ = "../common/db.o" "../common/eprintf.o" "../common/path.o" \
             "../common/session.o"

include ../config.mk
include ../default.mk

CPPFLAGS += -I../include
LDFLAGS += -L../lib -lsqlite3 -lpthread -ldl

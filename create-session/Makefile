# booker-create-session: start a session for booker

APP = create-session

SRC = main.c
EXTERN_OBJ = "../common/session.o" "../common/db.o" "../common/path.o"

include ../config.mk
include ../default.mk

CPPFLAGS += -I../include
LDFLAGS += -L../lib -lsqlite3 -lpthread -ldl

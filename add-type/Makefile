# booker-add-type: add a new product type to booker

APP = add-type

SRC = main.c
EXTERN_OBJ = "../common/db.o" "../common/eprintf.o" "../common/path.o" \
	     "../common/session.o" "../common/type.o"

include ../config.mk
include ../default.mk

CPPFLAGS += -I../include
LDFLAGS += -L../lib -lsqlite3 -lpthread

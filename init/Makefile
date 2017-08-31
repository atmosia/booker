# booker-init: initialize booker

APP = init

SRC = main.c
EXTERN_OBJ = "../common/db.o" "../common/path.o"

include ../config.mk
include ../default.mk

CPPFLAGS += -I../include
LDFLAGS += -L../lib -lsqlite3 -lpthread -ldl

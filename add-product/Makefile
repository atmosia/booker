# booker-add-product: adds a product to the database

APP = add-product

SRC = main.c
EXTERN_OBJ = "../common/eprintf.o"

include ../config.mk
include ../default.mk

CPPFLAGS += -I../include
LDFLAGS += -L../lib -lsqlite3 -lpthread

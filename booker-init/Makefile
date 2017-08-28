# booker-init: initialize booker

include ../config.mk

LDFLAGS += -L.. -lsqlite3

SRC = init-main.c
OBJ = ${SRC:.c=.o}
TESTS = $(wildcard t/*.pl)

all: options booker-init

options:
	@echo sis build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "COV      = ${COV}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

booker-init: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

test: all
	@echo TEST booker-init
	@for test in ${TESTS}; do \
		$$test; \
	done
	@${COV} ${SRC}

clean:
	@echo cleaning
	@rm -f booker-init ${OBJ} *.gc{ov,da,no}

.PHONY: all options test clean

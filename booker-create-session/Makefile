# booker-create-session: start a new booker session

include ../config.mk

CPPFLAGS += -I..
LDFLAGS += -L.. -lsqlite3 -lpthread -ldl

SRC = create-session-main.c
OBJ = ${SRC:.c=.o}
TESTS = $(wildcard t/*.pl)

all: options booker-create-session

options:
	@echo booker-create-session build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"
	@echo "COV      = ${COV}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

booker-create-session: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

test: all
	@echo TEST booker-create-session
	@for test in ${TESTS}; do \
		$$test; \
	done
	@${COV} ${SRC}

clean:
	@echo cleaning
	@rm -f booker-create-session ${OBJ} *.gc??

.PHONY: all options test clean

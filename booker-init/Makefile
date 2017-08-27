# booker-init: initialize booker

include ../config.mk

SRC = init-main.c
OBJ = ${SRC:.c=.o}

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
	@echo RUN booker-init
	@./booker-init
	@echo TEST booker-init
	@${COV} ${SRC}

clean:
	@echo cleaning
	@rm -f booker-init ${OBJ}

.PHONY: all options clean

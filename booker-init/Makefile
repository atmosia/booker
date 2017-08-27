# booker-init: initialize booker

include ../config.mk

INIT_SRC = init-main.c
INIT_OBJ = ${INIT_SRC:.c=.o}

OBJ = ${INIT_OBJ}

all: options booker-init

options:
	@echo sis build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

booker-init: ${INIT_OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${INIT_OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f booker-init ${OBJ}

.PHONY: all options clean

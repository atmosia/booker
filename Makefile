TARGETS := all test clean
DIRS := $(wildcard */.)

${TARGETS}: ${DIRS}

${DIRS}:
	$(MAKE) -C $@ ${MAKECMDGOALS}

.PHONY: ${TARGETS} ${DIRS}

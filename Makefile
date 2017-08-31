TARGETS := all test clean
PROJECTS := init create-session common list-products

${TARGETS}: ${PROJECTS}

${PROJECTS}:
	$(MAKE) -C $@ ${MAKECMDGOALS}

.PHONY: ${TARGETS} ${PROJECTS}

TARGETS := all test clean
PROJECTS := init create-session

${TARGETS}: ${PROJECTS}

${PROJECTS}:
	$(MAKE) -C $@ ${MAKECMDGOALS}

.PHONY: ${TARGETS} ${PROJECTS}

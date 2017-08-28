all:
	$(MAKE) -C booker-init
	$(MAKE) -C booker-create-session

test:
	$(MAKE) -C booker-init test
	$(MAKE) -C booker-create-session test

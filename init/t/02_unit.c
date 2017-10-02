#include <stdio.h>

#include "../init.c"

#define TEST_INIT \
	int _fail_count = 0
#define TASSERT(name, check) \
	if (!(check)) { \
		printf("test %s failed\n", name); \
		_fail_count++; \
	}
#define TEST_RET return _fail_count

#define SUITE_INIT \
	int _fail_count = 0
#define RUN_TESTS(c) _fail_count += c
#define SUITE_END return _fail_count;

int
test_config(void)
{
	TEST_INIT;
	TASSERT("invalid config path", !create_config("/no/such/dir"));
	TEST_RET;
}

int
test_cp(void)
{
	TEST_INIT;
	TASSERT("invalid cp source", !cp("/no/such/file", "unused"));
	TASSERT("invalid cp dest",
			!cp("resources/config.ini", "/no/such/file"));
	TEST_RET;
}

int
main(void)
{
	SUITE_INIT;
	RUN_TESTS(test_cp());
	RUN_TESTS(test_config());
	SUITE_END;
}

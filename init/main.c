#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/arg.h"
#include "../common/db.h"
#include "../common/path.h"
#include "../common/util.h"

extern void init(const char *path);

/* prints the usage information */
static void
usage(void)
{
	printf("usage: %s [-h] [-d DIR]\n", argv0);
}

int
main(int argc, char **argv)
{
	char *path;

	path = NULL;
	ARGBEGIN {
	COMMON_ARGS;
	} ARGEND;

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);
	init(path);
	return 0;
}

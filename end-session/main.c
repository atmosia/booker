#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/arg.h"
#include "../common/db.h"
#include "../common/path.h"
#include "../common/session.h"
#include "../common/util.h"

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

	sqlite3 *db = default_db(path);
	if (!valid_session(db))
		eprintf("session not opened\n");

	destroy_session(db);
	return 0;
}

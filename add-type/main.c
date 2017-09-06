#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/arg.h"
#include "../common/db.h"
#include "../common/path.h"
#include "../common/product.h"
#include "../common/session.h"
#include "../common/util.h"

static void
usage(void)
{
	printf("usage: %s [-h] [-d DIR] TYPE\n", argv0);
}

int
main(int argc, char **argv)
{
	char *path = NULL;
	const char *type;

	ARGBEGIN {
	case 'd':
		path = EARGF(usage());
		if (!*path)
			eprintf("path was emtpy\n");
		break;
	case 'h':
		usage();
		exit(0);
	} ARGEND;

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);
	type = *argv;

	sqlite3 *db = default_db(path);
	if (!valid_session(db)) {
		eprintf("not in a session\n");
	}
	add_type(db, type);

	return 0;
}

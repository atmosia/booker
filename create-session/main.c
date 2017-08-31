#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/db.h"
#include "../common/path.h"
#include "../common/session.h"

int
main(int argc, char **argv)
{
	int i;
	char *path;

	path = NULL;
	for (i = 1; i < argc; i++) {
		// TODO: parse -u option to read in users
		if (!strncmp(argv[i], "-d", 2)) {
			i++;
			if (i >= argc) {
				fprintf(stderr, "no argument for -d\n");
				exit(1);
			}
			path = argv[i];
		}
	}

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);

	sqlite3 *db = default_db(path);
	if (valid_session(db)) {
		fprintf(stderr, "session already open\n");
		exit(1);
	}

	// TODO: open session
	return 0;
}

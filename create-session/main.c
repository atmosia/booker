#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/arg.h"
#include "../common/db.h"
#include "../common/path.h"
#include "../common/session.h"
#include "../common/util.h"

#define MAX_USERS 10

static void
usage(void)
{
	printf("usage: %s [-h] [-d DIR] -u USER\n", argv0);
}

int
main(int argc, char **argv)
{
	char *path, *users[MAX_USERS];
	unsigned char i, user_count;
	unsigned long session_id;

	path = NULL;
	user_count = 0;
	ARGBEGIN {
	case 'd':
		path = EARGF(usage());
		if (!*path)
			eprintf("path was emtpy\n");
		break;
	case 'u':
		if (user_count >= MAX_USERS)
			eprintf("max users exceeded (%d max)\n", MAX_USERS);
		users[user_count++] = EARGF(usage());
		break;
	case 'h':
		usage();
		exit(0);
		break;
	} ARGEND;

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);

	sqlite3 *db = default_db(path);
	if (valid_session(db))
		eprintf("session already open\n");

	session_id = create_session(db);
	for (i = 0; i < MAX_USERS; i++)
		add_user_to_session(db, session_id, users[i]);
	return 0;
}

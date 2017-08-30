#include <stdio.h>

#include "db.h"
#include "path.h"

sqlite3*
default_db(const char *base)
{
	int rc;
	sqlite3 *db;

	rc = sqlite3_open(db_path(base), &db);
	if (rc) {
		fprintf(stderr, "failed to open db: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	return db;
}

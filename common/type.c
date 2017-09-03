#include <assert.h>

#include "util.h"
#include "product.h"

void
add_type(sqlite3 *db, const char *type)
{
	int rc;
	sqlite3_stmt *stmt;
	const char *unused;

	rc = sqlite3_prepare_v2(db,
			"INSERT INTO type(name) VALUES (?)",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	sqlite3_bind_text(stmt, 1, type, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_CONSTRAINT)
		eprintf("%s: type %s already exists", argv0, type);
	assert(rc == SQLITE_DONE);
	assert(sqlite3_finalize(stmt) == SQLITE_OK);
}

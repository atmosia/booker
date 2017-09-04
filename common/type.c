#include <assert.h>

#include "product.h"
#include "util.h"

void
add_type(sqlite3 *db, const char *type)
{
	int rc;
	sqlite3_stmt *stmt;
	const char *unused;

	if (type_exists(db, type))
		eprintf("%s already exists\n", type);

	rc = sqlite3_prepare_v2(db,
			"INSERT INTO type_log(name, added) "
				"VALUES (?, datetime('now'))",
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

int
type_exists(sqlite3 *db, const char *type)
{
	int rc, count;
	sqlite3_stmt *stmt;
	const char *unused;

	rc = sqlite3_prepare_v2(db,
			"SELECT COUNT(*) FROM type WHERE name=?",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	sqlite3_bind_text(stmt, 1, type, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	assert(rc == SQLITE_ROW);
	count = sqlite3_column_int(stmt, 1);
	assert(sqlite3_finalize(stmt) == SQLITE_OK);
	return count == 1;
}

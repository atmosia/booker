#include <assert.h>

#include "session.h"

void
add_user_to_session(sqlite3 *db, unsigned long session_id, const char *user)
{
}

unsigned long
create_session(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	const char *unused;
	int rc;

	db_exec(db, "INSERT INTO session(start) VALUES(datetime('now'))");

	rc = sqlite3_prepare_v2(db,
			"SELECT last_insert_rowid()",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	rc = sqlite3_step(stmt);
	assert(rc == SQLITE_ROW);

	return sqlite3_column_int64(stmt, 0) > 0;
}

int
valid_session(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	const char *unused;
	int rc;

	rc = sqlite3_prepare_v2(db,
			"SELECT COUNT(*) FROM session WHERE end IS NULL",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	rc = sqlite3_step(stmt);
	assert(rc == SQLITE_ROW);

	return sqlite3_column_int64(stmt, 0) > 0;
}

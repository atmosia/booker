#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "session.h"

void
add_user_to_session(sqlite3 *db, unsigned long session_id, const char *user)
{
	sqlite3_stmt *stmt;
	const char *unused;
	int rc;

	rc = sqlite3_prepare_v2(db,
			"INSERT INTO session_user VALUES(?1, ?2)",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	sqlite3_bind_int64(stmt, 1, session_id);
	sqlite3_bind_text(stmt, 2, user, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_CONSTRAINT) {
		fprintf(stderr, "skipping duplicate session user: %s\n", user);
		assert(sqlite3_finalize(stmt) == SQLITE_CONSTRAINT);
	} else {
		assert(rc == SQLITE_DONE);
		assert(sqlite3_finalize(stmt) == SQLITE_OK);
	}
}

unsigned long
create_session(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	const char *unused;
	long ret;
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

	ret = sqlite3_column_int64(stmt, 0);
	assert(sqlite3_finalize(stmt) == SQLITE_OK);
	return ret;
}

unsigned long
current_sesion(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	const char *unused;
	long ret;
	int rc;

	rc = sqlite3_prepare_v2(db,
			"SELECT rowid FROM session WHERE end IS NULL",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	rc = sqlite3_step(stmt);
	assert(rc == SQLITE_ROW);

	ret = sqlite3_column_int64(stmt, 0);
	assert(sqlite3_finalize(stmt) == SQLITE_OK);
	return ret;
}
void
destroy_session(sqlite3 *db)
{
	db_exec(db,
		"UPDATE session SET end=datetime('now') WHERE end IS NULL");
}
int
valid_session(sqlite3 *db)
{
	sqlite3_stmt *stmt;
	const char *unused;
	int rc, ret;

	rc = sqlite3_prepare_v2(db,
			"SELECT COUNT(*) FROM session WHERE end IS NULL",
			-1,
			&stmt,
			&unused);
	assert(rc == SQLITE_OK);
	assert(*unused == '\0');

	rc = sqlite3_step(stmt);
	assert(rc == SQLITE_ROW);

	ret = sqlite3_column_int64(stmt, 0) > 0;
	assert(sqlite3_finalize(stmt) == SQLITE_OK);
	return ret;
}

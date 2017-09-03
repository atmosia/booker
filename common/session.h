#include "db.h"

void add_user_to_session(sqlite3 *db, unsigned long session_id,
		const char *user);
unsigned long create_session(sqlite3 *db);
void destroy_session(sqlite3 *db);
int valid_session(sqlite3 *db);

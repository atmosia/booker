#include <sqlite3.h>

sqlite3 *default_db(const char *base);
void db_exec(sqlite3 *db, const char *stmt);

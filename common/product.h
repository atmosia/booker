#include "db.h"

void add_type(sqlite3 *db, const char *type);
int type_exists(sqlite3 *db, const char *type);

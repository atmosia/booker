#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "path.h"

static const char *CONFIG_PATH = "/config.ini";
static const char *DB_PATH = "/db.sqlite3";
static const char *PATH_SUFFIX = "/.booker";

/* test if a file exists */
int
exists(const char *path)
{
	return access(path, F_OK) > -1;
}

/* allocates memory for PATH, and then stores the default path in
 * PATH
 */
void
default_path(char **path)
{
	char *home;
	size_t home_len;

	home = getenv("HOME");
	home_len = strlen(home);
	*path = malloc(home_len + strlen(PATH_SUFFIX) + 1);
	assert(path);
	strncpy(*path, home, home_len);
	strncpy(*path + home_len, PATH_SUFFIX, strlen(PATH_SUFFIX));
	printf("using default path: %s\n", *path);
}

/* appends SUFFIX to the PATH, allocates a string that
 * must be freed by the caller
 */
char*
build_path(const char *base, const char *suffix)
{
	char *path;
	size_t base_len;

	base_len = strlen(base);
	path = malloc(base_len + strlen(suffix) + 1);
	assert(path);
	strncpy(path, base, base_len);
	strncpy(path + base_len, suffix, strlen(suffix));

	return path;
}

/* generate the path for the config file from BASE */
char*
config_path(const char *base)
{
	return build_path(base, CONFIG_PATH);
}

/* generate the path for the database file from BASE */
char*
db_path(const char *base)
{
	return build_path(base, DB_PATH);
}

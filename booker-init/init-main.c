#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int exists(const char *path);
static void default_path(char **path);
static char *build_path(const char *base, const char *suffix);
static char *config_path(const char *base);
static char *db_path(const char *base);
static int cp(const char *src, const char *dest);
static void create_config(const char *base);
static void create_db(const char *base);
static int init(const char *path);

static const char *CONFIG_PATH = "/config.ini";
static const char *DB_PATH = "/db.sqlite3";
static const char *DEFAULT_CONFIG = "resources/config.ini";
static const char *PATH_SUFFIX = "/.booker";

/* test if a file exists */
static int
exists(const char *path)
{
	return access(path, F_OK) > -1;
}

/* allocates memory for PATH, and then stores the default path in
 * PATH
 */
static void
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
static char*
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
static char*
config_path(const char *base)
{
	return build_path(base, CONFIG_PATH);
}

/* generate the path for the database file from BASE */
static char*
db_path(const char *base)
{
	return build_path(base, DB_PATH);
}

/* copy SRC to DEST */
static int
cp(const char *src, const char *dest)
{
	int in_fd, out_fd;
	char buf[8192];
	ssize_t result;

	in_fd = open(src, O_RDONLY);
	if (in_fd < 0) {
		fprintf(stderr, "failed to open %s: %s\n", src,
				strerror(errno));
		return 0;
	}
	out_fd = open(dest, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (out_fd < 0) {
		fprintf(stderr, "failed to open %s: %s\n", dest,
				strerror(errno));
		return 0;
	}

	while (1) {
		result = read(in_fd, buf, sizeof(buf));
		if (!result) break;
		if (result <= 0) {
			fprintf(stderr, "failed to read %s\n", src);
			close(in_fd);
			close(out_fd);
			return 0;
		}
		if (write(out_fd, buf, result) != result) {
			fprintf(stderr, "failed to write %s\n", dest);
			close(in_fd);
			close(out_fd);
			return 0;
		}
	}

	close(in_fd);
	close(out_fd);
	return 1;
}

/* create the default configuration files in BASE */
static void
create_config(const char *base)
{
	char *path;

	path = config_path(base);
	printf("copying %s to %s\n", DEFAULT_CONFIG, path);
	if(!cp(DEFAULT_CONFIG, path)) {
		fprintf(stderr, "failed to copy default configuration(%s) "
				"to %s\n", DEFAULT_CONFIG, path);
		exit(1);
	}
	free(path);
}

/* create the default db in BASE */
static void
create_db(const char *base)
{
	sqlite3 *db;
	char *path;

	path = db_path(base);
	printf("creating a database at %s\n", path);
	if (sqlite3_open(path, &db)) {
		fprintf(stderr, "failed to open %s: %s\n", path,
				sqlite3_errmsg(db));
		sqlite3_close(db);
		abort();
	}
	sqlite3_close(db);
}

/* create the required directories and files for booker to work */
static int
init(const char *path)
{
	if (exists(path)) {
		fprintf(stderr, "%s is already initialized\n", path);
		return 1;
	}
	assert(!mkdir(path, 0755));
	create_config(path);
	create_db(path);
	return 0;
}

int
main(int argc, char **argv)
{
	int i;
	char *path;

	path = NULL;
	for (i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-d", 2)) {
			i++;
			if (i >= argc) {
				fprintf(stderr, "no argument for -p\n");
				exit(1);
			}
			path = argv[i];
		}
	}

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);
	return init(path);
}

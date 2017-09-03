#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/arg.h"
#include "../common/db.h"
#include "../common/path.h"
#include "../common/util.h"

static const char *DEFAULT_CONFIG = "resources/config.ini";

static const char *TABLE_LIST[] = {
	"CREATE TABLE session ("
		"start DATETIME NOT NULL,"
		"end DATETIME"
	")",
	"CREATE TABLE session_user ("
		"session_id INTEGER NOT NULL,"
		"user TEXT NOT NULL,"
		"FOREIGN KEY (session_id) REFERENCES session(rowid),"
		"PRIMARY KEY (session_id, user)"
	")",
	"CREATE TABLE type ("
		"name TEXT PRIMARY KEY NOT NULL"
	")",
	"CREATE TABLE product ("
		"name TEXT NOT NULL,"
		"price NUMBER NOT NULL,"
		"qty NUMBER NOT NULL,"
		"type TEXT NOT NULL,"
		"cost NUMBER NOT NULL,"
		"FOREIGN KEY (type) REFERENCES type(name),"
		"PRIMARY KEY (name, type),"
		"CHECK (price >= 0),"
		"CHECK (cost >= 0)"
	")",
	"CREATE TABLE product_metadata ("
		"product_id INTEGER NOT NULL,"
		"key TEXT NOT NULL,"
		"value TEXT NOT NULL,"
		"FOREIGN KEY (product_id) REFERENCES product(rowid),"
		"PRIMARY KEY (product_id, key)"
	")",
	NULL
};

static int cp(const char *src, const char *dest);
static void create_config(const char *base);
static void create_db(const char *base);
static void init(const char *path);

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
	if(!cp(DEFAULT_CONFIG, path))
		eprintf("failed to copy default configuration(%s) to %s\n",
				DEFAULT_CONFIG, path);
	free(path);
}

/* create the default db in BASE */
static void
create_db(const char *base)
{
	sqlite3 *db;
	char *path;
	const char **tp;

	path = db_path(base);
	printf("creating a database at %s\n", path);
	if (sqlite3_open(path, &db)) {
		fprintf(stderr, "failed to open %s: %s\n", path,
				sqlite3_errmsg(db));
		sqlite3_close(db);
		abort();
	}

	for (tp = TABLE_LIST; *tp != NULL; tp++)
		db_exec(db, *tp);
	sqlite3_close(db);
}

/* create the required directories and files for booker to work */
static void
init(const char *path)
{
	if (exists(path))
		eprintf("%s is already initialized\n", path);
	assert(!mkdir(path, 0755));
	create_config(path);
	create_db(path);
}

/* prints the usage information */
static void
usage(void)
{
	printf("usage: %s [-h] [-d DIR]\n", argv0);
}

int
main(int argc, char **argv)
{
	char *path;

	path = NULL;
	ARGBEGIN {
	case 'd':
		path = EARGF(usage());
		if (!*path)
			eprintf("path was empty\n");
		break;
	case 'h':
		usage();
		exit(0);
		break;
	} ARGEND;

	if (path)
		printf("using provided path: %s\n", path);
	else
		default_path(&path);
	init(path);
	return 0;
}

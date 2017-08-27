#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int exists(const char *path);
void default_path(char **path);
char *config_path(const char *base);
void create_config(const char *base);
int init(const char *path);

const char *CONFIG_PATH = "/config.ini";
const char *PATH_SUFFIX = "/.booker";

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

/* Appends a config to the provided path, allocates a string that
 * must be freed by the caller
 */
char*
config_path(const char *base)
{
	char *config;
	size_t base_len;

	base_len = strlen(base);
	config = malloc(base_len + strlen(CONFIG_PATH) + 1);
	assert(config);
	strncpy(config, base, base_len);
	strncpy(config + base_len, CONFIG_PATH, strlen(CONFIG_PATH));

	return config;
}

void
create_config(const char *base)
{
	char *path;

	path = config_path(base);
	// TODO: copy file contents
	free(path);
}

/* create the required directories and files for booker to work */
int
init(const char *path)
{
	if (exists(path)) {
		fprintf(stderr, "%s is already initialized\n", path);
		return 1;
	}
	assert(!mkdir(path, 0755));
	create_config(path);
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

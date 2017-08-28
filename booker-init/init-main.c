#include <sys/types.h>
#include <sys/stat.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int exists(const char *path);
void default_path(char **path);
char *config_path(const char *base);
int cp(const char *src, const char *dest);
void create_config(const char *base);
int init(const char *path);

const char *CONFIG_PATH = "/config.ini";
const char *DEFAULT_CONFIG = "resources/config.ini";
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

/* copy SRC to DEST */
int
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

/* Create the default configuration files in BASE */
void
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
	// TODO: create database
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

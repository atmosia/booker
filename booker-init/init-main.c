#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *PATH_SUFFIX = "/.booker";

void
build_path(char **path) {
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
		build_path(&path);
	return 0;
}

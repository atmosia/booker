int exists(const char *path);
void default_path(char **path);
char *build_path(const char *base, const char *suffix);
char *config_path(const char *base);
char *db_path(const char *base);

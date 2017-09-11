/*
 * variation of 20h's arg.h
 * (https://github.com/Erethon/suckless-sbase/blob/master/arg.h)
 */

extern char *argv0;

/* use main(int argc, char *argv[]) */
#define ARGBEGIN	for (argv0 = *argv, argv++, argc--;\
					argv[0] && argv[0][1]\
					&& argv[0][0] == '-';\
					argc--, argv++) {\
				char argc_;\
				char **argv_;\
				int brk_;\
				if (argv[0][1] == '-' && argv[0][2] == '\0') {\
					argv++;\
					argc--;\
					break;\
				}\
				for (brk_ = 0, argv[0]++, argv_ = argv;\
						argv[0][0] && !brk_;\
						argv[0]++) {\
					if (argv_ != argv)\
						break;\
					argc_ = argv[0][0];\
					switch (argc_)

#define ARGEND			}\
			}

#define ARGC()		argc_

#define ARGNUMF()	(brk_ = 1, estrtonum(argv[0], 0, INT_MAX))

#define EARGF(x)	((argv[0][1] == '\0' && argv[1] == NULL)?\
				((x), exit(1), (char *)0) :\
				(brk_ = 1, (argv[0][1] != '\0')?\
					(&argv[0][1]) :\
					(argc--, argv++, argv[0])))

#define ARGF()		((argv[0][1] == '\0' && argv[1] == NULL)?\
				(char *)0 :\
				(brk_ = 1, (argv[0][1] != '\0')?\
					(&argv[0][1]) :\
					(argc--, argv++, argv[0])))

#define COMMON_ARGS \
	case 'd':\
		path = EARGF(usage());\
		if (!*path) {\
			eprintf("path was empty\n");\
		}\
		break;\
	case 'h':\
		usage();\
		exit(0);\
		break


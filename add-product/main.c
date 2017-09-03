#include <stdio.h>
#include <stdlib.h>

#include "../common/arg.h"
#include "../common/util.h"

static void
usage(void)
{
	printf("%s [-h] [-p PRICE] [-q QTY] [-c COST] -t TYPE NAME\n", argv0);
}

int
main(int argc, char **argv)
{
	unsigned price, qty, cost;
	const char *type;
	ARGBEGIN {
	case 'h':
		usage();
		exit(0);
		break;
	} ARGEND;
	return 0;
}

/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"


char *argv0;

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		goto usage;
	} ARGEND;
	if (argc)
		goto usage;

	printf("%ji\n", (intmax_t)getpgrp());
	if (fflush(stdout) || ferror(stdout) || fclose(stdout)) {
		fprintf(stderr, "%s: printf: %s\n", argv0, strerror(errno));
		return 1;
	}

	return 0;

usage:
	fprintf(stderr, "usage: %s\n", argv0);
	return 1;
}

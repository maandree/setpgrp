/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"

#define EXIT_ERROR  125
#define EXIT_EXEC   126
#define EXIT_NOENT  127


char *argv0;

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-e variable] [-pv] command [argument] ...\n", argv0);
	exit(EXIT_ERROR);
}

int
main(int argc, char *argv[])
{
	char group_str[3 * sizeof(pid_t) + 1];
	int print_to_stdout = 0;
	int print_to_stderr = 0;
	char *export = NULL;
	pid_t group;

	ARGBEGIN {
	case 'e':
		export = EARGF(usage());
		if (!*export)
			usage();
		break;
	case 'p':
		print_to_stdout = 1;
		break;
	case 'v':
		print_to_stderr = 1;
		break;
	default:
		usage();
	} ARGEND;

	if (!argc)
		usage();

	if (setpgid(0, 0)) {
		fprintf(stderr, "%s: setpgid 0 0: %s\n", argv0, strerror(errno));
		return EXIT_ERROR;
	}
	group = getpgrp();

	if (print_to_stdout) {
		printf("%ji\n", (intmax_t)group);
		if (fflush(stdout) || ferror(stdout)) {
			fprintf(stderr, "%s: printf: %s\n", argv0, strerror(errno));
			return EXIT_ERROR;
		}
	}

	if (export) {
		sprintf(group_str, "%ji", (intmax_t)group);
		if (setenv(export, group_str, 1)) {
			fprintf(stderr, "%s: setenv %s %s 1: %s\n", argv0, export, group_str, strerror(errno));
			return EXIT_ERROR;
		}
	}

	if (print_to_stderr)
		fprintf(stderr, "%s: running %s in process group %ji\n", argv0, argv[0], (intmax_t)group);

	execvp(argv[0], argv);
	fprintf(stderr, "%s: execvp %s: %s\n", argv0, argv[0], strerror(errno));
	return errno == ENOENT ? EXIT_NOENT : EXIT_EXEC;
}

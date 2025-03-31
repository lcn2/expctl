/*
 * expctl - expand control chars
 *
 * Copyright (c) 1987,1999,2015,2023,2025 by Landon Curt Noll.  All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright, this permission notice and text
 * this comment, and the disclaimer below appear in all of the following:
 *
 *       supporting documentation
 *       source copies
 *       source works derived from this source
 *       binaries derived from this source or from derived source
 *
 * LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * chongo (Landon Curt Noll) /\oo/\
 *
 * http://www.isthe.com/chongo/index.html
 * https://github.com/lcn2
 *
 * Share and Enjoy!     :-)
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/errno.h>
#include <stdbool.h>

/*
 * official version
 */
#define VERSION "1.4.0 2025-03-30"          /* format: major.minor YYYY-MM-DD */


/*
 * usage message
 */
static const char * const usage =
    "usage: %s [-h] [-V] [-o | -x] [-n] [-t]\n"
    "\n"
    "    -h        print help message and exit\n"
    "    -V        print version string and exit\n"
    "\n"
    "    -o        print non-printable bytes as \\0XXXX octal\n"
    "    -x        print non-printable bytes as \\xXX hex\n"
    "    -n        print ASCII newline as if it is a control character (def: print ASCII newline)\n"
    "    -t        print ASCII tab as if it is a control character (def: print ASCII tab)\n"
    "\n"
    "NOTE: -o and -x are cannot be used as the same time\n"
    "\n"
    "Exit codes:\n"
    "    0         all OK\n"
    "    2         -h and help string printed or -V and version string printed\n"
    "    3         command line error\n"
    " >= 10        internal error\n"
    "\n"
    "%s version: %s\n";


/*
 * static declarations
 */
static char *program = NULL;    /* our name */
static char *prog = NULL;       /* basename of program */
static const char * const version = VERSION;
/**/
static void pr_usage(FILE *stream);


/*
 * pr_usage - print usage message
 *
 * given:
 *
 *    stream - print usage message on stream, NULL ==> stderr
 */
static void
pr_usage(FILE *stream)
{
    /*
     * NULL stream means stderr
     */
    if (stream == NULL) {
        stream = stderr;
    }

    /*
     * firewall - change program if NULL
     */
    if (program == NULL) {
        program = "((NULL))";
    }

    /*
     * firewall set name if NULL
     */
    if (prog == NULL) {
        prog = rindex(program, '/');
    }
    /* paranoia if no / is found */
    if (prog == NULL) {
        prog = program;
    } else {
        ++prog;
    }

    /*
     * print usage message to stderr
     */
    fprintf(stream, usage, program, prog, version);
}


int
main(int argc, char *argv[])
{
    int c;		   /* char that we just read */
    int o_flag = false;    /* true ==> print non-printable bytes as \oXXX octal */
    int x_flag = false;    /* true ==> print non-printable bytes as \xXX hex */
    int n_flag = false;    /* true ==> print ASCII newline as if it is a control character */
    int t_flag = false;    /* true ==> print ASCII tab as if it is a control character */
    int i;

    /*
     * parse args
     */
    program = argv[0];
    while ((i = getopt(argc, argv, ":hVoxnt")) != -1) {
        switch (i) {

        case 'h':                   /* -h - print help message and exit */
            pr_usage(stderr);
            exit(2); /* ooo */
            /*NOTREACHED*/

        case 'V':                   /* -V - print version string and exit */
            (void) printf("%s\n", version);
            exit(2); /* ooo */
            /*NOTREACHED*/

	case 'o':		    /* -o - print non-printable bytes as \oXXX octal */
	    o_flag = true;
	    break;

	case 'x':		    /* -x - print non-printable bytes as \xXX hex */
	    x_flag = true;
	    break;

	case 'n':		    /* -n - print ASCII newline as if it is a control character */
	    n_flag = true;
	    break;

	case 't':		    /* -t - print ASCII tab as if it is a control character */
	    t_flag = true;
	    break;

        case ':':
            (void) fprintf(stderr, "%s: ERROR: requires an argument -- %c\n", program, optopt);
            pr_usage(stderr);
            exit(3); /* ooo */
            /*NOTREACHED*/

        case '?':
            (void) fprintf(stderr, "%s: ERROR: illegal option -- %c\n", program, optopt);
            pr_usage(stderr);
            exit(3); /* ooo */
            /*NOTREACHED*/

        default:
            fprintf(stderr, "%s: ERROR: invalid -flag\n", program);
            exit(3); /* ooo */
            /*NOTREACHED*/
        }
    }
    /* skip over command line options */
    argv += optind;
    argc -= optind;
    /* check the arg count */
    if (argc > 0) {
        fprintf(stderr, "%s: ERROR: expected at least 0 args, found: %d\n", program, argc);
        pr_usage(stderr);
        exit(3); /* ooo */
        /*NOTREACHED*/
    }

    /*
     * read chars until EOF
     */
    while ((c = getchar()) != EOF) {

	/*
	 * print any printable ASCII character as itself
	 */
	if (isascii(c) && isprint(c)) {
	    if (c == '\\') {
		printf("\\\\");
	    } else if (c == '^') {
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\^");
		}
	    } else {
		putchar(c);
	    }
	    continue;
	}

	/*
	 * print any control ASCII character as ^c
	 */
	if (isascii(c) && iscntrl(c)) {

	    /*
	     * special case some chars
	     */
	    switch (c) {
	    case '\0':
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("^%c", c+64);
		}
		break;
	    case '\a':
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\a");
		}
		break;
	    case '\b':
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\b");
		}
		break;
	    case '\f':
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\f");
		}
		break;
	    case '\n':
		if (n_flag) {
		    if (o_flag) {
			printf("\\o%03o", c);
		    } else if (x_flag) {
			printf("\\x%02x", c);
		    } else {
			printf("\\n");
		    }
		} else {
		    putchar(c);
                }
		break;
	    case '\r':
		if (o_flag) {
                    printf("\\o%03o", c);
                } else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\r");
		}
		break;
	    case '\t':
		if (t_flag) {
		    if (o_flag) {
			printf("\\o%03o", c);
		    } else if (x_flag) {
			printf("\\x%02x", c);
		    } else {
			printf("\\t");
		    }
		} else {
		    putchar(c);
		}
		break;
	    case '\v':
		if (o_flag) {
                    printf("\\o%03o", c);
                } else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\v");
		}
		break;
	    case '\177':
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("\\%03o", c);
		}
		break;
	    default:
		if (o_flag) {
		    printf("\\o%03o", c);
		} else if (x_flag) {
		    printf("\\x%02x", c);
		} else {
		    printf("^%c", c+64);
		}
		break;
	    }
	    continue;
	}

	/* what follows is either non-ASCII or an ASCII non-printable non-control character */

	/*
	 * octal expand any other char
	 */
	if (o_flag) {
	    printf("\\o%03o", c);
	} else if (x_flag) {
	    printf("\\x%02x", c);
	} else {
	    printf("\\%03o", c);
	}
    }
    exit(0);
}

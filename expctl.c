/*
 * expctl - expand control chars
 *
 * usage:
 *	expctl < infile > outfile
 *
 * @(#) $Revision: 1.1 $
 * @(#) $Id: expctl.c,v 1.1 1999/09/22 02:25:33 chongo Exp chongo $
 * @(#) $Source: /usr/local/src/cmd/expctl/RCS/expctl.c,v $
 *
 * Copyright (c) 1987 by Landon Curt Noll.  All Rights Reserved.
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
 * chongo <was here> /\oo/\
 *
 * Share and enjoy!
 */

#include <stdio.h>
#include <ctype.h>

main()
{
    int c;	/* char that we just read */

    /*
     * read chars until EOF
     */
    while ((c = getchar()) != EOF) {

	/*
	 * print any printable ASCII character as itself
	 */
	if (isascii(c) && isprint(c)) {
		putchar(c);
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
		case '\n':
		case '\t':
		    putchar(c);
		    break;
		case '\177':
		    printf("\\%03o", c);
		    break;
		default:
		    printf("^%c", c+64);
		    break;
		}
		continue;
	}

	/*
	 * octal expand any other char
	 */
	printf("\\%03o", c);
    }
    exit(0);
}

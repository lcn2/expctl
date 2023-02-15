/*
 * expctl - expand control chars
 *
 * usage:
 *	expctl < infile > outfile
 *
 * @(#) $Revision: 1.3 $
 * @(#) $Id: expctl.c,v 1.3 2015/09/06 07:13:02 root Exp $
 * @(#) $Source: /usr/local/src/bin/expctl/RCS/expctl.c,v $
 *
 * Copyright (c) 1987,2015 by Landon Curt Noll.  All Rights Reserved.
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
#include <stdlib.h>

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

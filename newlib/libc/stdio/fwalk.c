/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * and/or other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* No user fns here.  Pesch 15apr92. */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

#define _DEFAULT_SOURCE
#include <sys/cdefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
_fwalk_sglue (int (*func) (FILE *),
    struct _glue *g)
{
  FILE *fp;
  int n, ret = 0;

  /*
   * It should be safe to walk the list without locking it;
   * new nodes are only added to the end and none are ever
   * removed.
   *
   * Avoid locking this list while walking it or else you will
   * introduce a potential deadlock in [at least] refill.c.
   */
  do {
    for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
      if (fp->_flags != 0 && fp->_flags != 1 && fp->_file != -1)
	ret |= (*func) (fp);
    g = g->_next;
  } while (g != NULL);

  return ret;
}

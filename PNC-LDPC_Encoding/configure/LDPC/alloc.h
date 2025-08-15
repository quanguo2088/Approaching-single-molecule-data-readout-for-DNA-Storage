/* ALLOC.H - Interface to memory allocation procedure. */

/* Copyright (c) 1996 by Radford M. Neal
 *
 * Permission is granted for anyone to copy, use, or modify this program
 * for purposes of research or education, provided this copyright notice
 * is retained, and note is made of any changes that have been made.
 *
 * This program is distributed without any warranty, express or implied.
 * As this program was written for research purposes only, it has not been
 * tested to the degree that would be advisable in any important application.
 * All use of this program is entirely at the user's own risk.
 */
#ifndef _ALLOC_H
#define _ALLOC_H

void *chk_alloc(unsigned, unsigned); /* Calls 'calloc' and exits with error
                                        if it fails */

char *char_chk_alloc(unsigned, unsigned); /* Calls 'calloc' and exits with error
                                         if it fails */

#endif
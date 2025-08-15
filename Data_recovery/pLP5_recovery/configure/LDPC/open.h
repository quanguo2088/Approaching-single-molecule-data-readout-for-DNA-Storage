/* OPEN.H - Interface to routine for opening file that might be stdin/stdout. */

/* Copyright (c) 2001 by Radford M. Neal
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
#ifndef _OPEN_H
#define _OPEN_H
#include <cstdio>

FILE *open_file_std(char *, char *);

#endif
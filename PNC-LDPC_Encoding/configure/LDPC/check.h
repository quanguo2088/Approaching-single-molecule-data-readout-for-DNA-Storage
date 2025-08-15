/* CHECK.H - Interface to procedure for computing parity checks, etc. */

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
#ifndef _CHECK_H
#define _CHECK_H
#include "mod2sparse.h"

int check(mod2sparse *, char *, char *);

double changed(double *, char *, int);

double expected_parity_errors(mod2sparse *, double *);

double loglikelihood(double *, char *, int);
double expected_loglikelihood(double *, double *, int);

double entropy(double *, int);

#endif

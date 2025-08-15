/* MOD2CONVERT.H - Routines converting between sparse and dense mod2 matrices.*/

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
#ifndef _MOD2CONVERT_H
#define _MOD2CONVERT_H

#include "mod2dense.h"
#include "mod2sparse.h"

void mod2sparse_to_dense(mod2sparse *, mod2dense *);
void mod2dense_to_sparse(mod2dense *, mod2sparse *);

#endif
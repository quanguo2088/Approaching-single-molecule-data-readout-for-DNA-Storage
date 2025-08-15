/**
 * Author:   Modified by @Yancey
 * Date: 2023-08-10
 * Brief: LDPC Codec including encoding and decoding.
 **/
#ifndef _LDPCCODEC_H
#define _LDPCCODEC_H

#include "mod2sparse.h"
#include "mod2dense.h"
#include "mod2convert.h"

class LDPCCodec
{
public:
	explicit LDPCCodec() {};

	void init_encode(const char *pchk_file, const char *gen_file);
	void encode(char *cblk, char *sblk);

	void init_decode(const char *pchk_file, const char *gen_file);
	int LR_decode(char *recMsg, const char *corruptedCode, int maxIter);
	int LR_decode_soft(char *recMsg, double *lratio_cw, int maxIter);

private:
	/* VARIABLES DECLARED.  These global variables are set to representations
	   of the parity check and generator matrices by read_pchk and read_gen. */
	mod2sparse *H; /* Parity check matrix */

	int M; /* Number of rows in parity check matrix (equal to codeword length) */
	int N; /* Number of columns in parity check matrix (equal to check length) */

	char type; /* Type of generator matrix representation (s/d/m) */
	int *cols; /* Ordering of columns in generator matrix */

	mod2sparse *L, *U; /* Sparse LU decomposition, if type=='s' */
	int *rows;		   /* Ordering of rows in generator matrix (type 's') */

	mod2dense *G; /* Dense or mixed representation of generator matrix,
			 if type=='d' or type=='m' */

	/* PROCEDURES FOR READING DATA. */
	int read_pchk(char *);
	int read_gen(char *, int, int);

	/* The procedures obtain the generator matrix to use for
	   encoding from the global variables */
	void sparse_encode(char *, char *);
	void dense_encode(char *, char *, mod2dense *, mod2dense *);
	void mixed_encode(char *, char *, mod2dense *, mod2dense *);

	/* DECODING METHOD, ITER PARAMETERS, AND OTHER VARIABLES. */
	void initprp(mod2sparse *, double *, char *, double *);
	void iterprp(mod2sparse *, double *, char *, double *);

	int truncated_len;
	char *truncated_flag;
};

#endif

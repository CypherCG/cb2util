/*
 * Implementation of the stream cipher ARCFOUR
 *
 * Copyright (C) 2006-2010 Mathias Lafeldt <misfire@debugon.org>
 * Copyright (C) 1996-2000 Whistle Communications, Inc.
 * All rights reserved.
 *
 * This file is part of cb2util, the CodeBreaker PS2 File Utility.
 *
 * cb2util is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * cb2util is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * NOTE:
 * The ARCFOUR algorithm is believed to be fully interoperable with the RC4(TM)
 * algorithm.  RC4(TM) is trademark of RSA Data Security, Inc.
 */
#include "arcfour.h"

static inline void swap_bytes(uint8_t *a, uint8_t *b)
{
	uint8_t tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
 * Initialize an ARCFOUR context buffer using the supplied key,
 * which can have arbitrary length.
 */
void arc4_init(arc4_ctx_t *ctx, const uint8_t *key, int keylen)
{
	uint8_t j;
	int i;

	/* Initialize context with identity permutation */
	for (i = 0; i < 256; i++)
		ctx->perm[i] = (uint8_t)i;
	ctx->index1 = 0;
	ctx->index2 = 0;

	/* Randomize the permutation using key data */
	for (j = i = 0; i < 256; i++) {
		j += ctx->perm[i] + key[i % keylen];
		swap_bytes(&ctx->perm[i], &ctx->perm[j]);
	}
}

/*
 * Encrypt data using the supplied ARCFOUR context buffer.
 * Since ARCFOUR is a stream cypher, this function is used
 * for both encryption and decryption.
 */
void arc4_crypt(arc4_ctx_t *ctx, uint8_t *buf, size_t buflen)
{
	size_t i;
	uint8_t j;

	for (i = 0; i < buflen; i++) {
		/* Update modification indicies */
		ctx->index1++;
		ctx->index2 += ctx->perm[ctx->index1];

		/* Modify permutation */
		swap_bytes(&ctx->perm[ctx->index1], &ctx->perm[ctx->index2]);

		/* Encrypt/decrypt next byte */
		j = ctx->perm[ctx->index1] + ctx->perm[ctx->index2];
		buf[i] ^= ctx->perm[j];
	}
}

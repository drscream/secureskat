/*******************************************************************************
   This file is part of libTMCG.

 Copyright (C) 2004 Heiko Stamer, <stamer@gaos.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*******************************************************************************/

#ifndef INCLUDED_TMCG_SecretKey_HH
	#define INCLUDED_TMCG_SecretKey_HH

	// config.h
	#if HAVE_CONFIG_H
		#include "config.h"
	#endif

	// C++/STL header
	#include <cstdio>
	#include <cstdlib>
	#include <cassert>
	#include <string>
	#include <sstream>
	#include <iostream>
	
	// GNU multiple precision library
	#include <gmp.h>

	#include "mpz_srandom.h"
	#include "mpz_sqrtm.h"
	#include "mpz_helper.hh"
	#include "parse_helper.hh"

struct TMCG_SecretKey
{
	string						name, email, type, nizk, sig;
	mpz_t							m, y, p, q;
	// below this line are non-persistent values (pre-computation)
	mpz_t							y1, m1pq, gcdext_up, gcdext_vq, pa1d4, qa1d4;
	int								ret;
	
	TMCG_SecretKey
		(unsigned long int keysize, const string &n, const string &e):
			name(n), email(e)
	{
		mpz_t foo, bar;
		
		mpz_init(foo), mpz_init(bar);
		mpz_init(m), mpz_init(y), mpz_init(p), mpz_init(q);
		
		// sanity check, adjust keysize
		if (keysize > TMCG_MAX_KEYBITS)
			keysize = TMCG_MAX_KEYBITS;
		
		// set type of key
		ostringstream t;
		t << "TMCG/RABIN_" << keysize << "_NIZK";
		type = t.str();
		
		// generate appropriate primes for RABIN encryption with SAEP
		do
		{
			// choose random p \in Z, but with fixed size (n/2 + 1) bit
			do
			{
				mpz_ssrandomb(p, NULL, (keysize / 2L) + 1L);
			}
			while (mpz_sizeinbase(p, 2L) < ((keysize / 2L) + 1L));
			
			// make p odd
			if (mpz_even_p(p))
				mpz_add_ui(p, p, 1L);
			
			// while p is not probable prime and \equiv 3 (mod 4)
			while (!(mpz_congruent_ui_p(p, 3L, 4L) && mpz_probab_prime_p(p, 25)))
				mpz_add_ui(p, p, 2L);
			assert(!mpz_congruent_ui_p(p, 1L, 8L));
			
			// choose random q \in Z, but with fixed size (n/2 + 1) bit
			do
			{
				mpz_ssrandomb(q, NULL, (keysize / 2L) + 1L);
			}
			while (mpz_sizeinbase(q, 2L) < ((keysize / 2L) + 1L));
			
			// make q odd
			if (mpz_even_p(q))
				mpz_add_ui(q, q, 1L);
			
			// while q is not probable prime, \equiv 3 (mod 4) and
			// p \not\equiv q (mod 8)
			mpz_set_ui(foo, 8L);
			while (!(mpz_congruent_ui_p(q, 3L, 4L) && mpz_probab_prime_p(q, 25)
				&& !mpz_congruent_p(p, q, foo)))
					mpz_add_ui(q, q, 2L);
			assert(!mpz_congruent_ui_p(q, 1L, 8L));
			assert(!mpz_congruent_p(p, q, foo));
			
			// compute modulus: m = pq
			mpz_mul(m, p, q);
			
			// compute upper bound for SAEP, i.e. 2^{n+1} + 2^n
			mpz_set_ui(foo, 1L);
			mpz_mul_2exp(foo, foo, keysize);
			mpz_mul_2exp(bar, foo, 1L);
			mpz_add(bar, bar, foo);
		}
		while ((mpz_sizeinbase(m, 2L) < (keysize + 1L)) || (mpz_cmp(m, bar) >= 0));
		
		// choose random y \in NQR^\circ_m for TMCG
		do
		{
			mpz_srandomm(y, NULL, m);
		}
		while ((mpz_jacobi(y, m) != 1) || mpz_qrmn_p(y, p, q, m));
		
		// pre-compute non-persistent values
		mpz_init(y1);
		ret = mpz_invert(y1, y, m);
		assert(ret);
		mpz_init(m1pq);
		mpz_sub(foo, m, p);
		mpz_sub(foo, foo, q);
		mpz_add_ui(foo, foo, 1L);
		ret = mpz_invert(m1pq, m, foo);
		assert(ret);
		mpz_init(gcdext_up), mpz_init(gcdext_vq);
		mpz_init(pa1d4), mpz_init(qa1d4);
		mpz_gcdext(foo, gcdext_up, gcdext_vq, p, q);
		assert(mpz_cmp_ui(foo, 1L) == 0);
		mpz_mul(gcdext_up, gcdext_up, p);
		mpz_mul(gcdext_vq, gcdext_vq, q);
		mpz_set(pa1d4, p), mpz_set(qa1d4, q);
		mpz_add_ui(pa1d4, pa1d4, 1L);
		mpz_add_ui(qa1d4, qa1d4, 1L);
		mpz_fdiv_q_2exp(pa1d4, pa1d4, 2L);
		mpz_fdiv_q_2exp(qa1d4, qa1d4, 2L);
		
		// Rosario Gennaro, Daniele Micciancio, Tal Rabin:
		// 'An Efficient Non-Interactive Statistical Zero-Knowledge
		// Proof System for Quasi-Safe Prime Products',
		// 5th ACM Conference on Computer and Communication Security, CCS 1998
		
		// STAGE1/2: m = p^i * q^j, p and q prime
		// STAGE3: y \in NQR^\circ_m
		ostringstream nizk, input;
		input << m << "^" << y, nizk << "nzk^";
		size_t mnsize = mpz_sizeinbase(m, 2L) / 8;
		char *mn = new char[mnsize];
		
		// STAGE1: m Square Free, soundness error probability = d^{-nizk_stage1}
		nizk << nizk_stage1 << "^";
		for (size_t stage1 = 0; stage1 < nizk_stage1; stage1++)
		{
			// common random number foo \in Z^*_m (build from hash function g)
			do
			{
				g(mn, mnsize, (input.str()).c_str(), (input.str()).length());
				mpz_import(foo, 1, -1, mnsize, 1, 0, mn);
				mpz_mod(foo, foo, m);
				mpz_gcd(bar, foo, m);
				input << foo;
			}
			while (mpz_cmp_ui(bar, 1L));
			
			// compute bar = foo^{m^{-1} mod \phi(m)} mod m
			mpz_powm(bar, foo, m1pq, m);
			
			// update NIZK-proof stream
			nizk << bar << "^"; 
		}
		
		// STAGE2: m Prime Power Product, soundness error prob. = 2^{-nizk_stage2}
		nizk << nizk_stage2 << "^";
		for (size_t stage2 = 0; stage2 < nizk_stage2; stage2++)
		{
			// common random number foo \in Z^*_m (build from hash function g)
			do
			{
				g(mn, mnsize, (input.str()).c_str(), (input.str()).length());
				mpz_import(foo, 1, -1, mnsize, 1, 0, mn);
				mpz_mod(foo, foo, m);
				mpz_gcd(bar, foo, m);
				input << foo;
			}
			while (mpz_cmp_ui(bar, 1L));
			
			// compute square root of +-foo or +-2foo mod m
			if (mpz_qrmn_p(foo, p, q, m))
				mpz_sqrtmn_r(bar, foo, p, q, m, NULL);
			else
			{
				mpz_neg(foo, foo);
				if (mpz_qrmn_p(foo, p, q, m))
					mpz_sqrtmn_r(bar, foo, p, q, m, NULL);
				else
				{
					mpz_mul_2exp(foo, foo, 1L);
					if (mpz_qrmn_p(foo, p, q, m))
						mpz_sqrtmn_r(bar, foo, p, q, m, NULL);
					else
					{
						mpz_neg(foo, foo);
						if (mpz_qrmn_p(foo, p, q, m))
							mpz_sqrtmn_r(bar, foo, p, q, m, NULL);
						else
							mpz_set_ui(bar, 0L);
					}
				}
			}
			
			// update NIZK-proof stream
			nizk << bar << "^";
		}
		
		// STAGE3: y \in NQR^\circ_m, soundness error probability = 2^{-nizk_stage3}
		nizk << nizk_stage3 << "^";
		for (size_t stage3 = 0; stage3 < nizk_stage3; stage3++)
		{
			// common random number foo \in Z^\circ_m (build from hash function g)
			do
			{
				g(mn, mnsize, (input.str()).c_str(), (input.str()).length());
				mpz_import(foo, 1, -1, mnsize, 1, 0, mn);
				mpz_mod(foo, foo, m);
				input << foo;
			}
			while (mpz_jacobi(foo, m) != 1);
			
			// compute square root
			if (!mpz_qrmn_p(foo, p, q, m))
			{
				mpz_mul(foo, foo, y);
				mpz_mod(foo, foo, m);
			}
			mpz_sqrtmn_r(bar, foo, p, q, m, NULL);
			
			// update NIZK-proof stream
			nizk << bar << "^";
		}
		
		nizk = nizk.str();
		delete [] mn, mpz_clear(foo), mpz_clear(bar);
		
		// compute self-signature
		ostringstream data, repl;
		data << name << "|" << email << "|" << type << "|" <<
			m << "|" << y << "|" << nizk << "|";
		sig = sign(data.str());
		repl << "ID" << TMCG_KeyIDSize << "^";
		sig.replace(sig.find(repl.str()),
			(repl.str()).length() + TMCG_KeyIDSize, keyid());
	}
	
	TMCG_SecretKey
		(string s)
	{
		mpz_init(m), mpz_init(y), mpz_init(p), mpz_init(q);
		mpz_init(y1), mpz_init(m1pq), mpz_init(gcdext_up), mpz_init(gcdext_vq),
			mpz_init(pa1d4), mpz_init(qa1d4);
		
		try
		{
			// check magic
			if (!cm(s, "sec", '|'))
				throw false;
			
			// name
			name = gs(s, '|');
			if ((gs(s, '|') == NULL) || (!nx(s, '|')))
				throw false;
			
			// email
			email = gs(s, '|');
			if ((gs(s, '|') == NULL) || (!nx(s, '|')))
				throw false;
			
			// type
			type = gs(s, '|');
			if ((gs(s, '|') == NULL) || (!nx(s, '|')))
				throw false;
			
			// m
			if ((mpz_set_str(m, gs(s, '|'), TMCG_MPZ_IO_BASE) < 0) || (!nx(s, '|')))
				throw false;
			
			// y
			if ((mpz_set_str(y, gs(s, '|'), TMCG_MPZ_IO_BASE) < 0) || (!nx(s, '|')))
				throw false;
			
			// p
			if ((mpz_set_str(p, gs(s, '|'), TMCG_MPZ_IO_BASE) < 0) || (!nx(s, '|')))
				throw false;
			
			// q
			if ((mpz_set_str(q, gs(s, '|'), TMCG_MPZ_IO_BASE) < 0) || (!nx(s, '|')))
				throw false;
			
			// NIZK
			nizk = gs(s, '|');
			if ((gs(s, '|') == NULL) || (!nx(s, '|')))
				throw false;
			
			// sig
			sig = s;
			
			// pre-compute non-persistent values
			mpz_t foo;
			mpz_init(foo);
			
			ret = mpz_invert(y1, y, m);
			assert(ret);
			mpz_sub(foo, m, p);
			mpz_sub(foo, foo, q);
			mpz_add_ui(foo, foo, 1L);
			ret = mpz_invert(m1pq, m, foo);
			assert(ret);
			mpz_gcdext(foo, gcdext_up, gcdext_vq, p, q);
			assert(mpz_cmp_ui(foo, 1L) == 0);
			mpz_mul(gcdext_up, gcdext_up, p);
			mpz_mul(gcdext_vq, gcdext_vq, q);
			mpz_set(pa1d4, p), mpz_set(qa1d4, q);
			mpz_add_ui(pa1d4, pa1d4, 1L);
			mpz_add_ui(qa1d4, qa1d4, 1L);
			mpz_fdiv_q_2exp(pa1d4, pa1d4, 2L);
			mpz_fdiv_q_2exp(qa1d4, qa1d4, 2L);
			
			mpz_clear(foo);
			throw true;
		}
		catch (bool return_value)
		{
			return;
		}
	}
	
	string selfid
		()
	{
		string s = sig;
		
		// maybe a self signature
		if (s == "")
			return string("SELFSIG-SELFSIG-SELFSIG-SELFSIG-SELFSIG-SELFSIG");
		
		// check magic
		if (!cm(s, "sig", '|'))
			return NULL
		
		// skip the keyID
		if (!nx(s, '|'))
			return NULL
		
		// get the sigID
		return string(gs(s, '|'));
	}
	
	string keyid
		()
	{
		ostringstream data;
		string tmp = selfid();
		
		data << "ID" << TMCG_KeyIDSize << "^" << tmp.substr(tmp.length() -
			((TMCG_KeyIDSize < tmp.length()) ? TMCG_KeyIDSize : tmp.length()),
			(TMCG_KeyIDSize < tmp.length()) ? TMCG_KeyIDSize : tmp.length());
		return data.str();
	}
	
	string sigid
		(string s)
	{
		// check magic
		if (!cm(s, "sig", '|'))
			return string("NULL");
		
		// get the keyID
		return string(gs(s, '|'));
	}
	
	string decrypt
		(string value)
	{
		mpz_t vdata, vroot[4];
		size_t rabin_s2 = 2 * rabin_s0;
		size_t rabin_s1 = (mpz_sizeinbase(m, 2L) / 8) - rabin_s2;
		
		assert(rabin_s2 < (mpz_sizeinbase(m, 2L) / 16));
		assert(rabin_s2 < rabin_s1);
		assert(rabin_s0 < (mpz_sizeinbase(m, 2L) / 32));
		
		char *encval = new char[rabin_s0];
		char *yy = new char[rabin_s2 + rabin_s1 + 1024];
		char *r = new char[rabin_s1];
		char *Mt = new char[rabin_s2];
		char *g12 = new char[rabin_s2];
		mpz_init(vdata), mpz_init(vroot[0]), mpz_init(vroot[1]),
			mpz_init(vroot[2]), mpz_init(vroot[3]);
		try
		{
			// check magic
			if (!cm(value, "enc", '|'))
				throw false;
			
			// check keyID
			if (!cm(value, keyid(), '|'))
				throw false;
			
			// vdata
			if ((mpz_set_str(vdata, gs(value, '|'), TMCG_MPZ_IO_BASE) < 0) ||
				(!nx(value, '|')))
					throw false;
			
			// decrypt value, compute modular square roots
			if (!mpz_qrmn_p(vdata, p, q, m))
				throw false;
			mpz_sqrtmn_fast_all(vroot[0], vroot[1], vroot[2], vroot[3], vdata,
				p, q, m, gcdext_up, gcdext_vq, pa1d4, qa1d4);
			for (size_t k = 0; k < 4; k++)
			{
				if ((mpz_sizeinbase(vroot[k], 2L) / 8) <= (rabin_s1 + rabin_s2))
				{
					size_t cnt = 1;
					mpz_export(yy, &cnt, -1, rabin_s2 + rabin_s1, 1, 0, vroot[k]);
					memcpy(Mt, yy, rabin_s2), memcpy(r, yy + rabin_s2, rabin_s1);
					g(g12, rabin_s2, r, rabin_s1);
					
					for (size_t i = 0; i < rabin_s2; i++)
						Mt[i] ^= g12[i];
					
					memset(g12, 0, rabin_s0);
					if (memcmp(Mt + rabin_s0, g12, rabin_s0) == 0)
					{
						memcpy(encval, Mt, rabin_s0);
						throw true;
					}
				}
			}
			throw false;
		}
		catch (bool success)
		{
			delete [] yy, delete [] g12, delete [] Mt, delete [] r;
			mpz_clear(vdata), mpz_clear(vroot[0]), mpz_clear(vroot[1]),
				mpz_clear(vroot[2]), mpz_clear(vroot[3]);
			if (success)
			{
				string tmp = string(encval);
				delete [] encval;
				return tmp;
			}
			else
				return NULL;
		}
	}
	
	string sign
		(const string &data)
	{
		size_t mdsize = gcry_md_get_algo_dlen(TMCG_GCRY_MD_ALGO);
		size_t mnsize = mpz_sizeinbase(m, 2L) / 8;
		mpz_t foo, foo_sqrt[4];
		mpz_init(foo), mpz_init(foo_sqrt[0]), mpz_init(foo_sqrt[1]),
			mpz_init(foo_sqrt[2]), mpz_init(foo_sqrt[3]);
		
		assert(mpz_sizeinbase(m, 2L) > (mnsize * 8));
		assert(mnsize > (mdsize + rabin_k0));
		
		// WARNING: only a probabilistic algorithm (Rabin's signature scheme)
		// PRab from [Bellare, Rogaway: The Exact Security of Digital Signatures]
		do
		{
			char *r = new char[rabin_k0];
			gcry_randomize((unsigned char*)r, rabin_k0, GCRY_STRONG_RANDOM);
			
			char *Mr = new char[data.length() + rabin_k0];
			memcpy(Mr, data.c_str(), data.length());
			memcpy(Mr + data.length(), r, rabin_k0);
			
			char *w = new char[mdsize];
			h(w, Mr, data.length() + rabin_k0);
			
			char *g12 = new char[mnsize];
			g(g12, mnsize - mdsize, w, mdsize);
			
			for (size_t i = 0; i < rabin_k0; i++)
				r[i] ^= g12[i];
			
			char *yy = new char[mnsize];
			memcpy(yy, w, mdsize);
			memcpy(yy + mdsize, r, rabin_k0);
			memcpy(yy + mdsize + rabin_k0, g12 + rabin_k0, mnsize - mdsize - rabin_k0);
			mpz_import(foo, 1, -1, mnsize, 1, 0, yy);
			
			delete [] yy, delete [] g12, delete [] w, delete [] Mr, delete [] r;
		}
		while (!mpz_qrmn_p(foo, p, q, m));
		mpz_sqrtmn_fast_all(foo_sqrt[0], foo_sqrt[1], foo_sqrt[2], foo_sqrt[3], foo,
			p, q, m, gcdext_up, gcdext_vq, pa1d4, qa1d4);
		
		// choose square root randomly (one of four)
		mpz_srandomb(foo, NULL, 2L);
		
		ostringstream ost;
		ost << "sig|" << keyid() << "|" << foo_sqrt[mpz_get_ui(foo) % 4] << "|";
		mpz_clear(foo), mpz_clear(foo_sqrt[0]), mpz_clear(foo_sqrt[1]),
			mpz_clear(foo_sqrt[2]), mpz_clear(foo_sqrt[3]);
		
		return ost.str();
	}
	
	~TMCG_SecretKey
		()
	{
		mpz_clear(m), mpz_clear(y), mpz_clear(p), mpz_clear(q);
		// release non-persistent values
		mpz_clear(y1), mpz_clear(m1pq), mpz_clear(gcdext_up),
		mpz_clear(gcdext_vq), mpz_clear(pa1d4), mpz_clear(qa1d4);
	}
};

friend ostream& operator<< 
	(ostream &out, const TMCG_SecretKey &key)
{
	return out << "sec|" << key.name << "|" << key.email << "|" << key.type <<
		"|" << key.m << "|" << key.y << "|" << key.p << "|" << key.q << "|" <<
		key.nizk << "|" << key.sig;
}

#endif
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

#ifndef INCLUDED_VTMF_CardSecret_HH
	#define INCLUDED_VTMF_CardSecret_HH

	// config.h
	#if HAVE_CONFIG_H
		#include "config.h"
	#endif

	// C and STL header
	#include <cstdio>
	#include <cstdlib>
	#include <cassert>
	#include <string>
	#include <sstream>
	#include <iostream>

	// GNU multiple precision library
	#include <gmp.h>
	
	#include "parse_helper.hh"

struct VTMF_CardSecret
{
	mpz_t r;
	
	VTMF_CardSecret
		();
	
	VTMF_CardSecret
		(const VTMF_CardSecret& that);
	
	VTMF_CardSecret& operator =
		(const VTMF_CardSecret& that);
	
	bool import
		(std::string s);
	
	~VTMF_CardSecret
		();
};

std::ostream& operator<<
	(std::ostream &out, const VTMF_CardSecret &cardsecret);

#endif

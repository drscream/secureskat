/*******************************************************************************
   This file is part of SecureSkat.

 Copyright (C) 2002, 2003, 2004, 2006, 2007,
                                       2017  Heiko Stamer <HeikoStamer@gmx.net>

   SecureSkat is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   SecureSkat is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with SecureSkat; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
*******************************************************************************/

#ifndef INCLUDED_SecureSkat_pki_HH
	#define INCLUDED_SecureSkat_pki_HH
	
	#include "SecureSkat_defs.hh"
	#include "SecureSkat_misc.hh"
	
	void get_secret_key
		(const std::string &filename, TMCG_SecretKey &sec, std::string &prefix);
	void get_public_keys
		(const std::string &filename,
		std::map<std::string, TMCG_PublicKey> &keys);
	void set_public_keys
		(const std::string &filename,
		const std::map<std::string, TMCG_PublicKey> &keys);
	void create_pki
		(int &pki7771_port, int &pki7771_handle);
	void release_pki
		(int pki7771_handle);
#endif

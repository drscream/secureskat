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

#ifndef INCLUDED_TMCG_StackSecret_HH
	#define INCLUDED_TMCG_StackSecret_HH

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
	#include <vector>
	#include <algorithm>
	#include <functional>
	
	// GNU multiple precision library
	#include <gmp.h>

	#include "mpz_srandom.h"
	#include "parse_helper.hh"

template <typename CardSecretType> struct TMCG_StackSecret
{
	vector<pair<size_t, CardSecretType> >	stack;
	
	struct eq_first_component : public binary_function<
		pair<size_t, CardSecretType>, pair<size_t, CardSecretType>, bool>
	{
		bool operator() 
			(const pair<size_t, CardSecretType>& p1,
			 const pair<size_t, CardSecretType>& p2) const
		{
			return (p1.first == p2.first);
		}
	};
	
	TMCG_StackSecret
		()
	{
	}
	
	TMCG_StackSecret& operator =
		(const TMCG_StackSecret<CardSecretType>& that)
	{
		stack.clear();
		stack = that.stack;
	}
	
	const pair<size_t, CardSecretType>& operator []
		(size_t n) const
	{
		return stack[n];
	}
	
	pair<size_t, CardSecretType>& operator []
		(size_t n)
	{
		return stack[n];
	}
	
	size_t size
		() const
	{
		return stack.size();
	}
	
	void push
		(size_t index, const CardSecretType& cs)
	{
		stack.push_back(pair<size_t, CardSecretType>(index, cs));
	}
	
	void clear
		()
	{
		stack.clear();
	}
	
	bool find
		(size_t index)
	{
		return (std::find_if(stack.begin(), stack.end(),
			std::bind2nd(eq_first_component(),
				pair<size_t, CardSecretType>(index, CardSecretType()))) != stack.end());
	}
	
	bool import
		(string s)
	{
		size_t size = 0;
		char *ec;
		
		try
		{
			// check magic
			if (!cm(s, "sts", '^'))
				throw false;
			
			// size of stack
			if (gs(s, '^') == NULL)
				throw false;
			size = strtoul(gs(s, '^'), &ec, 10);
			if ((*ec != '\0') || (size <= 0) || (!nx(s, '^')))
				throw false;
			
			// cards on stack
			for (size_t i = 0; i < size; i++)
			{
				pair<size_t, CardSecretType> lej;
				
				// permutation index
				if (gs(s, '^') == NULL)
					throw false;
				lej.first = (size_t)strtoul(gs(s, '^'), &ec, 10);
				if ((*ec != '\0') || (lej.first < 0) || (lej.first >= size) ||
					(!nx(s, '^')))
						throw false;
				
				// card secret
				if (gs(s, '^') == NULL)
					throw false;
				if ((!lej.second.import(gs(s, '^'))) || (!nx(s, '^')))
					throw false;
				
				// store pair
				stack.push_back(lej);
			}
			
			throw true;
		}
		catch (bool return_value)
		{
			return return_value;
		}
	}
	
	~TMCG_StackSecret
		()
	{
		stack.clear();
	}
};

template<typename CardSecretType> friend ostream& operator<<
	(ostream &out, const TMCG_StackSecret<CardSecretType> &ss)
{
	out << "sts^" << ss.size() << "^";
	for (size_t i = 0; i < ss.size(); i++)
		out << ss[i].first << "^" << ss[i].second << "^";
	return out;
}

#endif
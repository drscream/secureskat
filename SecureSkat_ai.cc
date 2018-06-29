/*******************************************************************************
   This file is part of SecureSkat.

 Copyright (C) 2018  Heiko Stamer <HeikoStamer@gmx.net>

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*******************************************************************************/

#include "SecureSkat_defs.hh"
#include "SecureSkat_rule.hh"

void announce (const size_t spiel)
{
	switch (spiel % 100)
	{
		case 9:
			std::cout << "CMD sagean Sc" << std::endl;
			break;
		case 10:
			std::cout << "CMD sagean Ro" << std::endl;
			break;
		case 11:
			std::cout << "CMD sagean Gr" << std::endl;
			break;
		case 12:
			std::cout << "CMD sagean Ei" << std::endl;
			break;
		case 23:
			std::cout << "CMD sagean Nu" << std::endl;
			break;
		case 24:
			std::cout << "CMD sagean Gd" << std::endl;
			break;
	}
}

size_t num_jacks (const std::vector<size_t> &cards)
{
	size_t nj = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (std::count(cards.begin(), cards.end(), i))
			nj++;
	}
	return nj;
}

bool high_jacks (const std::vector<size_t> &cards)
{
	if (std::count(cards.begin(), cards.end(), 0) &&
		std::count(cards.begin(), cards.end(), 1))
		return true;
	return false;
}

size_t num_aces (const std::vector<size_t> &cards)
{
	size_t na = 0;
	if (std::count(cards.begin(), cards.end(), 4))
		na++;
	if (std::count(cards.begin(), cards.end(), 11))
		na++;
	if (std::count(cards.begin(), cards.end(), 18))
		na++;
	if (std::count(cards.begin(), cards.end(), 25))
		na++;
	return na;
}

size_t num_tens (const std::vector<size_t> &cards)
{
	size_t nt = 0;
	if (std::count(cards.begin(), cards.end(), 5))
		nt++;
	if (std::count(cards.begin(), cards.end(), 12))
		nt++;
	if (std::count(cards.begin(), cards.end(), 19))
		nt++;
	if (std::count(cards.begin(), cards.end(), 26))
		nt++;
	return nt;
}

size_t num_suite (const size_t spiel, const std::vector<size_t> &cards)
{
	size_t ns = 0;
	switch (spiel % 100)
	{
		case 9:
			if (std::count(cards.begin(), cards.end(), 25))
				ns++;
			if (std::count(cards.begin(), cards.end(), 26))
				ns++;
			if (std::count(cards.begin(), cards.end(), 27))
				ns++;
			if (std::count(cards.begin(), cards.end(), 28))
				ns++;
			if (std::count(cards.begin(), cards.end(), 29))
				ns++;
			if (std::count(cards.begin(), cards.end(), 30))
				ns++;
			if (std::count(cards.begin(), cards.end(), 31))
				ns++;
			break;
		case 10:
			if (std::count(cards.begin(), cards.end(), 18))
				ns++;
			if (std::count(cards.begin(), cards.end(), 19))
				ns++;
			if (std::count(cards.begin(), cards.end(), 20))
				ns++;
			if (std::count(cards.begin(), cards.end(), 21))
				ns++;
			if (std::count(cards.begin(), cards.end(), 22))
				ns++;
			if (std::count(cards.begin(), cards.end(), 23))
				ns++;
			if (std::count(cards.begin(), cards.end(), 24))
				ns++;
			break;
		case 11:
			if (std::count(cards.begin(), cards.end(), 11))
				ns++;
			if (std::count(cards.begin(), cards.end(), 12))
				ns++;
			if (std::count(cards.begin(), cards.end(), 13))
				ns++;
			if (std::count(cards.begin(), cards.end(), 14))
				ns++;
			if (std::count(cards.begin(), cards.end(), 15))
				ns++;
			if (std::count(cards.begin(), cards.end(), 16))
				ns++;
			if (std::count(cards.begin(), cards.end(), 17))
				ns++;
			break;
		case 12:
			if (std::count(cards.begin(), cards.end(), 4))
				ns++;
			if (std::count(cards.begin(), cards.end(), 5))
				ns++;
			if (std::count(cards.begin(), cards.end(), 6))
				ns++;
			if (std::count(cards.begin(), cards.end(), 7))
				ns++;
			if (std::count(cards.begin(), cards.end(), 8))
				ns++;
			if (std::count(cards.begin(), cards.end(), 9))
				ns++;
			if (std::count(cards.begin(), cards.end(), 10))
				ns++;
			break;
	}
	return ns;
}

bool full_suite (const std::vector<size_t> &cards)
{
	if (std::count(cards.begin(), cards.end(), 4) &&
		std::count(cards.begin(), cards.end(), 5) &&
		std::count(cards.begin(), cards.end(), 6))
		return true;
	if (std::count(cards.begin(), cards.end(), 4) &&
		std::count(cards.begin(), cards.end(), 6) &&
		(num_suite(12, cards) > 3))
		return true;
	if (std::count(cards.begin(), cards.end(), 5) &&
		std::count(cards.begin(), cards.end(), 6) &&
		(num_suite(12, cards) > 4))
		return true;
	if (std::count(cards.begin(), cards.end(), 11) &&
		std::count(cards.begin(), cards.end(), 12) &&
		std::count(cards.begin(), cards.end(), 13))
		return true;
	if (std::count(cards.begin(), cards.end(), 11) &&
		std::count(cards.begin(), cards.end(), 13) &&
		(num_suite(11, cards) > 3))
		return true;
	if (std::count(cards.begin(), cards.end(), 12) &&
		std::count(cards.begin(), cards.end(), 13) &&
		(num_suite(11, cards) > 4))
		return true;
	if (std::count(cards.begin(), cards.end(), 18) &&
		std::count(cards.begin(), cards.end(), 19) &&
		std::count(cards.begin(), cards.end(), 20))
		return true;
	if (std::count(cards.begin(), cards.end(), 18) &&
		std::count(cards.begin(), cards.end(), 20) &&
		(num_suite(10, cards) > 3))
		return true;
	if (std::count(cards.begin(), cards.end(), 19) &&
		std::count(cards.begin(), cards.end(), 20) &&
		(num_suite(10, cards) > 4))
		return true;
	if (std::count(cards.begin(), cards.end(), 25) &&
		std::count(cards.begin(), cards.end(), 26) &&
		std::count(cards.begin(), cards.end(), 27))
		return true;
	if (std::count(cards.begin(), cards.end(), 25) &&
		std::count(cards.begin(), cards.end(), 27) &&
		(num_suite(9, cards) > 3))
		return true;
	if (std::count(cards.begin(), cards.end(), 26) &&
		std::count(cards.begin(), cards.end(), 27) &&
		(num_suite(9, cards) > 4))
		return true;
	return false;
}

size_t eval (const std::vector<size_t> &cards, const bool starts)
{
	size_t nj = num_jacks(cards);
	bool hj = high_jacks(cards);
	size_t na = num_aces(cards);
	size_t nt = num_tens(cards);
	bool fs = full_suite(cards);
	// evaluate game of Grand
	if (starts)
	{
// TODO: hand
		if (hj && (na > 2) && (nt > 2))
			return 24;
		if (hj && fs)
			return 24;
	}
	else
	{
// TODO: hand
		if ((nj > 2) && (na > 2) && (nt > 2))
			return 24;
		if ((nj > 2) && fs)
			return 24;
	}
	// TODO: other suits and Null
	return 0;
}

size_t pkr_self = 100, pkr_pos = 100, pkr_spielt = 100;
size_t spiel = 0, reiz_counter = 0, biete = 0;
bool reize_dran = false, lege_dran = false, gepasst = false, handspiel = false;
std::vector<std::string> nicks, names;
std::vector<size_t> cards, ocards, stich;

void process_command (size_t &readed, char *buffer)
{
	const size_t BUFFER_SIZE = 65536;
	if ((readed <= 0) || (readed > BUFFER_SIZE))
		return;
	// detect line endings
	std::vector<size_t> pos_delim;
	size_t cnt_delim = 0, cnt_pos = 0, pos = 0;
	for (size_t i = 0; i < readed; i++)
	{
	    if (buffer[i] == '\n')
			cnt_delim++, pos_delim.push_back(i);
	}
	// process each line
	while (cnt_delim >= 1)
	{
	    char xtmp[BUFFER_SIZE];
	    memset(xtmp, 0, sizeof(xtmp));
	    memcpy(xtmp, buffer + cnt_pos, pos_delim[pos] - cnt_pos);
	    --cnt_delim, cnt_pos = pos_delim[pos] + 1, pos++;
	    std::string cmd = xtmp;
	    size_t ei;
	    std::vector<std::string> par;
//std::cerr << "parse_random: CMD = " << cmd << std::endl;
	    // parse params
	    while ((ei = cmd.find(" ", 0)) != cmd.npos)
	    {
			par.push_back(cmd.substr(0, ei));
			cmd = cmd.substr(ei + 1, cmd.length() - ei - 1);
	    }
	    par.push_back(cmd);
	    if (par.size() >= 2)
	    {
			size_t from = 100;
			for (size_t j = 0; j < nicks.size(); j++)
			    if (par[0] == nicks[j])
					from = j;
						
			if ((par[1] == "INIT") && (par.size() >= 4) && 
			    (nicks.size() < 3))
			{
			    if (par[2] == par[0])
					pkr_self = nicks.size();
			    nicks.push_back(par[2]);
			    std::string name;
			    for (size_t j = 0; j < (par.size() - 3); j++)
			    {
					name += par[3 + j];
					if (j < (par.size() - 4))
					    name += " ";
			    }
			    names.push_back(name);
			}
			if ((par[1] == "DONE") && (par.size() == 2) && (from == pkr_self))
			{
				// nothing to do
			}
			if ((par[1] == "MISCHEN") && (par.size() == 2) && (from == pkr_self))
			{
				// nothing to do
			}
			if ((par[1] == "GEBEN") && (par.size() == 2) && (from == pkr_self))
			{
				// nothing to do
			}
			if ((par[1] == "KARTE") && (par.size() == 3) && (from == pkr_self))
			{
				cards.push_back(atoi(par[2].c_str()));
			}
			if ((par[1] == "START") && (par.size() == 3) && (from == pkr_self))
			{
			    pkr_pos = atoi(par[2].c_str());
			    reize_dran = (pkr_pos == 1) ? true : false;
				reiz_counter = 0, spiel = 0;
				handspiel = false;
				spiel = eval(cards, !pkr_pos);
				if (spiel)
					biete = (skat_spitzen(spiel, cards) * (spiel % 100));
			}
			if ((par[1] == "RAMSCH") && (par.size() == 2) && (from == pkr_self))
			{
				// nothing to do
			}
			if ((par[1] == "SPIELT") && (par.size() == 2) && (pkr_self != 100))
			{
				if (par[0] == nicks[0])
					pkr_spielt = 0;
				else if (par[0] == nicks[1])
					pkr_spielt = 1;
				else if (par[0] == nicks[2])
					pkr_spielt = 2;
				if (pkr_spielt == pkr_self)
				{
					// TODO: decide
					if (tmcg_mpz_wrandom_ui() & 1L)
						std::cout << "CMD skat" << std::endl << std::flush;
					else
						std::cout << "CMD hand" << std::endl << std::flush;
				}
			}
			if ((par[1] == "PASSE") && (par.size() == 2))
			{
				if (from == pkr_self)
					gepasst = true;
				reize_dran = (!gepasst) ? true : false;
			}
			if ((par[1] == "REIZE") && (par.size() == 3))
			{
				reiz_counter++;
				if (from == pkr_self)
					reize_dran = false;
				else 
					reize_dran = (!gepasst) ? true : false;
			}
			if ((par[1] == "HAND") && (par.size() == 2) && (from == pkr_spielt))
			{
				reize_dran = false;
				handspiel = true;
				if ((from == pkr_spielt) && (pkr_spielt == pkr_self))
				{
					announce(spiel);
				}
			}
			if ((par[1] == "SKAT") && (par.size() == 2) && (from == pkr_spielt))
			{
				reize_dran = false;
				if ((from == pkr_spielt) && (pkr_spielt == pkr_self))
				{
					// TODO: reevaluate and bunker points
					std::string card0 = skat_type2string(cards[0]);
					std::string card1 = skat_type2string(cards[1]);
					cards.erase(cards.begin());
					cards.erase(cards.begin());
					std::cout << "CMD druecke " << 
						card0.substr(0, card0.length() - 1) << 
							" " << 
						card1.substr(0, card1.length() - 1) << 
						std::endl << std::flush;
				}
			}
			if ((par[1] == "DRUECKE") && (par.size() == 2) &&
				(from == pkr_spielt))
			{
				if ((from == pkr_spielt) && (pkr_spielt == pkr_self))
				{
					announce(spiel);
				}
			}
			if ((par[1] == "SAGEAN") && (par.size() == 3) && 
				(from == pkr_spielt))
			{
				spiel = atoi(par[2].c_str());
				lege_dran = (pkr_pos == 0) ? true : false;
			}
			if ((par[1] == "OUVERT") && (par.size() == 3) && 
				(from == pkr_spielt))
			{
				ocards.push_back(atoi(par[2].c_str()));
			}
			if ((par[1] == "LEGE") && (par.size() == 3))
			{
				size_t card = atoi(par[2].c_str());
				stich.push_back(card);
				lege_dran = ((stich.size() < 3) && 
					(((from + 1) % 3) == pkr_self)) ? true : false;
				// remove played card from my stack
				if (par[0] == nicks[pkr_self])
				{
					for (std::vector<size_t>::iterator ci = 
						cards.begin(); ci != cards.end(); ci++)
					{
						if (card == *ci)
						{
							cards.erase(ci);
							break;
						}
					}
    			}
				// remove played ouvert card from stack
				if ((par[0] == nicks[pkr_spielt]) && (ocards.size() > 0))
				{
					for (std::vector<size_t>::iterator oci = 
						ocards.begin(); oci != ocards.end(); oci++)
					{
						if (card == *oci)
						{
							ocards.erase(oci);
							break;
	    				}
					}
				}
				// TODO: count points of stich and parties
			}
			if ((par[1] == "BSTICH") && (par.size() == 2))
			{
				stich.clear();
				lege_dran = (par[0] == nicks[pkr_self]) ? true : false;
			}
			if ((par[1] == "STOP") && (par.size() == 2) && (from == pkr_self))
			{
				cards.clear(), ocards.clear(), stich.clear();
				pkr_pos = 100, pkr_spielt = 100, biete = 0, spiel = 0;
				reize_dran = false, lege_dran = false, gepasst = false;
			}
			if ((par[1] == "GEWONNEN") && (par.size() == 2))
			{
				// nothing to do
			}
			if ((par[1] == "VERLOREN") && (par.size() == 2))
			{
				// nothing to do
			}
			if ((par[1] == "PROTO") && (par.size() == 3))
			{
				// nothing to do
			}
		}
	} // end 2nd while
	char ytmp[BUFFER_SIZE];
	memset(ytmp, 0, sizeof(ytmp));
	readed -= cnt_pos;
	memcpy(ytmp, buffer + cnt_pos, readed);
	memcpy(buffer, ytmp, readed);
}

void act()
{
	if (reize_dran)
	{
		if (spiel && (biete > skat_reizwert[reiz_counter]))
			std::cout << "CMD reize" << std::endl << std::flush;
		else
			std::cout << "CMD passe" << std::endl << std::flush;
	}
	else if (lege_dran)
	{
		if ((stich.size() == 0) && (cards.size() > 0))
		{
			// Anspiel
			size_t idx = tmcg_mpz_wrandom_ui() % cards.size();
			std::string card = skat_type2string(cards[idx]);
			std::cout << "CMD lege " << 
				card.substr(0, card.length() - 1) << std::endl << std::flush;
		}
		else
		{
			// Stechen, Übernehmen oder Buttern
			std::vector<size_t> allowed_cards;
			for (std::vector<size_t>::iterator ci = cards.begin();
				ci != cards.end(); ci++)
			{
				if (skat_rulectl(stich[0], *ci, spiel, cards))
				{
					allowed_cards.push_back(*ci);
				}
			}
			assert((allowed_cards.size() > 0));
			size_t idx = tmcg_mpz_wrandom_ui() % allowed_cards.size();
			std::string card = skat_type2string(allowed_cards[idx]);
			std::cout << "CMD lege " << 
				card.substr(0, card.length() - 1) << std::endl << std::flush;
		}
		lege_dran = false;
	}
}

int main (int argc, char **argv)
{
	int fd = fileno(stdin); // file descriptor of STDIN
	fd_set rfds; // set of read descriptors
	int mfds = 0; // highest-numbered descriptor
	struct timeval tv; // timeout structure for select(2)
	char buffer[1024];
	size_t readed = 0;

	std::cout << argv[0] << " (c) 2018 <HeikoStamer@gmx.net> " << std::endl;
	while (1)
	{
		// select(2) -- initialize file descriptors
		FD_ZERO(&rfds);
		MFD_SET(fd, &rfds);

		// select(2) -- initialize timeout
		tv.tv_sec = 1L; // seconds
		tv.tv_usec = 0L; // microseconds

		// select(2)
		int ret = select(mfds + 1, &rfds, NULL, NULL, &tv);
		
		// error occured
		if ((ret < 0) && (errno != EINTR))
		{
			perror("SecureSkat_ai::main (select)");
			return -1;
		}
		
		if ((ret > 0) && FD_ISSET(fd, &rfds))
		{
			ssize_t num = read(fd, buffer + readed, sizeof(buffer) - readed);
			if (num < 0)
			{
				perror("SecureSkat_ai::main (read)");
				return -1;
			}
			readed += num;
			process_command(readed, buffer);
			if (num == 0)
			{
				// if parent process is dead, block child in this loop
				while (1)
					sleep(100);
			}
		}
		
		if (ret == 0)
			act();
	}
	return 0;
}


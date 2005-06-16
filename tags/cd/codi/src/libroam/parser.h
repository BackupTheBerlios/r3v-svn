/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class r3vMap;

class r3vParser
{
	public:
		virtual r3vMap* parse(const std::string &file) = 0;
};

class DEMParser : public r3vParser
{
	public:
		r3vMap* parse(const std::string &file);
	
	private:
		char readChar();
		double readDouble(int length, char *aux);
		int readInt(int length, char *aux);
		std::string readString(int length, char *aux);
		void readHeights(int howMany, double factor, std::vector<double> *v, char *aux);
		
		std::ifstream *m_file;
};

class PMParser : public r3vParser
{
	public:
		r3vMap* parse(const std::string &file);
	
	private:
		std::ifstream *m_file;
};


#endif

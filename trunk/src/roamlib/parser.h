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

#include <vector>

class QFile;

class r3vMap;

class r3vParser
{
	public:
		virtual r3vMap* parse(QFile &file) = 0;
};

class DEMParser : r3vParser
{
	public:
		r3vMap* parse(QFile &file);
	
	private:
		static QChar readChar(QFile &file);
		static double readDouble(QFile &file, int length, char *aux);
		static int readInt(QFile &file, int length, char *aux);
		static QString readString(QFile &file, int length, char *aux);
		static void readHeights(QFile &file, int howMany, double factor, std::vector<double> *v, char *aux);
};

class myParser : r3vParser
{
	public:
		r3vMap* parse(QFile &file);
};


#endif

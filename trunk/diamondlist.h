/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIAMONDLIST_H
#define DIAMONDLIST_H

#include <map>

class diamond;
class triangle;

class diamondList
{
	public:
		void clear();
		
		void insert(triangle *t1, triangle *t2);
		void remove(diamond *d);
		
		diamond *last() const;
// 		void print() const;
		
	private:
		std::multimap<double, diamond*> m_diamonds;
};

#endif

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

typedef std::multimap<double, diamond*>::iterator diamondListIterator;
typedef std::multimap<double, diamond*>::const_iterator diamondListConstIterator;

class diamondList
{
	public:
		void clear();
		
		void insert(triangle *t1, triangle *t2);
		void insert(diamond *d);
		void remove(diamond *d);
		
		diamondListConstIterator begin() const;
		diamondListConstIterator end() const;
		diamond *first() const;
// 		void print() const;
		
	private:
		std::multimap<double, diamond*> m_diamonds;
};

#endif

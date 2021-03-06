/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef TRIANGLELIST_H
#define TRIANGLELIST_H

#include <map>

class triangle;

typedef std::multimap<double, triangle*>::iterator triangleListIterator;
typedef std::multimap<double, triangle*>::const_iterator triangleListConstIterator;

class triangleList
{
	public:
		void clear();
		
		void insert(triangle *t);
		void remove(triangle *t);
		
		triangleListConstIterator begin() const;
		triangleListConstIterator end() const;
		triangle *last() const;
		
	private:
		std::multimap<double, triangle*> m_triangles;
};

#endif

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "trianglelist.h"
#include "triangle.h"

void triangleList::clear()
{
	m_triangles.clear();
}

void triangleList::insert(triangle *t)
{
	std::multimap<double, triangle*>::iterator it;
	it = m_triangles.insert(std::pair<double, triangle*>(t->priority(), t));
	t->setOwnIterator(it);
}

void triangleList::remove(triangle *t)
{
	m_triangles.erase(t->ownIterator());
}

triangle *triangleList::last() const
{
	std::multimap<double, triangle*>::const_iterator it = m_triangles.end();
	--it;
	return (*it).second;
}

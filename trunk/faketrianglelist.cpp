/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "faketrianglelist.h"
#include "triangle.h"

void fakeTriangleList::clear()
{
	m_triangles.clear();
}

void fakeTriangleList::insert(const fakeTriangle &ft)
{
	std::multimap<double, triangle*>::iterator it;
	it = m_triangles.insert(std::pair<double, triangle*>(ft.m_priority, ft.m_t));
	ft.m_t->m_it = it;
}

void fakeTriangleList::remove(triangle *t)
{
	m_triangles.erase(t->m_it);
}

triangle *fakeTriangleList::last() const
{
	std::multimap<double, triangle*>::const_iterator it = m_triangles.end();
	--it;
	return (*it).second;
}
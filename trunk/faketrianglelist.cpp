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
	QValueList<fakeTriangle>::iterator it;
	it = m_triangles.begin();
	
	if (it == m_triangles.end())
	{
		// m_triangles is empty
		m_triangles.append(ft);
	}
	else if (ft < *it)
	{
		// ft is smaller than the first element of m_triangles so 
		// put ft at the begging of m_triangles
		m_triangles.prepend(ft);
	}
	else
	{
		while(it != m_triangles.end() && (*it < ft)) ++it;
		// insert(it, x) 
		// Inserts the value x in front of the item pointed to by the iterator, it. 
		m_triangles.insert(it, ft);
	}
}

void fakeTriangleList::remove(triangle *t)
{
	QValueList<fakeTriangle>::iterator it;
	for (it = m_triangles.begin(); it != m_triangles.end(); ++it)
	{
		if ((*(*it)) == t)
		{
			m_triangles.remove(it);
			break;
		}
	}
}

const fakeTriangle& fakeTriangleList::last() const
{
	return m_triangles.last();
}

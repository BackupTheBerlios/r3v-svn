/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <algorithm>

#include <assert.h>
#include <stdio.h>

#include "node.h"
#include "triangle.h"

node::node(double x, double y, double z, int r, int g, int b)
{
	m_coords[0] = x;
	m_coords[2] = y;
	m_coords[1] = z;
	
	m_color[0] = (double)r / 255;
	m_color[1] = (double)g / 255;
	m_color[2] = (double)b / 255;
}

const double *node::color() const
{
// 	printf("%f %f %f\n", m_color[0], m_color[1], m_color[2]);
	return m_color;
}

const double *node::coords() const
{
	return m_coords;
}

double node::getX() const
{
	return m_coords[0];
}

double node::getZ() const
{
	return m_coords[2];
}

double node::getHeight() const
{
	return m_coords[1];
}

bool node::operator==(const node &n) const
{
	return m_coords[0] == n.m_coords[0] && m_coords[1] == n.m_coords[1] && m_coords[2] == n.m_coords[2];
}

bool node::operator!=(const node &n) const
{
	return !(*this == n);
}

void node::addTriangle(triangle *t)
{
	m_triangles.push_back(t);
}

void node::removeTriangle(triangle *t)
{
	m_triangles.remove(t);
}

triangle *node::getTriangle(node *n, const triangle *t) const
{
	std::list<triangle *> both, other;
	std::list<triangle *>::const_iterator it, it2, end, oBegin, oEnd;
	
	other = n -> m_triangles;
	other.remove(const_cast<triangle*>(t));
	other.remove(t -> parent());
	
	end = m_triangles.end();
	oBegin = other.begin();
	oEnd = other.end();
	for (it = m_triangles.begin(); it != end; ++it)
	{
		it2 = std::find(oBegin, oEnd, *it);
		if (it2 != oEnd) both.push_back(*it);
	}
	
	if (both.size() == 1) return both.front();
	else if (both.size() == 0) return 0;
	else
	{
		assert(both.size() == 2);
		int l1, l2;
		triangle *t1, *t2;
		
		t1 = both.front();
		l1 = t1 -> level();
		t2 = both.back();
		l2 = t2 -> level();
		assert(l1 != l2);
		if (l1 > l2) return t1;
		else return t2;
	}
}

void node::print() const
{
	printf("x:%f y:%f z:%f", m_coords[0], m_coords[1], m_coords[2]);
}

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <assert.h>
#include <stdio.h>

#include "node.h"
#include "triangle.h"

node::node(double x, double y, double z, const QColor &c)
{
	m_coords[0] = x;
	m_coords[2] = y;
	m_coords[1] = z;
	
	m_color[0] = (double)c.red() / 255;
	m_color[1] = (double)c.green() / 255;
	m_color[2] = (double)c.blue() / 255;
}

const double *node::color() const
{
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
	m_triangles.append(t);
}

triangle *node::getTriangle(node *n, triangle *t) const
{
	QValueList<triangle *> both, other;
	
	other = n -> triangles();
	other.remove(t);
	other.remove(t -> parent());
	
	QValueList<triangle *>::const_iterator it, it2;
	for (it = m_triangles.begin(); it != m_triangles.end(); ++it)
	{
		it2 = qFind(other.begin(), other.end(), *it);
		if (it2 != other.end()) both.append(*it);
	}
	
	if (both.count() == 1) return both.first();
	else if (both.count() == 0) return 0;
	else
	{
		assert(both.count() == 2);
		int l1, l2;
		triangle *t1, *t2;
		
		t1 = both.first();
		l1 = t1 -> level();
		t2 = both.last();
		l2 = t2 -> level();
		assert(l1 != l2);
		if (l1 > l2) return t1;
		else return t2;
	}
}

QValueList<triangle *> node::triangles()
{
	return m_triangles;
}

void node::print() const
{
	printf("x:%f y:%f z:%f", m_coords[0], m_coords[1], m_coords[2]);
}

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "diamond.h"
#include "diamondlist.h"
#include "triangle.h"
#include "trianglelist.h"

diamond::diamond(triangle *t1, triangle *t2) : m_t1(t1), m_t2(t2)
{
}

void diamond::clean()
{
	delete m_t1;
	delete m_t2;
}

void diamond::merge(triangleList *splitQueue, diamondList *mergeQueue)
{
	m_t1 -> deleteLeaves(splitQueue);
	m_t2 -> deleteLeaves(splitQueue);
	splitQueue -> insert(m_t1);
	splitQueue -> insert(m_t2);
	mergeQueue -> remove(this);
// 	delete this;
}

double diamond::priority() const
{
	if (m_t1 -> priority() > m_t2 -> priority()) return m_t1 -> priority();
	else return m_t2 -> priority();
}

triangle *diamond::t1() const
{
	return m_t1;
}

triangle *diamond::t2() const
{
	return m_t2;
}

void diamond::setOwnIterator(diamondListIterator it)
{
	m_it = it;
}

diamondListIterator diamond::ownIterator() const
{
	return m_it;
}

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "diamond.h"
#include "triangle.h"
 
diamond::diamond(triangle *t1, triangle *t2) : m_t1(t1), m_t2(t2)
{
}

triangle *diamond::t1() const
{
	return m_t1;
}

triangle *diamond::t2() const
{
	return m_t2;
}

void diamond::setOwnIterator(std::multimap<double, diamond*>::iterator it)
{
	m_it = it;
}

std::multimap<double, diamond*>::iterator diamond::ownIterator() const
{
	return m_it;
}

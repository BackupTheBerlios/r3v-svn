/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "diamondlist.h"
#include "diamond.h"
#include "triangle.h"

void diamondList::clear()
{
	m_diamonds.clear();
}

void diamondList::insert(triangle *t1, triangle *t2)
{
	diamond *d;
	
	d = new diamond(t1, t2);
	insert(d);
	t1 -> setDiamond(d);
	t2 -> setDiamond(d);
}

void diamondList::insert(diamond *d)
{
	double priority;
	diamondListIterator it;
	
	priority = d -> priority();
	it = m_diamonds.insert(std::pair<double, diamond*>(priority, d));
	d->setOwnIterator(it);
}

void diamondList::remove(diamond *d)
{
	m_diamonds.erase(d->ownIterator());
	d -> t1() -> setDiamond(0);
	d -> t2() -> setDiamond(0);
	delete d;
}

diamondListConstIterator diamondList::begin() const
{
	return m_diamonds.begin();
}

diamondListConstIterator diamondList::end() const
{
	return m_diamonds.end();
}

diamond *diamondList::first() const
{
	diamondListConstIterator it = m_diamonds.begin();
	return (*it).second;
}

int diamondList::count() const
{
	return m_diamonds.size();
}

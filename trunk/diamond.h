/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIAMOND_H
#define DIAMOND_H

#include "diamondlist.h"

class diamondList;
class r3vMap;
class triangle;
class triangleList;

class diamond
{
	public:
		diamond(triangle *t1, triangle *t2);
		void clean();
		
		void merge(triangleList *splitQueue, diamondList *mergeQueue);
		
		double priority() const;
		
		triangle *t1() const;
		triangle *t2() const;
		
		void setOwnIterator(diamondListIterator it);
		diamondListIterator ownIterator() const;
		
	private:
		triangle *m_t1, *m_t2;
		diamondListIterator m_it;
};

#endif

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FAKETRIANGLELIST_H
#define FAKETRIANGLELIST_H

#include <qvaluelist.h>

#include "faketriangle.h"

class fakeTriangleList
{
	public:
		void clear();
		
		void insert(const fakeTriangle &ft);
		void remove(triangle *t);
		
		const fakeTriangle& last() const;
		
	private:
		QValueList<fakeTriangle> m_triangles;
};

#endif

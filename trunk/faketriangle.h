/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FAKETRIANGLE_H
#define FAKETRIANGLE_H

class triangle;

class fakeTriangle
{
	public:
		fakeTriangle();
		fakeTriangle(triangle *t, double *modelViewMatrix);
		fakeTriangle(const fakeTriangle &ft);
		~fakeTriangle();
		
		triangle *operator*() const;
		bool operator<(const fakeTriangle &ft) const;
		
// 	private:
		triangle *m_t;
		double m_priority;
};

#endif

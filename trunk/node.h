/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef NODE_H
#define NODE_H

#include <qcolor.h>
#include <qvaluelist.h>

class triangle;

class node
{
	friend class r3vMap;
	
	public:
		const double *color() const;
		const double *coords() const;
		
		double getX() const;
		double getZ() const;
		double getHeight() const;
		
		bool operator==(const node &n) const;
		bool operator!=(const node &n) const;
		
		void addTriangle(triangle *t);
		void removeTriangle(triangle *t);
		triangle *getTriangle(node *n, triangle *t) const;
		QValueList<triangle *> triangles();
		
		void print() const;
		
	private:
		node(double x, double y, double z, int r, int g, int b);
	
		// 0 -> x
		// 1 -> height
		// 2 -> z
		double m_coords[3];
		
		// 0 -> r
		// 1 -> g
		// 2 -> b
		double m_color[3];
		
		QValueList<triangle *> m_triangles;
};

#endif

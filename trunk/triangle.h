/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <map>

#include <math.h>

#include <qstring.h>

#include "trianglelist.h"

class r3vMap;
class node;

class triangle
{
	public:
		triangle(r3vMap &m, node *apex, node *left, node *right, triangle *parentTriangle, QString n);
		~triangle();
		
		bool isLeaf() const;
		
		node *apex() const;
		node *leftVertex() const;
		node *rightVertex() const;
		
		triangle *leftTriangle() const;
		triangle *rightTriangle() const;
		triangle *parent() const;
		
		unsigned int level() const;
		
		void updateWedgie();
		
		void split(triangleList &splitQueue, double *modelViewMatrix);
		
		void print() const;
		
		double priority() const;
		void calcPriority(double *modelViewMatrix);
		
		void setOwnIterator(std::multimap<double, triangle*>::iterator it);
		std::multimap<double, triangle*>::iterator ownIterator() const;
	
	private:
		QString nom;  // TODO removeme
	
		r3vMap &m_map;
		
		node *m_apex;
		node *m_leftVertex;
		node *m_rightVertex;
		
		unsigned int m_level;
		
		double m_ownWedgie, m_wedgie;
		
		double m_priority;
		
		triangle *m_leftTriangle;
		triangle *m_rightTriangle;
		triangle *m_parentTriangle;
		std::multimap<double, triangle*>::iterator m_it;
};

#endif

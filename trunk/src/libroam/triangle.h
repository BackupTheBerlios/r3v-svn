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

#include <math.h>

#include "trianglelist.h"

class diamond;
class diamondList;
class frustum;
class node;
class r3vMap;

class triangle
{
	public:
		triangle(r3vMap &m, node *apex, node *left, node *right, triangle *parentTriangle);
		~triangle();
		
		enum FRUSTUMSTATUS {UNKNOWN, OUTSIDE, INSIDE, COMPLETELYINSIDE};
		
		bool isVisible() const;
		bool isLeaf() const;
		void deleteLeaves(triangleList *splitQueue);
		
		node *apex() const;
		node *leftVertex() const;
		node *rightVertex() const;
		
		triangle *leftTriangle() const;
		triangle *rightTriangle() const;
		triangle *parent() const;
		
		unsigned int level() const;
		
		void split(triangleList *splitQueue, diamondList *mergeQueue, const frustum &f);
		
		double priority() const;
		void calcPriority(const frustum &f);
		
		void updateMergeableStatus(diamondList *mergeQueue, triangle *baseTriangle);
		
		void setOwnIterator(triangleListIterator it);
		triangleListIterator ownIterator() const;
		
		void setDiamond(diamond *d);
		
		FRUSTUMSTATUS frustumStatus() const;
		void setFrustumStatus(FRUSTUMSTATUS s);
		
		triangle *getBaseTriangle() const;
	
	private:
		void setMergeable(bool mergeable, diamondList *mergeQueue, triangle *baseTriangle);
		void updateWedgie();
		
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
		triangleListIterator m_it;
		
		diamond *m_mergeableDiamond;
		FRUSTUMSTATUS m_status;
};

#endif

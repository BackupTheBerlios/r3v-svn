/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <float.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "diamondlist.h"
#include "map.h"
#include "node.h"
#include "triangle.h"

triangle::triangle(r3vMap &m, node *apex, node *left, node *right, triangle *parentTriangle, QString n) : nom(n), m_map(m), m_apex(apex), m_leftVertex(left), m_rightVertex(right), m_leftTriangle(0), m_rightTriangle(0), m_parentTriangle(parentTriangle), m_mergeableDiamond(0)
{
	assert(*apex != *left);
	assert(*apex != *right);
	assert(*left != *right);

	if (parentTriangle) m_level = parentTriangle -> m_level + 1;
	else m_level = 0;
	
	double currentHeight, realHeight;
	
// 	qDebug("NOMBRE: %s", n.latin1());
	
	currentHeight = (left -> getHeight() + right -> getHeight()) / 2;
// 	qDebug("Altura actual %f", currentHeight);
//  	qDebug("Pido la altura de %f %f", (left -> getX() + right -> getX()) / 2, (left -> getZ() + right -> getZ()) / 2);
	realHeight = m.height((left -> getX() + right -> getX()) / 2, (left -> getZ() + right -> getZ()) / 2);
//  	qDebug("Altura real %f", realHeight);
	m_ownWedgie = fabs(currentHeight - realHeight);
	m_wedgie = m_ownWedgie;
	
// 	qDebug("wedgie %f", m_wedgie);
	
	apex -> addTriangle(this);
	left -> addTriangle(this);
	right -> addTriangle(this);
}

triangle::~triangle()
{
	delete m_leftTriangle;
	delete m_rightTriangle;
}

bool triangle::isLeaf() const
{
	return !m_leftTriangle && !m_rightTriangle;
}

node *triangle::apex() const
{
	return m_apex;
}

node *triangle::leftVertex() const
{
	return m_leftVertex;
}

node *triangle::rightVertex() const
{
	return m_rightVertex;
}

triangle *triangle::leftTriangle() const
{
	return m_leftTriangle;
}

triangle *triangle::rightTriangle() const
{
	return m_rightTriangle;
}

triangle *triangle::parent() const
{
	return m_parentTriangle;
}

unsigned int triangle::level() const
{
	return m_level;
}

void triangle::updateWedgie()
{
	m_wedgie = m_ownWedgie + QMAX(m_leftTriangle -> m_wedgie, m_rightTriangle -> m_wedgie);
	if (m_parentTriangle) m_parentTriangle -> updateWedgie();
}

void triangle::split(triangleList &splitQueue, diamondList &mergeQueue, double *modelViewMatrix)
{
// 	assert(!m_leftTriangle);
// 	assert(!m_rightTriangle);
	
	splitQueue.remove(this);
	
	triangle *baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	if (baseTriangle && baseTriangle -> level() < m_level)
	{
		baseTriangle -> split(splitQueue, mergeQueue, modelViewMatrix);
		
		// our base triangle has changed
		baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	}
	
	double x, y;
	x = (m_leftVertex -> getX() + m_rightVertex -> getX()) / 2;
	y = (m_leftVertex -> getZ() + m_rightVertex -> getZ()) / 2;
	
	node *newApex = m_map.getNode(x, y);
	
	/*
	if (*newApex == *m_leftVertex)
	{
		printf("1\n");
	}
	else if (*m_apex == *newApex)
	{
		printf("2\n");
	}
	else if (*m_rightVertex == *m_leftVertex)
	{
		printf("3\n");
	}*/
	
	m_leftTriangle = new triangle(m_map, newApex, m_apex, m_leftVertex, this, nom+"L");
	m_rightTriangle = new triangle(m_map, newApex, m_rightVertex, m_apex, this, nom+"R");
	m_leftTriangle -> calcPriority(modelViewMatrix);
	m_rightTriangle -> calcPriority(modelViewMatrix);
	splitQueue.insert(m_leftTriangle);
	splitQueue.insert(m_rightTriangle);
	
	// baseTriangle -> m_leftTriangle to stop recurring splitting between base neighbours
	if (baseTriangle && !baseTriangle -> m_leftTriangle) baseTriangle -> split(splitQueue, mergeQueue, modelViewMatrix);
	
	if (m_parentTriangle) m_parentTriangle -> setMergeable(false, mergeQueue, baseTriangle);
	if (baseTriangle)
	{
		if (m_leftTriangle -> isLeaf() && m_rightTriangle -> isLeaf() && baseTriangle -> m_leftTriangle -> isLeaf() && baseTriangle -> m_rightTriangle -> isLeaf())
		{
			setMergeable(true, mergeQueue, baseTriangle);
		}
		else qDebug("NO PASA NUNCA");
	}
	
	updateWedgie();
}

void triangle::print() const
{
	/*printf("Apex ");
	m_apex->print();
	printf("\nLeftVertex ");
	m_leftVertex->print();
	printf("\nrightVertex ");
	m_rightVertex->print();
	printf("\n");*/
	QString l, r, p, b;
	if (m_leftTriangle) l = m_leftTriangle->nom.latin1();
	if (m_rightTriangle) r = m_rightTriangle->nom.latin1();
	if (m_parentTriangle) p = m_parentTriangle->nom.latin1();
// 	qDebug("%s left %s right %s padre %s base %s", nom.latin1(), l.latin1(), r.latin1(), p.latin1(), b.latin1());
	
	if (m_leftTriangle) m_leftTriangle->print();
	if (m_rightTriangle) m_rightTriangle->print();
}

double triangle::priority() const
{
	return m_priority;
}

void triangle::calcPriority(double *modelViewMatrix)
{
// 	qDebug("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", modelViewMatrix[0], modelViewMatrix[1], modelViewMatrix[2], modelViewMatrix[3], modelViewMatrix[4], modelViewMatrix[5], modelViewMatrix[6], modelViewMatrix[7], modelViewMatrix[8], modelViewMatrix[9], modelViewMatrix[10], modelViewMatrix[11], modelViewMatrix[12], modelViewMatrix[13], modelViewMatrix[14], modelViewMatrix[15]);

	double a, b, c, p1, q1, r1, p2, q2, r2, p3, q3, r3, d1, d2, d3, csq, r1sq, r2sq, r3sq;
	a = modelViewMatrix[0] * 0.0 + modelViewMatrix[4] * m_wedgie + modelViewMatrix[8] * 0.0;
	b = modelViewMatrix[1] * 0.0 + modelViewMatrix[5] * m_wedgie + modelViewMatrix[9] * 0.0;
	c = modelViewMatrix[2] * 0.0 + modelViewMatrix[6] * m_wedgie + modelViewMatrix[10] * 0.0;
	
	p1 = modelViewMatrix[0] * m_apex -> getX() + 
	     modelViewMatrix[4] * m_apex -> getHeight() + 
	     modelViewMatrix[8] * m_apex -> getZ() + 
	     modelViewMatrix[12];
	q1 = modelViewMatrix[1] * m_apex -> getX() + 
	     modelViewMatrix[5] * m_apex -> getHeight() + 
	     modelViewMatrix[9] * m_apex -> getZ() + 
	     modelViewMatrix[13];
	r1 = modelViewMatrix[2] * m_apex -> getX() + 
	     modelViewMatrix[6] * m_apex -> getHeight() + 
	     modelViewMatrix[10] * m_apex -> getZ() + 
	     modelViewMatrix[14];
	
// 	qDebug("Triangulo %s", nom.latin1());
// 	qDebug("Apex %f %f %f en camera-space %f %f %f", m_apex -> getX(), m_apex -> getHeight(), m_apex -> getZ(), p1, q1, r1);
	
	p2 = modelViewMatrix[0] * m_rightVertex -> getX() + 
	     modelViewMatrix[4] * m_rightVertex -> getHeight() + 
	     modelViewMatrix[8] * m_rightVertex -> getZ() + 
	     modelViewMatrix[12];
	q2 = modelViewMatrix[1] * m_rightVertex -> getX() + 
	     modelViewMatrix[5] * m_rightVertex -> getHeight() + 
	     modelViewMatrix[9] * m_rightVertex -> getZ() + 
	     modelViewMatrix[13];
	r2 = modelViewMatrix[2] * m_rightVertex -> getX() + 
	     modelViewMatrix[6] * m_rightVertex -> getHeight() + 
	     modelViewMatrix[10] * m_rightVertex -> getZ() + 
	     modelViewMatrix[14];
// 	qDebug("RV %f %f %f en camera-space %f %f %f", m_rightVertex -> getX(), m_rightVertex -> getHeight(), m_rightVertex -> getZ(), p2, q2, r2);
	
	p3 = modelViewMatrix[0] * m_leftVertex -> getX() + 
	     modelViewMatrix[4] * m_leftVertex -> getHeight() + 
	     modelViewMatrix[8] * m_leftVertex -> getZ() + 
	     modelViewMatrix[12];
	q3 = modelViewMatrix[1] * m_leftVertex -> getX() + 
	     modelViewMatrix[5] * m_leftVertex -> getHeight() + 
	     modelViewMatrix[9] * m_leftVertex -> getZ() + 
	     modelViewMatrix[13];
	r3 = modelViewMatrix[2] * m_leftVertex -> getX() + 
	     modelViewMatrix[6] * m_leftVertex -> getHeight() + 
	     modelViewMatrix[10] * m_leftVertex -> getZ() + 
	     modelViewMatrix[14];
// 	qDebug("LV %f %f %f en camera-space %f %f %f", m_leftVertex -> getX(), m_leftVertex -> getHeight(), m_leftVertex -> getZ(), p3, q3, r3);
	
	csq = c * c;
	r1sq = r1 * r1;
	r2sq = r2 * r2;
	r3sq = r3 * r3;
	
	if (r1sq > csq) d1 = 2 / (r1sq - csq) * sqrt(pow(a * r1 - c * p1, 2) + pow(b * r1 - c * q1, 2));
	else d1 = DBL_MIN;
	
	if (r2sq > csq) d2 = 2 / (r2sq - csq) * sqrt(pow(a * r2 - c * p2, 2) + pow(b * r2 - c * q2, 2));
	else d2 = DBL_MIN;
	
	if (r3sq > csq) d3 = 2 / (r3sq - csq) * sqrt(pow(a * r3 - c * p3, 2) + pow(b * r3 - c * q3, 2));
	else d3 = DBL_MIN;
	
// 	qDebug("%f %f %f", d1, d2, d3);
	m_priority = QMAX(d1, d2);
	m_priority = QMAX(m_priority, d3);
	
	// TODO usar un define o algo ese 1.0 es el ZNear
// 	if (r1 >= -1.0 || r2 >= -1.0 || r3 >= -1.0) m_priority = DBL_MIN;
	
// 	if (d1 == d2 && d2 == d3 && d3 == 0) m_priority = DBL_MIN;
// 	qDebug("wedgie  %f prio %f", m_wedgie, m_priority);
}

void triangle::setMergeable(bool mergeable, diamondList &mergeQueue, triangle *baseTriangle)
{
	if (mergeable)
	{
		if (!m_mergeableDiamond) mergeQueue.insert(this, baseTriangle);
	}
	else
	{
		if (m_mergeableDiamond) mergeQueue.remove(m_mergeableDiamond);
	}
}

void triangle::setOwnIterator(std::multimap<double, triangle*>::iterator it)
{
	m_it = it;
}

std::multimap<double, triangle*>::iterator triangle::ownIterator() const
{
	return m_it;
}

void triangle::setDiamond(diamond *d)
{
	m_mergeableDiamond = d;
}

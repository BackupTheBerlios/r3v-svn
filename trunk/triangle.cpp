/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "map.h"
#include "node.h"
#include "triangle.h"

triangle::triangle(map &m, node *apex, node *left, node *right, triangle *parentTriangle, QString n) : nom(n), m_map(m), m_apex(apex), m_leftVertex(left), m_rightVertex(right), m_leftTriangle(0), m_rightTriangle(0), m_parentTriangle(parentTriangle)
{
	assert(*apex != *left);
	assert(*apex != *right);
	assert(*left != *right);
	
	if (parentTriangle) m_level = parentTriangle -> m_level + 1;
	else m_level = 0;
	
	double currentHeight, realHeight;
	
	qDebug("NOMBRE: %s", n.latin1());
	
	currentHeight = (left -> getHeight() + right -> getHeight()) / 2;
	qDebug("Altura actual %f", currentHeight);
 	qDebug("Pido la altura de %f %f", (left -> getX() + right -> getX()) / 2, (left -> getZ() + right -> getZ()) / 2);
	realHeight = m.height((left -> getX() + right -> getX()) / 2, (left -> getZ() + right -> getZ()) / 2);
 	qDebug("Altura real %f", realHeight);
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

double triangle::wedgie() const
{
	return m_wedgie;
}

void triangle::updateWedgie()
{
	m_wedgie = m_ownWedgie + QMAX(m_leftTriangle -> wedgie(), m_rightTriangle -> wedgie());
	if (m_parentTriangle) m_parentTriangle -> updateWedgie();
}

void triangle::split(fakeTriangleList &splitQueue, double *modelViewMatrix)
{
	assert(!m_leftTriangle);
	assert(!m_rightTriangle);
	
	splitQueue.remove(this);
	
	triangle *baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	
	if (baseTriangle && baseTriangle -> level() < m_level)
	{
// 		qDebug("SPLITING %s", baseTriangle -> nom.latin1());
		baseTriangle -> split(splitQueue, modelViewMatrix);
		
		// our base triangle has changed
		baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	}
	
	double x, y;
	x = (m_leftVertex -> getX() + m_rightVertex -> getX()) / 2;
	y = (m_leftVertex -> getZ() + m_rightVertex -> getZ()) / 2;
	
	node *newApex = m_map.getNode(x, y);
	m_leftTriangle = new triangle(m_map, newApex, m_apex, m_leftVertex, this, nom+"L");
	m_rightTriangle = new triangle(m_map, newApex, m_rightVertex, m_apex, this, nom+"R");
	splitQueue.insert(fakeTriangle(m_leftTriangle, modelViewMatrix));
	splitQueue.insert(fakeTriangle(m_rightTriangle, modelViewMatrix));
	
	// baseTriangle -> m_leftTriangle to stop recurring splitting between base neighbours
	if (baseTriangle && !baseTriangle -> m_leftTriangle) baseTriangle -> split(splitQueue, modelViewMatrix);
	
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
	qDebug("%s left %s right %s padre %s base %s", nom.latin1(), l.latin1(), r.latin1(), p.latin1(), b.latin1());
	
	if (m_leftTriangle) m_leftTriangle->print();
	if (m_rightTriangle) m_rightTriangle->print();
}

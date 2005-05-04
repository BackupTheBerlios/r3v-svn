/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <algorithm>

#include <float.h>

#include <assert.h>
#include <math.h>

#include "diamondlist.h"
#include "frustum.h"
#include "map.h"
#include "node.h"
#include "triangle.h"

triangle::triangle(r3vMap &m, node *apex, node *left, node *right, triangle *parentTriangle) : m_map(m), m_apex(apex), m_leftVertex(left), m_rightVertex(right), m_leftTriangle(0), m_rightTriangle(0), m_parentTriangle(parentTriangle), m_mergeableDiamond(0), m_status(UNKNOWN)
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
	m_map.addTriangles(-1);
	if (!m_leftTriangle) m_map.addLeaves(-1);
	
	m_apex -> removeTriangle(this);
	m_leftVertex -> removeTriangle(this);
	m_rightVertex -> removeTriangle(this);
	
	delete m_leftTriangle;
	delete m_rightTriangle;
}

bool triangle::isVisible() const
{
	return m_status == INSIDE || m_status == COMPLETELYINSIDE;
}

bool triangle::isLeaf() const
{
	return !m_leftTriangle && !m_rightTriangle;
}

void triangle::deleteLeaves(triangleList *splitQueue)
{
	if (m_leftTriangle)
	{
		splitQueue -> remove(m_leftTriangle);
		delete m_leftTriangle;
		m_leftTriangle = 0;
		splitQueue -> remove(m_rightTriangle);
		delete m_rightTriangle;
		m_rightTriangle = 0;
		m_map.addLeaves(1);
	}
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
	m_wedgie = m_ownWedgie + std::max(m_leftTriangle -> m_wedgie, m_rightTriangle -> m_wedgie);
	if (m_parentTriangle) m_parentTriangle -> updateWedgie();
}

void triangle::split(triangleList *splitQueue, diamondList *mergeQueue, double *modelViewMatrix, const frustum &f)
{
	assert(!m_leftTriangle);
	assert(!m_rightTriangle);
	
	splitQueue->remove(this);
	
	triangle *baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	if (baseTriangle && baseTriangle -> level() < m_level)
	{
		baseTriangle -> split(splitQueue, mergeQueue, modelViewMatrix, f);
		
		// our base triangle has changed
		baseTriangle = m_rightVertex -> getTriangle(m_leftVertex, this);
	}
	
	double x, y;
	x = (m_leftVertex -> getX() + m_rightVertex -> getX()) / 2;
	y = (m_leftVertex -> getZ() + m_rightVertex -> getZ()) / 2;
	
	node *newApex = m_map.getNode(x, y);
	
	m_leftTriangle = new triangle(m_map, newApex, m_apex, m_leftVertex, this);
	m_rightTriangle = new triangle(m_map, newApex, m_rightVertex, m_apex, this);
	f.setTriangleStatus(m_leftTriangle);
	f.setTriangleStatus(m_rightTriangle);
	
	// TODO place in other function?
	m_map.addTriangles(2);
	m_map.addLeaves(1);
	m_leftTriangle -> calcPriority(modelViewMatrix);
	m_rightTriangle -> calcPriority(modelViewMatrix);
	splitQueue->insert(m_leftTriangle);
	splitQueue->insert(m_rightTriangle);
	
	// baseTriangle -> m_leftTriangle to stop recurring splitting between base neighbours
	if (baseTriangle && !baseTriangle -> m_leftTriangle) baseTriangle -> split(splitQueue, mergeQueue, modelViewMatrix, f);
	
	if (m_parentTriangle) m_parentTriangle -> setMergeable(false, mergeQueue, baseTriangle);
	if (baseTriangle)
	{
		assert(m_leftTriangle -> isLeaf());
		assert(m_rightTriangle -> isLeaf());
		assert(baseTriangle -> m_leftTriangle -> isLeaf());
		assert(baseTriangle -> m_rightTriangle -> isLeaf());
		if (m_leftTriangle -> isLeaf() && m_rightTriangle -> isLeaf() && baseTriangle -> m_leftTriangle -> isLeaf() && baseTriangle -> m_rightTriangle -> isLeaf())
		{
			setMergeable(true, mergeQueue, baseTriangle);
		}
	}
	
	updateWedgie();
}

double triangle::priority() const
{
	return m_priority;
}

void triangle::calcPriority(double *modelViewMatrix)
{
	assert(m_status != UNKNOWN);
	if (!isVisible())
	{
		m_priority = DBL_MIN;
		return;
	}

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
	
	if (r1sq > csq + 0.01) d1 = 2 / (r1sq - csq) * sqrt(pow(a * r1 - c * p1, 2) + pow(b * r1 - c * q1, 2));
	else d1 = DBL_MIN;
	
	if (r2sq > csq + 0.01) d2 = 2 / (r2sq - csq) * sqrt(pow(a * r2 - c * p2, 2) + pow(b * r2 - c * q2, 2));
	else d2 = DBL_MIN;
	
	if (r3sq > csq + 0.01) d3 = 2 / (r3sq - csq) * sqrt(pow(a * r3 - c * p3, 2) + pow(b * r3 - c * q3, 2));
	else d3 = DBL_MIN;
		
// 	printf("%f %f %f\n", d1, d2, d3);
// 	printf("%f %f %f\n", r1, r2, r3);
	m_priority = std::max(d1, d2);
	m_priority = std::max(m_priority, d3);
	
	// TODO HACE FALTA AHORA QUE HACEMOS VIEW FRUSTUM CULLING??? usar un define o algo ese 1.0 es el ZNear
	if (r1sq < 1.0 || r2sq < 1.0 || r3sq < 1.0) m_priority = DBL_MIN;
	
// 	if (d1 == d2 && d2 == d3 && d3 == 0) m_priority = DBL_MIN;
// 	qDebug("wedgie  %f prio %f", m_wedgie, m_priority);
}

void triangle::setMergeable(bool mergeable, diamondList *mergeQueue, triangle *baseTriangle)
{
	if (mergeable)
	{
		if (!m_mergeableDiamond) mergeQueue->insert(this, baseTriangle);
	}
	else
	{
		if (m_mergeableDiamond) mergeQueue->remove(m_mergeableDiamond);
	}
}

void triangle::setOwnIterator(triangleListIterator it)
{
	m_it = it;
}

triangleListIterator triangle::ownIterator() const
{
	return m_it;
}

void triangle::setDiamond(diamond *d)
{
	m_mergeableDiamond = d;
}

triangle::FRUSTUMSTATUS triangle::frustumStatus() const
{
	return m_status;
}

void triangle::setFrustumStatus(FRUSTUMSTATUS s)
{
	m_status = s;
}

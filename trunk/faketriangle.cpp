/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "faketriangle.h"
#include "node.h"
#include "triangle.h"
 
fakeTriangle::fakeTriangle()
{
}

fakeTriangle::fakeTriangle(triangle *t, double *modelViewMatrix) : m_t(t)
{
/*	m_priority = t->wedgie();
	return;*/
	
	float a, b, c, p1, q1, r1, p2, q2, r2, p3, q3, r3, d1, d2, d3;
	a = modelViewMatrix[0] * 0.0 + modelViewMatrix[4] * t -> wedgie() + modelViewMatrix[8] * 0.0;
	b = modelViewMatrix[1] * 0.0 + modelViewMatrix[5] * t -> wedgie() + modelViewMatrix[9] * 0.0;
	c = modelViewMatrix[2] * 0.0 + modelViewMatrix[6] * t -> wedgie() + modelViewMatrix[10] * 0.0;
	
	p1 = modelViewMatrix[0] * t -> apex() -> getX() + 
	     modelViewMatrix[4] * t -> apex() -> getHeight() + 
	     modelViewMatrix[8] * t -> apex() -> getZ() + 
	     modelViewMatrix[12];
	q1 = modelViewMatrix[1] * t -> apex() -> getX() + 
	     modelViewMatrix[5] * t -> apex() -> getHeight() + 
	     modelViewMatrix[9] * t -> apex() -> getZ() + 
	     modelViewMatrix[13];
	r1 = modelViewMatrix[2] * t -> apex() -> getX() + 
	     modelViewMatrix[6] * t -> apex() -> getHeight() + 
	     modelViewMatrix[10] * t -> apex() -> getZ() + 
	     modelViewMatrix[14];
	
	p2 = modelViewMatrix[0] * t -> rightVertex() -> getX() + 
	     modelViewMatrix[4] * t -> rightVertex() -> getHeight() + 
	     modelViewMatrix[8] * t -> rightVertex() -> getZ() + 
	     modelViewMatrix[12];
	q2 = modelViewMatrix[1] * t -> rightVertex() -> getX() + 
	     modelViewMatrix[5] * t -> rightVertex() -> getHeight() + 
	     modelViewMatrix[9] * t -> rightVertex() -> getZ() + 
	     modelViewMatrix[13];
	r2 = modelViewMatrix[2] * t -> rightVertex() -> getX() + 
	     modelViewMatrix[6] * t -> rightVertex() -> getHeight() + 
	     modelViewMatrix[10] * t -> rightVertex() -> getZ() + 
	     modelViewMatrix[14];
	
	p3 = modelViewMatrix[0] * t -> leftVertex() -> getX() + 
	     modelViewMatrix[4] * t -> leftVertex() -> getHeight() + 
	     modelViewMatrix[8] * t -> leftVertex() -> getZ() + 
	     modelViewMatrix[12];
	q3 = modelViewMatrix[1] * t -> leftVertex() -> getX() + 
	     modelViewMatrix[5] * t -> leftVertex() -> getHeight() + 
	     modelViewMatrix[9] * t -> leftVertex() -> getZ() + 
	     modelViewMatrix[13];
	r3 = modelViewMatrix[2] * t -> leftVertex() -> getX() + 
	     modelViewMatrix[6] * t -> leftVertex() -> getHeight() + 
	     modelViewMatrix[10] * t -> leftVertex() -> getZ() + 
	     modelViewMatrix[14];
	
	d1 = 2 / (r1 * r1 - c * c) * sqrt(pow(a * r1 - c * p1, 2) + pow(b * r1 - c * q1, 2));
	d2 = 2 / (r2 * r2 - c * c) * sqrt(pow(a * r2 - c * p2, 2) + pow(b * r2 - c * q2, 2));
	d3 = 2 / (r3 * r3 - c * c) * sqrt(pow(a * r3 - c * p3, 2) + pow(b * r3 - c * q3, 2));
	qDebug("%f %f %f", d1, d2, d3);
	m_priority = QMAX(d1, d2);
	m_priority = QMAX(m_priority, d3);
	qDebug("Creo %s wedgie %f prio %f", t->nom.latin1(), t->wedgie(), m_priority);
}

fakeTriangle::~fakeTriangle()
{
}

fakeTriangle::fakeTriangle(const fakeTriangle &ft) : m_t(ft.m_t), m_priority(ft.m_priority)
{
}

triangle *fakeTriangle::operator*() const
{
	return m_t;
}

bool fakeTriangle::operator<(const fakeTriangle &ft) const
{
	return m_priority < ft.m_priority;
}

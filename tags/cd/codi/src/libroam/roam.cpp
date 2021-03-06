/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "GL/gl.h"
#include "GL/glu.h"

#include "diamond.h"
#include "frustum.h"
#include "map.h"
#include "node.h"
#include "roam.h"
#include "observer.h"
#include "parser.h"

ROAM::ROAM(int leaves) : m_map(0), m_observer(0), m_splitQueue(0), m_mergeQueue(0), m_leaves(leaves)
{
}

ROAM::~ROAM()
{
	clean();
}

void ROAM::clean()
{
	delete m_map;
	m_map = 0;
	delete m_observer;
	m_observer = 0;
	delete m_splitQueue;
	m_splitQueue = 0;
	if (m_mergeQueue)
	{
		m_mergeQueue -> clean();
		delete m_mergeQueue;
	}
	m_mergeQueue = 0;
}

ROAM::Error ROAM::open(const std::string &file)
{
	diamond *d;
	r3vParser *parser;
	
	if (m_map) clean();
	
	if (file.substr(file.length() - 4, 4) == ".dem") parser = new DEMParser;
	else if (file.substr(file.length() - 3, 3) == ".pm") parser = new PMParser;
	else return unknownFormat;

	m_map = parser -> parse(file);
	if (!m_map) return openingError;
	delete parser;
	
	m_map -> calcAmplitude();
	
	d = m_map -> baseDiamond();

	int size = m_map -> size();
	const float r = 1.75;
	const float degX = M_PI / 4;
	// si hubiera 2 columnas, la 0 y la 1, no querriamos estar en 1 sino en 0.5
	m_observer = new observer((size - 1) / 2.0, m_map -> midHeight() + size * (r - 0.5) * tan(degX), r * size, degX, 0, size / 150.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float ox, oy, oz, vrpx, vrpy, vrpz;
	m_observer -> position(&ox, &oy, &oz);
	m_observer -> vrp(&vrpx, &vrpy, &vrpz);
	gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);
	
	m_splitQueue = new triangleList();
	m_mergeQueue = new diamondList();
	

	frustum f = getFrustum();
	f.setTriangleStatus(d -> t1());
	f.setTriangleStatus(d -> t2());
	d -> t1() -> calcPriority(f);
	d -> t2() -> calcPriority(f);
	m_splitQueue -> insert(d -> t1());
	m_splitQueue -> insert(d -> t2());
	
	return OK;
}

void ROAM::close()
{
	clean();
}

bool ROAM::hasMap()
{
	return m_map;
}

void ROAM::paint()
{
	triangleList *newSplitQueue;
	triangleListConstIterator sqIt, sqEndIt;
	triangle *t;
	diamondList *newMergeQueue;
	diamondListConstIterator mqIt, mqEndIt;
	diamond *d;
		
	float ox, oy, oz, vrpx, vrpy, vrpz;
	m_observer -> position(&ox, &oy, &oz);
	m_observer -> vrp(&vrpx, &vrpy, &vrpz);
	gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);
	
	
	frustum f = getFrustum();
	
	d = m_map -> baseDiamond();
	f.setTriangleStatus(d -> t1());
	f.setTriangleStatus(d -> t2());
	
	newSplitQueue = new triangleList();
	sqEndIt = m_splitQueue -> end();
	for (sqIt = m_splitQueue -> begin(); sqIt != sqEndIt; ++sqIt)
	{
		t = (*sqIt).second;
		t -> calcPriority(f);
		newSplitQueue->insert(t);
	}
	delete m_splitQueue;
	m_splitQueue = newSplitQueue;
	
	newMergeQueue = new diamondList();
	mqEndIt = m_mergeQueue -> end();
	for (mqIt = m_mergeQueue -> begin(); mqIt != mqEndIt; ++mqIt)
	{
		d = (*mqIt).second;
		d -> t1() -> calcPriority(f);
		if (d -> t2()) d -> t2() -> calcPriority(f);
		newMergeQueue->insert(d);
	}

	delete m_mergeQueue;
	m_mergeQueue = newMergeQueue;
	
	while (m_map -> leaves() < m_leaves ||
	       (m_splitQueue -> last() -> priority() > m_mergeQueue -> first() -> priority()))
	{
		if (m_map -> leaves() < m_leaves)
		{
			t = m_splitQueue -> last();
			t -> split(m_splitQueue, m_mergeQueue, f);
		}
		else
		{
			d = m_mergeQueue -> first();
			d -> merge(m_splitQueue, m_mergeQueue);
		}
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	paintTriangle(m_map -> baseDiamond() -> t1(), false);
	paintTriangle(m_map -> baseDiamond() -> t2(), false);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	glBegin(GL_TRIANGLES);
	paintTriangle(m_map -> baseDiamond() -> t1(), true);
	paintTriangle(m_map -> baseDiamond() -> t2(), true);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void ROAM::moveObserverForward()
{
	m_observer -> forward();
}

void ROAM::moveObserverBackward()
{
	m_observer -> backward();
}

void ROAM::moveObserverLeft()
{
	m_observer -> left();
}

void ROAM::moveObserverRight()
{
	m_observer -> right();
}

void ROAM::moveObserverUp()
{
	m_observer -> up();
}

void ROAM::moveObserverDown()
{
	m_observer -> down();
}

void ROAM::observerAngles(float *x, float *y) const
{
	m_observer -> angles(x, y);
}

void ROAM::rotateObserver(float x, float y)
{
	m_observer -> rotate(x, y);
}

float ROAM::observerStep() const
{
	m_observer -> step();
}

void ROAM::setObserverStep(float step)
{
	m_observer -> setStep(step);
}

void ROAM::observerPosition(float *x, float *y, float *z) const
{
	m_observer -> position(x, y, z);
}

void ROAM::setObserverPosition(float x, float y, float z)
{
	m_observer -> setPosition(x, y, z);
}

int ROAM::leaves() const
{
	return m_leaves;
}

void ROAM::setLeaves(int n)
{
	m_leaves = n;
}

frustum ROAM::getFrustum() const
{
	double modelViewMatrix[16], projectionMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	return frustum(projectionMatrix, modelViewMatrix);
}

void ROAM::paintTriangle(const triangle *t, bool color) const
{
	if (t -> isLeaf())
	{
		node *nodes[3];
		nodes[0] = t -> apex();
		nodes[1] = t -> rightVertex();
		nodes[2] = t -> leftVertex();
		
		if (!color) glColor3d(0.0, 0.0, 0.0);
		for (int i = 0; i < 3; i++)
		{
			if (color) glColor3dv(nodes[i] -> color());
			glVertex3dv(nodes[i] -> coords());
		}
	}
	else
	{
		paintTriangle(t -> leftTriangle(), color);
		paintTriangle(t -> rightTriangle(), color);
	}
}

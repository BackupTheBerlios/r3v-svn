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
#include "map.h"
#include "node.h"
#include "roam.h"
#include "observer.h"
#include "parser.h"

ROAM::ROAM() : m_map(0), m_observer(0), m_splitQueue(0), m_mergeQueue(0)
{
}

int ROAM::open(const std::string &file)
{
	diamond *d;
	
	if (m_map) return 1;
	
	if (file.substr(file.length() - 4, 4) == ".dem")
	{
		DEMParser dp;
		m_map = dp.parse(file);
		if (!m_map) return 2;
	}
	else return 3;
	
	m_map->square();
	
	d = m_map -> baseDiamond();

	int size = m_map -> columns();
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
	
	double modelViewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	d -> t1() -> calcPriority(modelViewMatrix);
	d -> t2() -> calcPriority(modelViewMatrix);
	m_splitQueue -> insert(d -> t1());
	m_splitQueue -> insert(d -> t2());
	
/*	triangle *t;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	for (int kk = 0; kk < 500; kk++)
	{
		t = m_splitQueue -> last();
		t -> split(m_splitQueue, m_mergeQueue, modelViewMatrix);
	}*/
	return 0;
}

ROAM::~ROAM()
{
	delete m_map;
	delete m_observer;
	delete m_splitQueue;
	delete m_mergeQueue;
}

void ROAM::paint()
{
	triangleList *newSplitQueue;
	triangleListConstIterator sqIt, sqEndIt;
	triangle *t;
	diamondList *newMergeQueue;
	diamondListConstIterator mqIt, mqEndIt;
	diamond *d;
	double modelViewMatrix[16];
	
	float ox, oy, oz, vrpx, vrpy, vrpz;
	m_observer -> position(&ox, &oy, &oz);
	m_observer -> vrp(&vrpx, &vrpy, &vrpz);
	gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);
	
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	
	newSplitQueue = new triangleList();
	sqEndIt = m_splitQueue -> end();
	for (sqIt = m_splitQueue -> begin(); sqIt != sqEndIt; ++sqIt)
	{
		t = (*sqIt).second;
		t -> calcPriority(modelViewMatrix);
		newSplitQueue->insert(t);
	}
	delete m_splitQueue;
	m_splitQueue = newSplitQueue;
	
	newMergeQueue = new diamondList();
	mqEndIt = m_mergeQueue -> end();
	for (mqIt = m_mergeQueue -> begin(); mqIt != mqEndIt; ++mqIt)
	{
		d = (*mqIt).second;
		d -> t1() -> calcPriority(modelViewMatrix);
		d -> t2() -> calcPriority(modelViewMatrix);
		newMergeQueue->insert(d);
	}
	delete m_mergeQueue;
	m_mergeQueue = newMergeQueue;
	
// 	while (m_map -> leaves() < 5000 ||
// 	       m_splitQueue -> last() -> priority() > m_mergeQueue -> first() -> priority())
// 	{
// 		if (m_map -> leaves() < 5000)
// 		{
// 			t = m_splitQueue -> last();
// 			t -> split(m_splitQueue, m_mergeQueue, modelViewMatrix);
// 		}
// 		else
// 		{
// 			d = m_mergeQueue -> first();
// 			d -> merge(m_splitQueue, m_mergeQueue);
// 		}
// 	}
	

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

// 	if (m_FPSEnabled)
// 	{
// 		if (m_lastFPS != 0)
// 		{
// 			glColor3f(1,1,1);
// 			renderText(0, m_fontHeight, QString("%1 FPS").arg(m_lastFPS));
// 		}
// 		
// 		QTime t = QTime::currentTime();
// 		m_newFPSSum += m_lastTime.msecsTo(t);
// 		m_FPSTimes++;
// 		m_lastTime = t;


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

void ROAM::rotateObserver(float x, float y)
{
	m_observer -> rotate(x, y);
}

void ROAM::splitOne()
{
	double modelViewMatrix[16];
	triangle *t;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	t = m_splitQueue->last();
	t -> split(m_splitQueue, m_mergeQueue, modelViewMatrix);
}

void ROAM::mergeOne()
{
	diamond *d;
	d = m_mergeQueue->first();
	d -> merge(m_splitQueue, m_mergeQueue);
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

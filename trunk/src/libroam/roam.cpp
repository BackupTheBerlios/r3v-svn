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
	
	triangle *t;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	for (int kk = 0; kk < 500; kk++)
	{
		t = m_splitQueue -> last();
		t -> split(m_splitQueue, m_mergeQueue, modelViewMatrix);
	}
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
	float ox, oy, oz, vrpx, vrpy, vrpz;

	m_observer -> position(&ox, &oy, &oz);
	m_observer -> vrp(&vrpx, &vrpy, &vrpz);
		
	gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);

// 		triangleList *newSplitQueue;
// 		triangleListConstIterator sqIt, sqEndIt;
// 		triangle *t;
// 		diamondList *newMergeQueue;
// 		diamondListConstIterator mqIt, mqEndIt;
// 		diamond *d;
// 		double modelViewMatrix[16];
// 		
// 		glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
// 		
// 		newSplitQueue = new triangleList();
// 		sqEndIt = splitQueue -> end();
// 		for (sqIt = splitQueue -> begin(); sqIt != sqEndIt; ++sqIt)
// 		{
// 			t = (*sqIt).second;
// 			t -> calcPriority(modelViewMatrix);
// 			newSplitQueue->insert(t);
// 		}
// 		delete splitQueue;
// 		splitQueue = newSplitQueue;
// 		
// 		newMergeQueue = new diamondList();
// 		mqEndIt = mergeQueue -> end();
// 		for (mqIt = mergeQueue -> begin(); mqIt != mqEndIt; ++mqIt)
// 		{
// 			d = (*mqIt).second;
// 			d -> t1() -> calcPriority(modelViewMatrix);
// 			d -> t2() -> calcPriority(modelViewMatrix);
// 			newMergeQueue->insert(d);
// 		}
// 		delete mergeQueue;
// 		mergeQueue = newMergeQueue;


// // 		for (int kk = 0; kk < 1000; kk++)
// 		while (m_map -> leaves() < 5000 || m_map -> leaves() > 5500 || 
// 		       splitQueue -> last() -> priority() > mergeQueue -> first() -> priority())
// 		{
// 			
// 			if (m_map -> leaves() < 5000)
// 			{
// 				t = splitQueue -> last();
// 				t -> split(splitQueue, mergeQueue, modelViewMatrix);
// 			}
// 			else
// 			{
// 				d = mergeQueue -> first();
// 				d -> merge(splitQueue, mergeQueue);
// 			}
// 		}
// 		updateGL();
// 		qDebug("Triangulos totales %d\n", m_map -> triangles());
// 		qDebug("Hojas totales %d\n", m_map -> leaves());	
	
		glBegin(GL_TRIANGLES);
		paintTriangle(m_map -> baseDiamond() -> t1());
		paintTriangle(m_map -> baseDiamond() -> t2());
		glEnd();
		
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
// 	}
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

void ROAM::paintTriangle(const triangle *t) const
{
	if (t -> isLeaf())
	{
		if (t -> m_parentTriangle && t -> m_parentTriangle -> m_mergeableDiamond) glColor3d(1.0, 1.0, 1.0);
		else glColor3dv(t -> apex() -> color());
		glVertex3dv(t -> apex() -> coords());
		if (t -> m_parentTriangle && t -> m_parentTriangle -> m_mergeableDiamond) glColor3d(1.0, 1.0, 1.0);
		else glColor3dv(t -> rightVertex() -> color());
		glVertex3dv(t -> rightVertex() -> coords());
		if (t -> m_parentTriangle&& t -> m_parentTriangle -> m_mergeableDiamond) glColor3d(1.0, 1.0, 1.0);
		else glColor3dv(t -> leftVertex() -> color());
		glVertex3dv(t -> leftVertex() -> coords());
	}
	else
	{
		paintTriangle(t -> leftTriangle());
		paintTriangle(t -> rightTriangle());
	}
}

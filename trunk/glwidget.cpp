/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <math.h>

#include <qapplication.h> 
#include <qcursor.h>
#include <qdesktopwidget.h> 
#include <qfiledialog.h>
#include <qfontmetrics.h>
#include <qpopupmenu.h>
#include <qtimer.h>

#include "diamond.h"
#include "glwidget.h"
#include "map.h"
#include "node.h"
#include "observer.h"
#include "parser.h"
#include "triangle.h"
#include "faketrianglelist.h"

glWidget::glWidget(QWidget *parent) : QGLWidget(parent), m_map(0), m_diamond(0), m_fromPopup(false), m_FPSEnabled(true)
{
	setMouseTracking(true);
	
/*	QTimer *t = new QTimer(this);
	connect(t, SIGNAL(timeout()), this, SLOT(updateGL()));
	t -> start(40);
	
	m_FPSTimer = new QTimer(this);
	connect(m_FPSTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
	initFPSTimer();
	
	m_fontHeight = QFontMetrics(font()).height();
	m_lastTime = QTime::currentTime();*/
	
	showFullScreen();
	
	openMap();
}

glWidget::~glWidget()
{
	closeMap();
}

void glWidget::resizeGL(int width, int height)
{
	float aspect = (float)width / height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, aspect, 1.0, 8000.0);
}

void glWidget::initializeGL()
{
// 	glClearColor(0, 0, 0, 0);
//  	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void glWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

// 	qDebug("%f %f %f", m_posX, m_posY, m_posZ);

	if (m_map)
	{
		float ox, oy, oz, vrpx, vrpy, vrpz;
	
		m_observer -> position(&ox, &oy, &oz);
		m_observer -> vrp(&vrpx, &vrpy, &vrpz);
	
		gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);
// 	qDebug("%f %f %f 0 1 0", vrpx-ox, vrpy-oy, vrpz-oz);

/*	int side = m_map -> columns();
	glBegin(GL_POINTS);
	for (int x = 0; x < side; x++)
	{
		for (int y = 0; y < side; y++)
		{
			
			glVertex3f(x, (float)m_map -> height(x, y), y);
		}
	}*/
		glBegin(GL_TRIANGLES);
		paintTriangle(m_diamond -> t1());
		paintTriangle(m_diamond -> t2());
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
	swapBuffers();
}

void glWidget::keyPressEvent(QKeyEvent *e)
{
	double modelViewMatrix[16];
	
	switch(e->key())
	{
		case Key_Up:
		case Key_W:
			if (m_map) m_observer -> forward();
		break;
		
		case Key_Down:
		case Key_S:
			if (m_map) m_observer -> backward();
		break;
		
		case Key_Left:
		case Key_A:
			if (m_map) m_observer -> left();
		break;
		
		case Key_Right:
		case Key_D:
			if (m_map) m_observer -> right();
		break;
		
		case Key_Escape:
			qApp -> quit();
		break;
		
		case Key_F2:
			m_FPSEnabled = !m_FPSEnabled;
			if (m_FPSEnabled) initFPSTimer();
			else m_FPSTimer -> stop();
		break;
		
// 		case Key_F1:
// 		
// 		int viewport[4];
// 		double modelViewMatrix[16];
// 		double projectionMatrix[16];
// 		glGetIntegerv(GL_VIEWPORT, viewport);
// 		glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
// 		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
// 	
// 		for (int i = 0; i < 4; i += 1)
// 		{
// 			qDebug("%f %f %f %f", modelViewMatrix[i], modelViewMatrix[i+4], modelViewMatrix[i+8], modelViewMatrix[i+12]);
// 		}
// 		
// 		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
// 		
// 		x1 = modelViewMatrix[0] * 10.0 + modelViewMatrix[4] * 11.0 + modelViewMatrix[8] * 12.0 + modelViewMatrix[12];
// 		y1 = modelViewMatrix[1] * 10.0 + modelViewMatrix[5] * 11.0 + modelViewMatrix[8] * 12.0 + modelViewMatrix[13];
// 		z1 = modelViewMatrix[2] * 10.0 + modelViewMatrix[6] * 11.0 + modelViewMatrix[10] * 12.0 + modelViewMatrix[14];
// 		
// 		x2 = modelViewMatrix[0] * 10.0 + modelViewMatrix[4] * 15.0 + modelViewMatrix[8] * 12.0 + modelViewMatrix[12];
// 		y2 = modelViewMatrix[1] * 10.0 + modelViewMatrix[5] * 15.0 + modelViewMatrix[8] * 12.0 + modelViewMatrix[13];
// 		z2 = modelViewMatrix[2] * 10.0 + modelViewMatrix[6] * 15.0 + modelViewMatrix[10] * 12.0 + modelViewMatrix[14];
// 		
// 		x3 = modelViewMatrix[0] * 0.0 + modelViewMatrix[4] * 4.0 + modelViewMatrix[8] * 0.0;
// 		y3 = modelViewMatrix[1] * 0.0 + modelViewMatrix[5] * 4.0 + modelViewMatrix[8] * 0.0;
// 		z3 = modelViewMatrix[2] * 0.0 + modelViewMatrix[6] * 4.0 + modelViewMatrix[10] * 0.0;
// 		
// 		qDebug("%f %f %f", x1, y1, z1);
// 		qDebug("%f %f %f", x2, y2, z2);
// 		
// 		qDebug("%f %f %f", x2 - x1, y2 - y1, z2 - z1);
// 		qDebug("%f %f %f", x3, y3, z3);
// 
// 		m_observer->pepe();
// 	
// 		break;
		
		case Key_1:
			if (!m_map) return;
		
			glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
			
			// 	while(splitQueue.size() < 20)
			for (int kk = 0; kk < 1; kk++)
			{
				fakeTriangle ft = splitQueue.last();
// 				qDebug("Parto %s", ft.m_t->nom.latin1());
				(*ft) -> split(splitQueue, modelViewMatrix);
			}
			updateGL();
		break;
		
		default:
		
		break;
	}
	updateGL();
}

void glWidget::mouseMoveEvent(QMouseEvent *e)
{
	int hw, hh, dify, difx;
	
	if (m_fromPopup)
	{
		m_fromPopup = false;
		return;
	}
	
	hw = width() / 2;
	hh = height() / 2;
	
	dify = hw - e -> x();
	difx = hh - e -> y();
	
	// TODO that 50.0 should be box dependant ... or not?
	if (m_map) m_observer -> rotate(difx / 25.0, dify / 25.0);
	
	QCursor::setPos(hw, hh);
	updateGL();
	
}

void glWidget::mousePressEvent(QMouseEvent *e)
{
	if (e -> button() == RightButton)
	{
		m_fromPopup = true;
		QPopupMenu popup;
		popup.insertItem("&Open", this, SLOT(openMap()));
		popup.insertItem("&Quit", qApp, SLOT(quit()));
		popup.exec(e->pos());
	}
}

void glWidget::openMap()
{
	setCursor(ArrowCursor);
	QString file = QFileDialog::getOpenFileName(QString::null, "DEM files (*.dem);;Plain files (*)", this);
	setCursor(BlankCursor);
	if (file.isNull()) return;
	
	QFile f(file);
	map *m;
	if (file.right(4) == ".dem") m = DEMParser::parse(f);
	else m = myParser::parse(f);
	
	m->square();
	
	closeMap();
	
	m_map = m;
	
	m_diamond = m_map -> baseDiamond();

	int size = m_map -> columns();
	const float r = 1.75;
	const float degX = M_PI / 4;
	m_observer = new observer(size / 2.0, m_map -> midHeight() + size * (r - 0.5) * tan(degX), r * size, degX, 0, size / 150.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float ox, oy, oz, vrpx, vrpy, vrpz;
	m_observer -> position(&ox, &oy, &oz);
	m_observer -> vrp(&vrpx, &vrpy, &vrpz);
	gluLookAt(ox, oy, oz, vrpx, vrpy, vrpz, 0, 1, 0);
	
	splitQueue.clear();
	
	double modelViewMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	splitQueue.insert(fakeTriangle(m_diamond -> t1(), modelViewMatrix));
	splitQueue.insert(fakeTriangle(m_diamond -> t2(), modelViewMatrix));

	QCursor::setPos(width() / 2, height() / 2);
	m_fromPopup = true;
}

void glWidget::updateFPS()
{
	int aux;
	aux = m_newFPSSum / m_FPSTimes;
	m_lastFPS = 1000 / aux;
	m_FPSTimes = 0;
	m_newFPSSum = 0;
}

void glWidget::closeMap()
{
	if (m_map)
	{
		delete m_map;
		delete m_diamond -> t1();
		delete m_diamond -> t2();
		delete m_diamond;
		delete m_observer;
	}
}

void glWidget::initFPSTimer()
{
	m_FPSTimer -> start(1000);
	m_lastTime = QTime::currentTime();
	m_lastFPS = 0;
	m_newFPSSum = 0;
	m_FPSTimes = 0;
}

void glWidget::paintTriangle(triangle *t) const
{
	if (t -> isLeaf())
	{
		glColor3dv(t -> apex() -> color());
		glVertex3dv(t -> apex() -> coords());
		glColor3dv(t -> rightVertex() -> color());
		glVertex3dv(t -> rightVertex() -> coords());
		glColor3dv(t -> leftVertex() -> color());
		glVertex3dv(t -> leftVertex() -> coords());
	}
	else
	{
		paintTriangle(t -> leftTriangle());
		paintTriangle(t -> rightTriangle());
	}
}

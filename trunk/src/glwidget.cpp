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
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qtimer.h>

#include "glwidget.h"

glWidget::glWidget(QWidget *parent) : QGLWidget(parent), m_fromPopup(false), m_FPSEnabled(true)
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
// 	glShadeModel(GL_SMOOTH);
// 	glShadeModel(GL_FLAT);
}

void glWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (m_roam.hasMap()) m_roam.paint();
	swapBuffers();
}

void glWidget::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
		case Key_Up:
		case Key_W:
			if (m_roam.hasMap()) m_roam.moveObserverForward();
		break;
		
		case Key_Down:
		case Key_S:
			if (m_roam.hasMap()) m_roam.moveObserverBackward();
		break;
		
		case Key_Left:
		case Key_A:
			if (m_roam.hasMap()) m_roam.moveObserverLeft();
		break;
		
		case Key_Right:
		case Key_D:
			if (m_roam.hasMap()) m_roam.moveObserverRight();
		break;
		
		case Key_Escape:
			qApp -> quit();
		break;
		
// 		case Key_F2:
// 			m_FPSEnabled = !m_FPSEnabled;
// 			if (m_FPSEnabled) initFPSTimer();
// 			else m_FPSTimer -> stop();
// 		break;
		
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
			if (m_roam.hasMap()) m_roam.splitOne();
		break;
		
		case Key_2:
			if (m_roam.hasMap()) m_roam.mergeOne();
		break;
		
// 		case Key_3:
// 			for (int i = 0; i < 10; i++)
// 			{
// 				d = m_map -> height(1292, i);
// 				qDebug("%f", d);
// 				d = m_map -> height(1291, i);
// 				qDebug("%f", d);
// 			}
// 			
// 		break;
		
// 		case Key_2:
// 		float a, b, c, p1, q1, r1, p2, q2, r2, p3, q3, r3, d1, d2, d3;
// 		glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
// 			a = modelViewMatrix[4];
// 			b = modelViewMatrix[5];
// 			c = modelViewMatrix[6];
// 	
// 			p1 = modelViewMatrix[0] * 646.5 + 
// 	     		modelViewMatrix[4] * 446.7 + 
// 	     		modelViewMatrix[8] * 2300.0 + 
// 	     		modelViewMatrix[12];
// 			q1 = modelViewMatrix[1] * 646.5 + 
// 	     		modelViewMatrix[5] * 446.7 + 
// 	     		modelViewMatrix[9] * 2300.0 + 
// 	     	modelViewMatrix[13];
// 	r1 = modelViewMatrix[2] * 646.5 + 
// 	     modelViewMatrix[6] * 446.7 + 
// 	     modelViewMatrix[10] * 2300.0 + 
// 	     modelViewMatrix[14];
// 		 
// 		 d1 = 2 / (r1 * r1 - c * c) * sqrt(pow(a * r1 - c * p1, 2) + pow(b * r1 - c * q1, 2));
// 		 qDebug("%f %f %f", p1, q1, r1);
// 		 qDebug("Prioridad 1:%f", d1);
// 		 
// 		 p2 = modelViewMatrix[0] * 646.5 + 
// 	     		modelViewMatrix[4] * 446.7 + 
// 	     		modelViewMatrix[8] * 2400.0 + 
// 	     		modelViewMatrix[12];
// 			q2 = modelViewMatrix[1] * 646.5 + 
// 	     		modelViewMatrix[5] * 446.7 + 
// 	     		modelViewMatrix[9] * 2400.0 + 
// 	     	modelViewMatrix[13];
// 	r2 = modelViewMatrix[2] * 646.5 + 
// 	     modelViewMatrix[6] * 446.7 + 
// 	     modelViewMatrix[10] * 2400.0 + 
// 	     modelViewMatrix[14];
// 		 
// 		 qDebug("%f %f %f", p2, q2, r2);
// 		 d2 = 2 / (r2 * r2 - c * c) * sqrt(pow(a * r2 - c * p2, 2) + pow(b * r2 - c * q2, 2));
// 		 qDebug("Prioridad 2:%f", d2);
// 		break;
		
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
	
	// TODO that 25.0 should be box dependant ... or not?
	if (m_roam.hasMap()) m_roam.rotateObserver(difx / 25.0, dify / 25.0);
	
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
		int id = popup.insertItem("&Close", this, SLOT(closeMap()));
		popup.insertItem("&Quit", qApp, SLOT(quit()));
		popup.setItemEnabled(id, m_roam.hasMap());
		popup.exec(e->pos());
	}
}

void glWidget::openMap()
{
	setCursor(ArrowCursor);
	QString file = QFileDialog::getOpenFileName(QString::null, "DEM files (*.dem);;Plain maps (*.pm)", this);
	setCursor(BlankCursor);
	if (file.isNull()) return;
	
	ROAM::Error e = m_roam.open(file);
	if (e != ROAM::OK)
	{
		if (e == ROAM::openingError) QMessageBox::critical(this, "Error", QString("Could not open the map, check you have read permission on %1").arg(file), QMessageBox::Ok, QMessageBox::NoButton);
		else if (e == ROAM::unknownFormat) QMessageBox::critical(this, "Error", "There is no parser available for that kind of file.", QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	updateGL();
	
	QCursor::setPos(width() / 2, height() / 2);
	m_fromPopup = true;
}

void glWidget::closeMap()
{
	m_roam.close();
	updateGL();
}

void glWidget::updateFPS()
{
	int aux;
	aux = m_newFPSSum / m_FPSTimes;
	m_lastFPS = 1000 / aux;
	m_FPSTimes = 0;
	m_newFPSSum = 0;
}

void glWidget::initFPSTimer()
{
	m_FPSTimer -> start(1000);
	m_lastTime = QTime::currentTime();
	m_lastFPS = 0;
	m_newFPSSum = 0;
	m_FPSTimes = 0;
}

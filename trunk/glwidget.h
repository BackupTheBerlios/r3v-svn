/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <qdatetime.h>
#include <qgl.h>

#include "trianglelist.h"

class QTimer;

class diamond;
class map;
class observer;
class triangle;

class glWidget : public QGLWidget
{
Q_OBJECT
	public:
		glWidget(QWidget *parent);
		~glWidget();
	
	protected:
		void paintGL();
		void resizeGL(int width, int height);
		void initializeGL();
		
		void keyPressEvent(QKeyEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
	
	private slots:
		void openMap();
		void updateFPS();

	private:
		void closeMap();
		void initFPSTimer();
		void paintTriangle(triangle *t) const;
	
		map *m_map;
		diamond *m_diamond;
		
		observer *m_observer;
		
		// popup
		bool m_fromPopup;
		
		// FPS related
		bool m_FPSEnabled;
		QTimer *m_FPSTimer;
		QTime m_lastTime;
		int m_lastFPS, m_newFPSSum, m_FPSTimes;
		int m_fontHeight;
		
		triangleList splitQueue;
};

#endif

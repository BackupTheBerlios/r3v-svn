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
#include <qmap.h>
#include <qgl.h>
#include <qtranslator.h>

#include "libroam/roam.h"

class QTimer;

class glWidget : public QGLWidget
{
Q_OBJECT
	public:
		glWidget(QWidget *parent);
		~glWidget();
		
		void openMap(const QString &file);
	
	public slots:
		void openMap();
	
	protected:
		void paintGL();
		void resizeGL(int width, int height);
		void initializeGL();
		
		void keyPressEvent(QKeyEvent *e);
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
	
	private slots:
		void closeMap();
		void updateFPS();
	
	private:
		void findTranslationFiles(const QString &path);
		int findTranslation(const QString &locale);
		void initFPSTimer();
	
		// popup
		bool m_fromPopup;
		
		// FPS related
		bool m_FPSEnabled;
		QTimer *m_FPSTimer;
		QTime m_lastTime;
		int m_lastFPS, m_newFPSSum, m_FPSTimes;
		int m_fontHeight;
		
		QMap<int, QString> m_langs;
		
		ROAM m_roam;
		
		QTranslator m_tra;
};

#endif

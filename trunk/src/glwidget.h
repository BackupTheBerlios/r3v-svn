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
		
		void openMapDeferred(const QString &file);
	
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
		void openMapDeferredSlot();
		void about();
	
	private:
		void openMap(const QString &file);
		void findTranslationFiles(const QString &path, QMap<int, QString> &langs);
		int findTranslation(const QString &locale, QMap<int, QString> &langs);

		// deferred map opening
		QString m_deferredFile;
		
		// popup
		bool m_fromPopup;
		
		ROAM m_roam;
		
		QTranslator m_tra;
};

#endif

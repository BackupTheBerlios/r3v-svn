/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <config.h>

#include <math.h>

#include <qapplication.h> 
#include <qcursor.h>
#include <qdesktopwidget.h> 
#include <qevent.h> 
#include <qfiledialog.h>
#include <qfontmetrics.h>
#include <qlocale.h>
#include <qmessagebox.h>
#if QT4
#include <qmenu.h>
#else
#include <qpopupmenu.h>
#endif
#include <qtimer.h>

#include "glwidget.h"

glWidget::glWidget(QWidget *parent) : QGLWidget(parent), m_fromPopup(false), m_FPSEnabled(true)
{
/*	QTimer *t = new QTimer(this);
	connect(t, SIGNAL(timeout()), this, SLOT(updateGL()));
	t -> start(40);
	
	m_FPSTimer = new QTimer(this);
	connect(m_FPSTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));
	initFPSTimer();
	
	m_fontHeight = QFontMetrics(font()).height();
	m_lastTime = QTime::currentTime();*/

	showFullScreen();

	QString locale = QLocale::system().name();
	// remove the .encoding from the locale
#if QT4
	locale = locale.left(locale.lastIndexOf("."));
#else
	locale = locale.left(locale.findRev("."));
#endif

	findTranslationFiles(QString("%1/share/apps/r3v/translations").arg(PREFIX));
	// if did not found any translation file installed try to look for non installed ones
	if (m_langs.count() == 0) findTranslationFiles("translations");

	// look if we find any translation matching the current locale
	int i = findTranslation(locale);
	if (i == -1)
	{
		// failed finding a translation for the current locale, if the locale is foo_bar try finding
		// one for foo
#if QT4
		locale = locale.left(locale.lastIndexOf("_"));
#else
		locale = locale.left(locale.findRev("_"));
#endif
		i = findTranslation(locale);
	}
	
	if (i != -1)
	{
		m_tra.load(m_langs[i]);
		qApp -> installTranslator(&m_tra);
	}
}

glWidget::~glWidget()
{
}

void glWidget::openMap(const QString &file)
{
	setCursor(Qt::BlankCursor);
	if (file.isNull()) return;
	
#if QT4
	ROAM::Error e = m_roam.open(file.toStdString());
#else
	ROAM::Error e = m_roam.open(file);
#endif
	if (e != ROAM::OK)
	{
		if (e == ROAM::openingError) QMessageBox::critical(this, tr("Error"), tr("Could not open the map, check you have read permission on %1").arg(file), QMessageBox::Ok, QMessageBox::NoButton);
		else if (e == ROAM::unknownFormat) QMessageBox::critical(this, tr("Error"), tr("There is no parser available for that kind of file."), QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	
	QCursor::setPos(width() / 2, height() / 2);
	m_fromPopup = true;
	setMouseTracking(true);
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
		case Qt::Key_Up:
		case Qt::Key_W:
			if (m_roam.hasMap()) m_roam.moveObserverForward();
		break;
		
		case Qt::Key_Down:
		case Qt::Key_S:
			if (m_roam.hasMap()) m_roam.moveObserverBackward();
		break;
		
		case Qt::Key_Left:
		case Qt::Key_A:
			if (m_roam.hasMap()) m_roam.moveObserverLeft();
		break;
		
		case Qt::Key_Right:
		case Qt::Key_D:
			if (m_roam.hasMap()) m_roam.moveObserverRight();
		break;
		
		case Qt::Key_Q:
			if (m_roam.hasMap()) m_roam.moveObserverUp();
		break;
		
		case Qt::Key_E:
			if (m_roam.hasMap()) m_roam.moveObserverDown();
		break;
		
		case Qt::Key_Escape:
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
		
/*		case Qt::Key_1:
			if (m_roam.hasMap()) m_roam.splitOne();
		break;
		
		case Qt::Key_2:
			if (m_roam.hasMap()) m_roam.mergeOne();
		break;*/
		
/*		case Qt::Key_3:
			if (m_roam.hasMap()) m_roam.renew();
		break;*/
		
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
	if (e -> button() == Qt::RightButton)
	{
		m_fromPopup = true;
#if QT4
		QMenu popup;
		popup.addAction(tr("&Open"), this, SLOT(openMap()));
		QAction *a = popup.addAction(tr("&Close"), this, SLOT(closeMap()));
		popup.addAction(tr("&Quit"), qApp, SLOT(quit()));
		a->setEnabled(m_roam.hasMap());
#else
		QPopupMenu popup;
		popup.insertItem(tr("&Open"), this, SLOT(openMap()));
		int id = popup.insertItem(tr("&Close"), this, SLOT(closeMap()));
		popup.insertItem(tr("&Quit"), qApp, SLOT(quit()));
		popup.setItemEnabled(id, m_roam.hasMap());
#endif
		popup.exec(e->pos());
	}
}

void glWidget::openMap()
{
	setCursor(Qt::ArrowCursor);
#if QT4
	QString file = QFileDialog::getOpenFileName(this, QString(), QString(), tr("DEM files (*.dem);;Plain maps (*.pm)"));
#else
	QString file = QFileDialog::getOpenFileName(QString::null, tr("DEM files (*.dem);;Plain maps (*.pm)"), this);
#endif
	
	openMap(file);
	updateGL();
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

void glWidget::findTranslationFiles(const QString &path)
{
	QStringList list;
	QString filePath;
	QDir d(path);
#if QT4
	QStringList filters;
	filters << "*.qm";
	list = d.entryList(filters);
#else
	d.setNameFilter("*.qm");
	list = d.entryList();
#endif
	
	int i = m_langs.count();
	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
	{
		filePath = d.path()+"/"+*it;
		m_tra.load(filePath);
		qApp -> installTranslator(&m_tra);
		m_langs.insert(i, filePath);
		qApp -> removeTranslator(&m_tra);
		i++;
	}
}

int glWidget::findTranslation(const QString &locale)
{
	QStringList files = m_langs.values();
	QStringList::const_iterator it, itEnd;
	it = files.begin();
	itEnd = files.end();
	int i = 0;
	for(; it != itEnd; ++it)
	{
#if QT4
		QString file = (*it).right((*it).length() - (*it).lastIndexOf("/") - 1);
		// remove the .qm from the file
		file = file.left(file.lastIndexOf("."));
		if (file.toLower() == locale.toLower()) break;
#else
		QString file = (*it).right((*it).length() - (*it).findRev("/") - 1);
		// remove the .qm from the file
		file = file.left(file.findRev("."));
		if (file.lower() == locale.lower()) break;
#endif
		i++;
	}
	if (it == itEnd) return -1;
	else return i;
}

void noop()
{
	/* only here to cheat lupdate*/
	QT_TRANSLATE_NOOP("QFileDialog", "Open");
	QT_TRANSLATE_NOOP("QFileDialog", "File &name:");
	QT_TRANSLATE_NOOP("QFileDialog", "File &type:");
	QT_TRANSLATE_NOOP("QFileDialog", "Cancel");
	QT_TRANSLATE_NOOP("QFileDialog", "Look &in:");
	QT_TRANSLATE_NOOP("QFileDialog", "Back");
	QT_TRANSLATE_NOOP("QFileDialog", "One directory up");
	QT_TRANSLATE_NOOP("QFileDialog", "Create New Folder");
	QT_TRANSLATE_NOOP("QFileDialog", "List View");
	QT_TRANSLATE_NOOP("QFileDialog", "Detail View");
	QT_TRANSLATE_NOOP("QFileDialog", "&Rename");
	QT_TRANSLATE_NOOP("QFileDialog", "&Delete");
	QT_TRANSLATE_NOOP("QFileDialog", "R&eload");
	QT_TRANSLATE_NOOP("QFileDialog", "Sort");
	QT_TRANSLATE_NOOP("QFileDialog", "Show &hidden files");
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Name");
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Size");
	QT_TRANSLATE_NOOP("QFileDialog", "Sort by &Date");
	QT_TRANSLATE_NOOP("QFileDialog", "&Unsorted");
	QT_TRANSLATE_NOOP("QLineEdit", "&Undo");
	QT_TRANSLATE_NOOP("QLineEdit", "&Redo");
	QT_TRANSLATE_NOOP("QLineEdit", "Cu&t");
	QT_TRANSLATE_NOOP("QLineEdit", "&Copy");
	QT_TRANSLATE_NOOP("QLineEdit", "&Paste");
	QT_TRANSLATE_NOOP("QLineEdit", "Clear");
	QT_TRANSLATE_NOOP("QLineEdit", "Select All");
}

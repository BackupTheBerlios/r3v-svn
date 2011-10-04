/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef QT4
#include <config.h>
#endif

#ifndef PREFIX
#define PREFIX "/usr"
#endif

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

glWidget::glWidget(QWidget *parent) : QGLWidget(parent), m_fromPopup(false), m_roam(5000)
{
	QString iconPath = QString("%1/share/apps/r3v/icon/r3v.png").arg(PREFIX);
	if (!QFile::exists(iconPath))
	{
		iconPath = "icon/r3v.png";
		if (!QFile::exists(iconPath))
		{
			iconPath = "";
		}
	}
	
	
	if (!iconPath.isEmpty())
	{
#if QT4
		setWindowIcon(QIcon(iconPath));
#else
		setIcon(iconPath);
#endif
	}

	showFullScreen();

	QString locale = QLocale::system().name();
	// remove the .encoding from the locale
#if QT4
	locale = locale.left(locale.lastIndexOf("."));
#else
	locale = locale.left(locale.findRev("."));
#endif

	QMap<int, QString> langs;

	findTranslationFiles(QString("%1/share/apps/r3v/translations").arg(PREFIX), langs);
	// if did not found any translation file installed try to look for non installed ones
	if (langs.count() == 0) findTranslationFiles("translations", langs);

	// look if we find any translation matching the current locale
	int i = findTranslation(locale, langs);
	if (i == -1)
	{
		// failed finding a translation for the current locale, if the locale is foo_bar try finding
		// one for foo
#if QT4
		locale = locale.left(locale.lastIndexOf("_"));
#else
		locale = locale.left(locale.findRev("_"));
#endif
		i = findTranslation(locale, langs);
	}
	
	if (i != -1)
	{
		m_tra.load(langs[i]);
		qApp -> installTranslator(&m_tra);
	}
}

glWidget::~glWidget()
{
}

void glWidget::openMapDeferred(const QString &file)
{
	m_deferredFile = file;
	QTimer::singleShot(0, this, SLOT(openMapDeferredSlot()));
}

void glWidget::openMapDeferredSlot()
{
	openMap(m_deferredFile);
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
	updateGL();
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
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
		
		default:
		break;
	}
	updateGL();
}

void glWidget::mouseMoveEvent(QMouseEvent *e)
{
	int hw, hh, dify, difx;
	
	hw = width() / 2;
	hh = height() / 2;
	
	if (m_fromPopup)
	{
		m_fromPopup = false;
		QCursor::setPos(hw, hh);
		return;
	}
	
	dify = hw - e -> x();
	difx = hh - e -> y();
	
	if (m_roam.hasMap()) m_roam.rotateObserver(difx / 250.0, dify / 250.0);
	
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
		a = popup.addAction(tr("&About"), this, SLOT(about()));
		popup.insertSeparator(a);
#else
		QPopupMenu popup;
		popup.insertItem(tr("&Open"), this, SLOT(openMap()));
		int id = popup.insertItem(tr("&Close"), this, SLOT(closeMap()));
		popup.insertItem(tr("&Quit"), qApp, SLOT(quit()));
		popup.setItemEnabled(id, m_roam.hasMap());
		popup.insertSeparator();
		popup.insertItem(tr("&About"), this, SLOT(about()));
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
}

void glWidget::closeMap()
{
	m_roam.close();
	updateGL();
}

void glWidget::about()
{
	setCursor(Qt::ArrowCursor);
	QMessageBox::about(this, tr("About"), tr("Roam 3D Viewer\n\nDeveloped by Albert Astals Cid\nSupervised by Llu� P�ez Vidal\n\nThis program is Free Software"));
	setCursor(Qt::BlankCursor);
}

void glWidget::findTranslationFiles(const QString &path, QMap<int, QString> &langs)
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
	
	int i = langs.count();
	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
	{
		filePath = d.path()+"/"+*it;
		m_tra.load(filePath);
		qApp -> installTranslator(&m_tra);
		langs.insert(i, filePath);
		qApp -> removeTranslator(&m_tra);
		i++;
	}
}

int glWidget::findTranslation(const QString &locale, QMap<int, QString> &langs)
{
	QStringList files = langs.values();
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
	QT_TRANSLATE_NOOP("QMessageBox", "OK");
}

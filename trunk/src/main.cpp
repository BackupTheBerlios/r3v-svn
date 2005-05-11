/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <config.h>

#include <qapplication.h>
#include <qcursor.h>
#include <qfile.h>
#include <qmessagebox.h>

#include "glwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	glWidget *g = new glWidget(0);
	g->show();
	
#if !QT4
	a.setMainWidget(g);
#endif

	if (argc == 2)
	{
		QString file = QFile::decodeName(argv[1]);
		if (QFile::exists(file))
		{
			g->openMapDeferred(file);
		}
		else
		{
			QMessageBox::critical(g, a.tr("Error"), a.tr("File %1 does not exist").arg(file), QMessageBox::Ok, QMessageBox::NoButton);
			g->openMap();
		}
	}
	else
	{
		if (argc > 2) QMessageBox::critical(g, a.tr("Error"), "Syntax error, correct syntax is\nr3v [mapFile]\n\tmapFile: path of a map file (optional)");
		g->openMap();
	}

	a.exec();
	
	delete g;
}

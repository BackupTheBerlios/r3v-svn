/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qapplication.h>
#include <qfile.h>

#include <stdio.h>

#include "diamond.h"
#include "glwidget.h"
#include "map.h"
#include "node.h"
#include "parser.h"
#include "triangle.h"

int main(int argc, char *argv[])
{
// 	QFile f("mauna_kea.dem");
// 	map * m = DEMParser::parse(f);
// m->square();
	
/*	qDebug("Columnas %d", m->columns());
	for (int i = 0; i < m->columns(); i++)
	{
		qDebug("Alturas de %d es %d", i, m->heights(i));
	}*/
	
/*	for (int i = 0; i < m->columns(); i++)
	{
		for (int j = 0; j < m->columns(); j++)
		{
			qDebug("%d %d es %f", i, j, m->height(i, j));
		}
	}*/
	
	QApplication a(argc, argv);
	
	glWidget *g = new glWidget(0);
	g->show();
	
	a.setMainWidget(g);

	a.exec();
	
	delete g;
}

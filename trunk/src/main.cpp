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

#include "glwidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	glWidget *g = new glWidget(0);
	g->show();
	
#if !QT4
	a.setMainWidget(g);
#endif

	if (a.argc() == 2) g->openMap(a.argv()[1]);
	else g->openMap();

	a.exec();
	
	delete g;
}

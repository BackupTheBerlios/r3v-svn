/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAP_H
#define MAP_H

#include <qcolor.h>
#include <qmap.h>
#include <qvaluevector.h>

#include "triangle.h"

class diamond;
class node;

class r3vMap
{
	public:
		r3vMap();
		~r3vMap();
		
		void addColumn(QValueVector<double> *column);
		
		diamond *baseDiamond();
		
		double height(int i, int j) const;
		double height(double i, double j) const;
		int heights(int column) const;
		int columns() const;
		double midHeight() const;
		
		node *getNode(double x, double y);
		
		void square();
	
	private:
		QColor color(double height) const;
	
		bool checkSquare(int pos, int size) const;
	
		QValueVector<QValueVector<double>*> m_heights;
		double m_minHeight, m_amplitude;
		
		QMap<QPair<double, double>, node*> m_nodes;
};

#endif

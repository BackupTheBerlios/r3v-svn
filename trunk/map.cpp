/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <math.h>
 
#include "diamond.h"
#include "map.h"
#include "node.h"
#include "triangle.h"

r3vMap::r3vMap() : m_baseDiamond(0)
{
}

r3vMap::~r3vMap()
{
	QValueVector<QValueVector<double>*>::iterator it;
	while (m_heights.size())
	{
		it = m_heights.begin();
		delete *it;
		m_heights.erase(it);
	}
	
	qDebug("%d nodos", m_nodes.count());
	qDebug("%d triangulos", m_triangleCount);
	qDebug("%d hojas", m_leavesCount);
	m_baseDiamond -> clean();
	delete m_baseDiamond;
	qDebug("%d triangulos", m_triangleCount);
	qDebug("%d hojas", m_leavesCount);
	QMap<QPair<double, double>, node*>::const_iterator it2;
	it2 = m_nodes.begin();
	while (it2 != m_nodes.end())
	{
		delete *it2;
		++it2;
	}
	m_nodes.clear();
}

void r3vMap::addColumn(QValueVector<double> *column)
{
	m_heights.append(column);
}

diamond *r3vMap::baseDiamond() 
{
	if (!m_baseDiamond) 
	{
		int size;
		node *n1, *n2, *n3;
		triangle *t1, *t2;
	
		size = columns();
		size--;
	
		n1 = getNode(0, 0);
		n2 = getNode(size, 0);
		n3 = getNode(0, size);
		t1 = new triangle(*this, n1, n2, n3, 0, "T1");
	
		n1 = getNode(size, size);
		t2 = new triangle(*this, n1, n3, n2, 0, "T2");
	
	/*	t1 -> setBaseTriangle(t2);
		t2 -> setBaseTriangle(t1);*/
	
		m_baseDiamond = new diamond(t1, t2);
		m_triangleCount = 2;
		m_leavesCount = 2;
	}
	return m_baseDiamond;
}

double r3vMap::height(int i, int j) const
{
	return (*m_heights[i])[j];
}

double r3vMap::height(double i, double j) const
{
	double h1, h2, h3, h4, d1x, d1y, d2x, d2y, d3x, d3y, d4x, d4y, res;
	int fi, fj, ci, cj;
	
	ci = (int)ceil(i);
	cj = (int)ceil(j);
	
	if (i == ci && j == cj) return height(ci, cj);
	
	fi = (int)floor(i);
	fj = (int)floor(j);
	
	h1 = height(fi, fj);
	h2 = height(ci, fj);
	h3 = height(fi, cj);
	h4 = height(ci, cj);
	
	d1x = fabs(i - fi);
	d1y = fabs(j - fj);
	d2x = fabs(i - ci);
	d2y = fabs(j - fj);
	d3x = fabs(i - fi);
	d3y = fabs(j - cj);
	d4x = fabs(i - ci);
	d4y = fabs(j - cj);
	
	d1x = 1 - d1x;
	d1y = 1 - d1y;
	d2x = 1 - d2x;
	d2y = 1 - d2y;
	d3x = 1 - d3x;
	d3y = 1 - d3y;
	d4x = 1 - d4x;
	d4y = 1 - d4y;
	
	if (i == ci) return h2 * d2y + h4 * d4y;
	if (i == fi) return h1 * d1y + h3 * d3y;
	if (j == cj) return h3 * d3x + h4 * d4x;
	if (j == fj) return h1 * d1x + h2 * d2x;
	
	res = d1x;
	if (d1y < res && d1y != 0) res = d1y;
	if (d2y < res && d2x != 0) res = d2x;
	if (d2y < res && d2y != 0) res = d2y;
	if (d3y < res && d3x != 0) res = d3x;
	if (d3y < res && d3y != 0) res = d3y;
	if (d4y < res && d4x != 0) res = d4x;
	if (d4y < res && d4y != 0) res = d4y;
	
// 	qDebug("%f", res);
	
// 	qDebug("%f %f %f %f", d1x / res * d1y / res, d2x / res * d2y / res, d3x / res * d3y / res, d4x / res * d4y / res);
	
	return (h1 * d1x / res * d1y / res + h2 * d2x / res * d2y / res + h3 * d3x / res * d3y / res + h4 * d4x / res * d4y / res) / ((1 / res) * (1 / res));
}

int r3vMap::heights(int column) const
{
	return m_heights[column]->size();
}

int r3vMap::columns() const
{
	return m_heights.size();
}

double r3vMap::midHeight() const
{
	return m_minHeight + m_amplitude / 2;
}

node *r3vMap::getNode(double x, double y)
{
	QMap<QPair<double, double>, node*>::const_iterator it;
	QPair<double, double> p(x, y);
	
	it = m_nodes.find(p);
	if (it != m_nodes.end())
	{
// 		qDebug("ME piden uno que existe %f %f", x, y);
		return it.data();
	}
	
	double h;
	h = height(x, y);
	node *n = new node(x, y, h, color(h));
	m_nodes[p] = n;
	
	return n;
}

void r3vMap::square()
{
	int x = 0; // maximum square side size
	int p = 0; // position where the maximum square side size begins
	int col = columns();
	int n; // current side size
	for (int i = 0; i < col; i++)
	{
		n = heights(i);

		if (i + n >= col) n = col - i;
		if (n > x)
		{
			bool good = checkSquare(i, n);
			if (good)
			{
				x = n;
				p = i;
			}
// 			else 
// 			{
// 				int nn = n - 1;
// 				while(!good && nn > x)
// 				{
// 					good = checkSquare(i, nn);
// 					if (good)
// 					{
// 						x = nn;
// 						p = i;
// 					}
// 					nn--;
// 				}
// 			}
			
		}
	}
	qDebug("Lado maximo %d", x);
	qDebug("Posicion %d", p);
	/*qDebug("Demostracion");
	for (int i = 0; i < x; i++)
		qDebug("Altura de %d es %d", i+p, heights(i+p));*/
	
// 	x -= 10;
	
	// once we have searched the maximum square we crop the map
	QValueVector<QValueVector<double>*>::iterator it;
	for (int i = 0; i < p; i++)
	{
		it = m_heights.begin();
		delete *it;
		m_heights.erase(it);
	}
	
	QValueVector<double> *v;
	for (int i = 0; i < x; i++)
	{
		v = m_heights[i];
		v -> resize(x);
	}
	
	for (int i = 0; i < col - p - x; i++)
	{
		v = m_heights[i + x];
		delete v;
	}
	m_heights.resize(x);
	
	col = columns();
	double maxHeight, aux;
	maxHeight = height(0, 0);
	m_minHeight = height(0, 0);
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < col; j++)
		{
			aux = height(i,j);
			if (aux > maxHeight) maxHeight = aux;
			if (aux < m_minHeight) m_minHeight = aux;
		}
	}
	
	m_amplitude = maxHeight - m_minHeight;
	qDebug("%f %f", m_minHeight, m_amplitude);
}

void r3vMap::addTriangles(int n)
{
	m_triangleCount += n;
}

void r3vMap::addLeaves(int n)
{
	m_leavesCount += n;
}

int r3vMap::triangles() const
{
	return m_triangleCount;
}

int r3vMap::leaves() const
{
	return m_leavesCount;
}

QColor r3vMap::color(double h) const
{
	int r, g, b;
	
	if (h < (m_minHeight + 0.25 * m_amplitude))
	{
		r = 0;
		g = (int) qRound(4 * 255 * (h - m_minHeight) / m_amplitude);
		b = 255;
	}
	else if (h < (m_minHeight + 0.5 * m_amplitude)) 
	{
		r = 0;
		g = 255;
		b = (int) qRound(2 * 255 - 4 * 255 * (h - m_minHeight) / m_amplitude);
	}
	else if (h < (m_minHeight + 0.75 * m_amplitude))
	{
		r = qRound(4 * 255 * (h - m_minHeight) / m_amplitude - 2 * 255);
		g = 255;
		b = 0;
	}
	else
	{
		r = 255;
		g = qRound(4 * 255 - 4 * 255 * (h - m_minHeight) / m_amplitude);
		b = 0;
	}
	
	return QColor(r, g, b);
}

bool r3vMap::checkSquare(int pos, int size) const
{
	bool good = true;
	int j = pos;
	while(good && j < size + pos)
	{
		good = good && heights(j) >= size;
		j++;
	}
	return good;
}

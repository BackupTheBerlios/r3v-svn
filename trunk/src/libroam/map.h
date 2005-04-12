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

#include <map>
#include <vector>

#include "triangle.h"

class diamond;
class node;

class r3vMap
{
	friend class DEMParser;
	
	public:
		r3vMap();
		~r3vMap();
		
		void setMap(const std::vector<std::vector<double>*> &heights, bool byColumns);
		
		diamond *baseDiamond();
		
		double height(int i, int j) const;
		double height(double i, double j) const;
		int size() const;
		double midHeight() const;
		
		node *getNode(double x, double y);
		
		void addTriangles(int n);
		void addLeaves(int n);
		
		int triangles() const;
		int leaves() const;
		
		void calcAmplitude();
		
	private:
		void color(double height, int &r, int &g, int &b) const;
	
		std::vector<std::vector<double>*> m_heights;
		bool m_byColumns;
		double m_minHeight, m_amplitude;
		
		std::map<std::pair<double, double>, node*> m_nodes;
		
		diamond *m_baseDiamond;
		int m_triangleCount;
		int m_leavesCount;
};

#endif

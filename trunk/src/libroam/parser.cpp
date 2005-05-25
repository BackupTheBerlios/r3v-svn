/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// #include <stdlib.h>
#include <fstream>

#include "parser.h"
#include "map.h"

r3vMap *DEMParser::parse(const std::string &file)
{
	r3vMap *m;
	m_file = new std::ifstream(file.c_str());
	m = 0;
	if (m_file -> is_open())
	{
		char aux[1025];
		char processCode;
		std::string name, descriptorField, originCode, sectionalIndicator;
		int DEMLevel, elevationPattern, groundPlanimetricReferenceSystem, zoneCode, groundCoordsUnit, elevationCoordsUnit, polygonSides;
		
		m = new r3vMap();
		
		/* name */
		name = readString(40, aux);
		
		/* description */
		descriptorField = readString(40, aux);;
		
		/* filler */
		m_file -> read(aux, 29);
		
		/* SE geographic quadrangle corner */
		int dx, mx, dy, my;
		double sx, sy;
		dx = readInt(4, aux);
		mx = readInt(2, aux);
		sx = readDouble(7, aux);
		
		dy = readInt(4, aux);
		my = readInt(2, aux);
		sy = readDouble(7, aux);
		
		/* process code */
		processCode = readChar();
		
		/* filler */
		m_file -> read(aux, 1);
		
		/* sectionalIndicator */
		sectionalIndicator = readString(3, aux);
		
		/* origin code */
		originCode = readString(4, aux);
		
		/* DEM LEVEL */
		DEMLevel = readInt(6, aux);
		
		/* Elevetion Pattern */
		elevationPattern = readInt(6, aux);
		
		/* ground planimetric reference system */
		groundPlanimetricReferenceSystem = readInt(6, aux);
		
		/* zone code */
		zoneCode = readInt(6, aux);
		
		if (groundPlanimetricReferenceSystem >= 0 && groundPlanimetricReferenceSystem <= 2)
		{
			m_file -> read(aux, 360);
		}
		else
		{
			perror("groundPlanimetricReferenceSystem is different from 0, 1 and 2");
			delete m_file;
			return 0;
		}
		
		/* groundCoordsUnit */
		groundCoordsUnit = readInt(6, aux);
		if (groundCoordsUnit == 0 || groundCoordsUnit == 3)
		{
			perror("Radians and arc-seconds are units of mesure not supported");
			delete m_file;
			return 0;
		}
		
		/* elevationCoordsUnit */
		elevationCoordsUnit = readInt(6, aux);
		
		/* polygonSides */
		polygonSides = readInt(6, aux);
		if (polygonSides != 4)
		{
			perror("polygonSides different from 4 is not supported");
			delete m_file;
			return 0;
		}
		
		/* coordenadas ? */
		double SEx, SEy, SWx, SWy, NWx, NWy, NEx, NEy;
		SEx = readDouble(24, aux);
		SEy = readDouble(24, aux);
		SWx = readDouble(24, aux);
		SWy = readDouble(24, aux);
		NWx = readDouble(24, aux);
		NWy = readDouble(24, aux);
		NEx = readDouble(24, aux);
		NEy = readDouble(24, aux);
		
		/* elevacion */
		double minEl, maxEl;
		minEl = readDouble(24, aux);
		maxEl = readDouble(24, aux);
		
		/* angulo de desviacion de noseque */
		double degree;
		degree = readDouble(24, aux);
		
		/* accuracy */
		int accuracy;
		accuracy = readInt(6, aux);
		
		/* resolution */
		double rx, ry, rz;
		rx = readDouble(12, aux);
		ry = readDouble(12, aux);
		rz = readDouble(12, aux);
#if DEBUG	
 		assert(rx == ry);
#endif
		
		double conversionFactor = rz / rx;
		if (groundCoordsUnit != elevationCoordsUnit)
		{
			if (groundCoordsUnit == 1)
			{
				// groundCoordsUnit = 1
				// elevationCoordsUnit = 2
				// groundCoordsUnit = feet 
				// elevationCoordsUnit = meters
				conversionFactor = conversionFactor / 0.3048;
			}
			else
			{
				// groundCoordsUnit = 2
				// elevationCoordsUnit = 1
				// groundCoordsUnit = meters
				// elevationCoordsUnit = feet
				conversionFactor = conversionFactor * 0.3048;
			}
		}
		
		/* perfiles */
		int row, n;
		row = readInt(6, aux);
#if DEBUG
		assert(row == 1);
#endif
		n = readInt(6, aux);
		
		m_file -> read(aux, 160);
		
		std::vector<std::vector<double>*> allHeights;
		std::vector<double> *heights;
		/* Record 2 */
		for (int i = 0; i < n; i++)
		{
			int l;
			l = 0;
		
			/* fila columna */
			int row, column;
			row = readInt(6, aux);
			column = readInt(6, aux);
			l += 12;
		
			int rows, columns;
			rows = readInt(6, aux);
			columns = readInt(6, aux);
#if DEBUG
			assert(columns == 1);
#endif
			l += 12;
			
			double xgp, ygp;
			xgp = readDouble(24, aux);
			ygp = readDouble(24, aux);
			l += 48;
		
			double elevation;
			elevation = readDouble(24, aux);
			l += 24;
		
			/* elevacion local */
			double minLocEl, maxLocEl;
			minLocEl = readDouble(24, aux);
			maxLocEl = readDouble(24, aux);
			l += 48;
			
			heights = new std::vector<double>;
		
			if (rows <= 146)
			{
				readHeights(rows, conversionFactor, heights, aux);
				l += rows*6;
				
				m_file -> read(aux, 1024 - l);
			}
			else
			{
				readHeights(146, conversionFactor, heights, aux);
				m_file -> read(aux, 4);
				
				int restan, read;
				restan = rows - 146;
				while (restan > 0)
				{
					if (restan >= 170) read = 170;
					else read = restan;
					
					readHeights(read, conversionFactor, heights, aux);
					m_file -> read(aux, 1024 - read * 6);
					restan -= read;
				}
			}
		
			allHeights.push_back(heights);
		}
		m_file -> close();
		
		// SQUARE THE MAP
		int x = 0; // maximum square side size
		int p = 0; // position where the maximum square side size begins
		int col = allHeights.size();
		for (int i = 0; i < col; i++)
		{
			n = allHeights[i]->size();

			if (i + n >= col) n = col - i;
			if (n > x)
			{
				bool good = true;
				int j = i;
				while(good && j < n + i)
				{
					good = good && allHeights[j]->size() >= n;
					j++;
				}
				
				if (good)
				{
					x = n;
					p = i;
				}
// 				else 
// 				{
// 					int nn = n - 1;
// 					while(!good && nn > x)
// 					{
// 						good = checkSquare(i, nn);
// 						if (good)
// 						{
// 							x = nn;
// 							p = i;
// 						}
// 						nn--;
// 					}
// 				}
			}
		}
	
		// once we have searched the maximum square we crop the map
		std::vector<std::vector<double>*>::iterator it;
		for (int i = 0; i < p; i++)
		{
			it = allHeights.begin();
			delete *it;
			allHeights.erase(it);
		}
	
		std::vector<double> *v;
		for (int i = 0; i < x; i++)
		{
			v = allHeights[i];
			v -> resize(x);
		}
		
		for (int i = 0; i < col - p - x; i++)
		{
			v = allHeights[i + x];
			delete v;
		}
		allHeights.resize(x);
		
		m->setMap(allHeights, true);
	}
	delete m_file;
	return m;
}

char DEMParser::readChar()
{
	char aux;
	
	m_file -> read(&aux, 1);
	return aux;
}

double DEMParser::readDouble(int length, char *aux)
{
	m_file -> read(aux, length);
	aux[length] = '\0';
	for (int i = 0; i < length; i++) if (aux[i] == 'D') aux[i] = 'E';
	return atof(aux);
}

int DEMParser::readInt(int length, char *aux)
{
	m_file -> read(aux, length);
	aux[length] = '\0';
	return atoi(aux);
}

std::string DEMParser::readString(int length, char *aux)
{
	std::string res;
	
	m_file -> read(aux, length);
	aux[length] = '\0';
	res = aux;
	return res;
}

void DEMParser::readHeights(int howMany, double factor, std::vector<double> *v, char *aux)
{
	int height;
	
	for (int i = 0; i < howMany; i++)
	{
		height = readInt(6, aux);
		v->push_back(height * factor);
	}
}


r3vMap *PMParser::parse(const std::string &file)
{
	std::vector<std::vector<double>*> allHeights;
	m_file = new std::ifstream(file.c_str());
	if (m_file -> is_open())
	{
		r3vMap *m;
		int columns;
		double aux;
		std::vector<double> *heights;
		
		m = new r3vMap();
		*m_file >> columns;
		
		
		for (int i = 0; i < columns; i++)
		{
			heights = new std::vector<double>;
			for (int j = 0; j < columns; j++)
			{
				*m_file >> aux;
				heights -> push_back(aux);
			}
			allHeights.push_back(heights);
		}
		
		m_file -> close();
		m -> setMap(allHeights, true);
		return m;
	}
	
	return 0;
}


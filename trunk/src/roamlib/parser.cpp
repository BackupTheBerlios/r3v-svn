/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h>

#include <qfile.h>
#include <qtextstream.h>

//#include "geopoint.h"
#include "parser.h"
#include "map.h"

r3vMap *DEMParser::parse(QFile &file)
{
	if (file.open(IO_ReadOnly))
	{
		char aux[1025];
		QChar processCode;
		QString name, descriptorField, originCode, sectionalIndicator;
		//geoPoint *SE;
		int DEMLevel, elevationPattern, groundPlanimetricReferenceSystem, zoneCode, groundCoordsUnit, elevationCoordsUnit, polygonSides;
		
		r3vMap *m = new r3vMap();
		
		/* name */
		name = readString(file, 40, aux);
		// qDebug("Nombre:%s", name.latin1());
		
		/* description */
		descriptorField = readString(file, 40, aux);;
		// qDebug("Descripci�:%s", descriptorField.latin1());
		
		/* filler */
		file.readBlock(aux, 29);
		
		/* SE geographic quadrangle corner */
		int dx, mx, dy, my;
		double sx, sy;
		dx = readInt(file, 4, aux);
		mx = readInt(file, 2, aux);
		sx = readDouble(file, 7, aux);
		
		dy = readInt(file, 4, aux);
		my = readInt(file, 2, aux);
		sy = readDouble(file, 7, aux);
		
		//SE = new geoPoint(dx, mx, sx, dy, my, sy);
		// qDebug("Punto SE:%s", SE->asString().latin1());
		
		/* process code */
		processCode = readChar(file);
		// qDebug("Process code:%c", processCode.latin1());
		
		/* filler */
		file.readBlock(aux, 1);
		
		/* sectionalIndicator */
		sectionalIndicator = readString(file, 3, aux);
		// qDebug("sectionalIndicator:%s", sectionalIndicator.latin1());
		
		/* origin code */
		originCode = readString(file, 4, aux);
		// qDebug("Origin Code:%s", originCode.latin1());
		
		/* DEM LEVEL */
		DEMLevel = readInt(file, 6, aux);
		// qDebug("Nivel DEM:%d", DEMLevel);
		
		/* Elevetion Pattern */
		elevationPattern = readInt(file, 6, aux);
		// qDebug("Patr� de elevaci�:%d", elevationPattern);
		
		/* ground planimetric reference system */
		groundPlanimetricReferenceSystem = readInt(file, 6, aux);
		// qDebug("groundPlanimetricReferenceSystem:%d", groundPlanimetricReferenceSystem);
		
		/* zone code */
		zoneCode = readInt(file, 6, aux);
		// qDebug("zoneCode:%d", zoneCode);
		
		if (groundPlanimetricReferenceSystem >= 0 && groundPlanimetricReferenceSystem <= 2)
		{
			file.readBlock(aux, 360);
		}
		else
		{
			qFatal("No tratamos groundPlanimetricReferenceSystem diferentes de 0, 1, o 2");
		}
		
		/* groundCoordsUnit */
		groundCoordsUnit = readInt(file, 6, aux);
		qDebug("groundCoordsUnit:%d", groundCoordsUnit);
		if (groundCoordsUnit == 0 || groundCoordsUnit == 3) qFatal("No soportamos radians ni arc-segundos como unidad de medida");
		
		/* elevationCoordsUnit */
		elevationCoordsUnit = readInt(file, 6, aux);
		qDebug("elevationCoordsUnit:%d", elevationCoordsUnit);
		
		/* polygonSides */
		polygonSides = readInt(file, 6, aux);
		// qDebug("polygonSides:%d", polygonSides);
		if (polygonSides != 4) qFatal("No tratamos polygonSides diferentes de 4");
		
		/* coordenadas ? */
		double SEx, SEy, SWx, SWy, NWx, NWy, NEx, NEy;
		SEx = readDouble(file, 24, aux);
		SEy = readDouble(file, 24, aux);
		SWx = readDouble(file, 24, aux);
		SWy = readDouble(file, 24, aux);
		NWx = readDouble(file, 24, aux);
		NWy = readDouble(file, 24, aux);
		NEx = readDouble(file, 24, aux);
		NEy = readDouble(file, 24, aux);
		
		// qDebug("SE:%f %f", SEx, SEy);
		// qDebug("SW:%f %f", SWx, SWy);
		// qDebug("NW:%f %f", NWx, NWy);
		// qDebug("NE:%f %f", NEx, NEy);
		
		/* elevacion */
		double minEl, maxEl;
		minEl = readDouble(file, 24, aux);
		maxEl = readDouble(file, 24, aux);
		// qDebug("Minima Elevacion:%f", minEl);
		// qDebug("Maxima Elevacion:%f", maxEl);
		
		/* angulo de desviacion de noseque */
		double degree;
		degree = readDouble(file, 24, aux);
		// qDebug("AnguloDesv:%f", degree);
		
		/* accuracy */
		int accuracy;
		accuracy = readInt(file, 6, aux);
		// qDebug("Exactitud: %d", accuracy);
		
		/* resolution */
		double rx, ry, rz;
		rx = readDouble(file, 12, aux);
		ry = readDouble(file, 12, aux);
		rz = readDouble(file, 12, aux);
		qDebug("Resolucion X:%f", rx);
		qDebug("Resolucion Y:%f", ry);
		qDebug("Resolucion Z:%f", rz);
		
		Q_ASSERT(rx == ry);
		
		double conversionFactor = rz / rx;
		if (groundCoordsUnit != elevationCoordsUnit)
		{
			if (groundCoordsUnit == 1)
			{
				// groundCoordsUnit = 1
				// elevationCoordsUnit = 2
				// groundCoordsUnit = pies
				// elevationCoordsUnit = metros
				conversionFactor = conversionFactor / 0.3048;
			}
			else
			{
				// groundCoordsUnit = 2
				// elevationCoordsUnit = 1
				// groundCoordsUnit = metros
				// elevationCoordsUnit = pies
				conversionFactor = conversionFactor * 0.3048;
			}
		}
		qDebug("Factor conversion %f", conversionFactor);
		
		/* perfiles */
		int row, n;
		row = readInt(file, 6, aux);
		if (row != 1) qFatal("Hay mas de 1 fila");
		n = readInt(file, 6, aux);
		// qDebug("Columnas:%d", n);
		
		/*file.readBlock(aux, 5);
		aux[5] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 1);
		aux[1] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 5);
		aux[5] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 1);
		aux[1] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 4);
		aux[4] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 4);
		aux[4] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 1);
		aux[1] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 1);
		aux[1] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 2);
		aux[2] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 2);
		aux[2] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 2);
		aux[2] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 4);
		aux[4] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 4);
		aux[4] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 8);
		aux[8] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 7);
		aux[7] = '\0';
		printf("%s\n", aux);
		
		file.readBlock(aux, 109);
		*/
		
		file.readBlock(aux, 160);
		
		std::vector<double> *heights;
		/* Record 2 */
		for (int i = 0; i < n; i++)
		{
			int l;
			// qDebug("\nRecord Tipo B");
			l = 0;
		
			/* fila columna */
			int row, column;
			row = readInt(file, 6, aux);
			column = readInt(file, 6, aux);
			// qDebug("Fila:%d", row);
			// qDebug("Columna:%d", column);
			l += 12;
		
			int rows, columns;
			rows = readInt(file, 6, aux);
			// qDebug("Filas:%d", rows);
			columns = readInt(file, 6, aux);
			if (columns != 1) qFatal("Columnas es diferente de 1");
			// qDebug("Columnas:%d", columns);
			l += 12;
			
			double xgp, ygp;
			xgp = readDouble(file, 24, aux);
			ygp = readDouble(file, 24, aux);
			// qDebug("GP:%f %f", xgp,ygp);
			l += 48;
		
			double elevation;
			elevation = readDouble(file, 24, aux);
			// qDebug("Elevation:%f", elevation);
			l += 24;
		
			/* elevacion local */
			double minLocEl, maxLocEl;
			minLocEl = readDouble(file, 24, aux);
			maxLocEl = readDouble(file, 24, aux);
			// qDebug("Minima Elevacion Loc:%f", minLocEl);
			// qDebug("Maxima Elevacion Loc:%f", maxLocEl);
			l += 48;
			
			heights = new std::vector<double>;
		
			if (rows <= 146)
			{
				readHeights(file, rows, conversionFactor, heights, aux);
				l += rows*6;
				
				file.readBlock(aux, 1024 - l);
			}
			else
			{
				readHeights(file, 146, conversionFactor, heights, aux);
				file.readBlock(aux, 4);
				
				int restan, read;
				restan = rows - 146;
				while (restan > 0)
				{
					if (restan >= 170) read = 170;
					else read = restan;
					
					readHeights(file, read, conversionFactor, heights, aux);
					file.readBlock(aux, 1024 - read * 6);
					restan -= read;
				}
				
// 				falta = 1024 - (m*n-146)*6;
			}
		
			m->addColumn(heights);
// 			file.readBlock(aux, falta);
		}
		file.close();
		return m;
	}
	return 0;
}

QChar DEMParser::readChar(QFile &file)
{
	char aux;
	
	file.readBlock(&aux, 1);
	return aux;
}

double DEMParser::readDouble(QFile &file, int length, char *aux)
{
	QString s;
	s = readString(file, length, aux);
	s = s.replace("D", "E");
	return s.toDouble();
}

int DEMParser::readInt(QFile &file, int length, char *aux)
{
	file.readBlock(aux, length);
	aux[length] = '\0';
	return atoi(aux);
}

QString DEMParser::readString(QFile &file, int length, char *aux)
{
	QString res;
	
	file.readBlock(aux, length);
	aux[length] = '\0';
	res = aux;
	return res;
}

void DEMParser::readHeights(QFile &file, int howMany, double factor, std::vector<double> *v, char *aux)
{
	int height;
	
	for (int i = 0; i < howMany; i++)
	{
		height = readInt(file, 6, aux);
		v->push_back(height * factor);
	}
}


/* */

r3vMap *myParser::parse(QFile &file)
{
	if (file.open(IO_ReadOnly))
	{
		r3vMap *m = new r3vMap();
		
		QTextStream ts(&file);
		int columns = ts.readLine().toInt();
		std::vector<double> *heights;
		
		for (int i = 0; i < columns; i++)
		{
			heights = new std::vector<double>;
			for (int j = 0; j < columns; j++)
			{
				heights->push_back(ts.readLine().toDouble());
			}
			m -> addColumn(heights);
		}
		
		file.close();
		return m;
	}
	
	return 0;
}

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ROAM_H
#define ROAM_H

class QString;

class diamondList;
class observer;
class r3vMap;
class triangleList;

class ROAM
{
	public:
		ROAM(const QString &file);
		~ROAM();
	
		void paint();
		
		void moveObserverForward();
		void moveObserverBackward();
		void moveObserverLeft();
		void moveObserverRight();
		void rotateObserver(float x, float y);
		
		void mergeOne();
		void splitOne();
		
	private:
		void paintTriangle(const triangle *t) const;
		
		r3vMap *m_map;
		observer *m_observer;

		triangleList *m_splitQueue;
		diamondList *m_mergeQueue;
};

#endif

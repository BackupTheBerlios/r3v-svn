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

class diamondList;
class observer;
class r3vMap;
class triangle;
class triangleList;

class ROAM
{
	public:
		ROAM();
		~ROAM();
		
		// OK -> OK
		// openingError -> error opening the file
		// unknownFormat -> no parser for that kind of file
		enum Error {OK, openingError, unknownFormat};
		
		Error open(const std::string &file);
		void close();
		
		bool hasMap();
	
		void paint();
		
		void moveObserverForward();
		void moveObserverBackward();
		void moveObserverLeft();
		void moveObserverRight();
		void rotateObserver(float x, float y);
		
		void mergeOne();
		void splitOne();
		
		void renew();
		
	private:
		void clean();
		void paintTriangle(const triangle *t, bool color) const;
		
		r3vMap *m_map;
		observer *m_observer;

		triangleList *m_splitQueue;
		diamondList *m_mergeQueue;
};

#endif

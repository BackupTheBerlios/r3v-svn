/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef DIAMOND_H
#define DIAMOND_H

class map;
class triangle;

class diamond
{
	public:
		diamond(triangle *t1, triangle *t2);
		
		triangle *t1() const;
		triangle *t2() const;
		
	private:
		triangle *m_t1, *m_t2;
};

#endif

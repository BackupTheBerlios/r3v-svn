/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FRUSTUM_H
#define FRUSTUM_H

class frustum
{
	public:
		frustum(double *pjm, double *mvm);
	
	private:
		double m_frustum[6][4];
};

#endif

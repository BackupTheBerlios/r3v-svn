/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef OBSERVER_H
#define OBSERVER_H

class observer
{
	public:
		observer(float x, float y, float z, float rotX, float rotY, float step);
		
		void forward();
		void backward();
		void left();
		void right();
		
		void rotate(float x, float y);
		
		void position(float *x, float *y, float *z) const;
		void vrp(float *x, float *y, float *z) const;
		
	private:
		float m_posX, m_posY, m_posZ;
		float m_rotX, m_rotY;
		const float m_step;
		bool m_changed;
};

#endif

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <math.h>

#include "observer.h"

observer::observer(float x, float y, float z, float rotX, float rotY, float step) : m_posX(x), m_posY(y), m_posZ(z), m_rotX(rotX), m_rotY(rotY), m_step(step)
{
}

void observer::forward()
{
	m_posX -= m_step * sin(m_rotY) * cos(m_rotX);
	m_posY -= m_step * sin(m_rotX);
	m_posZ -= m_step * cos(m_rotY) * cos(m_rotX);
}

void observer::backward()
{
	m_posX += m_step * sin(m_rotY) * cos(m_rotX);
	m_posY += m_step * sin(m_rotX);
	m_posZ += m_step * cos(m_rotY) * cos(m_rotX);
}

void observer::left()
{
	m_posX += m_step * sin(m_rotY - M_PI / 2) * cos(m_rotX);
	m_posZ += m_step * cos(m_rotY - M_PI / 2) * cos(m_rotX);
}

void observer::right()
{
	m_posX += m_step * sin(m_rotY + M_PI / 2) * cos(m_rotX);
	m_posZ += m_step * cos(m_rotY + M_PI / 2) * cos(m_rotX);
}

/*#include <stdio.h>
void observer::pepe()
{
	printf("\n");
	printf("%f %f\n", m_rotX, m_rotY);
	printf("\n");
	printf("%f 0.000000 %f\n", cos(m_rotY), -sin(m_rotY));
	printf("%f %f %f\n", -sin(m_rotX)*sin(m_rotY), cos(m_rotX), -sin(m_rotX)*cos(m_rotY));
	printf("%f %f %f\n", sin(m_rotY)*cos(m_rotX), sin(m_rotX), cos(m_rotY)*cos(m_rotX));
}
*/
void observer::rotate(float x, float y)
{
	m_rotX += x;
	m_rotY += y;
	if (m_rotX > 1.545398) m_rotX = 1.545398;
	if (m_rotX < -1.545398) m_rotX = -1.545398;
}

void observer::position(float *x, float *y, float *z) const
{
	*x = m_posX;
	*y = m_posY;
	*z = m_posZ;
}

void observer::vrp(float *x, float *y, float *z) const
{
	*x = m_posX - cos(m_rotX) * sin(m_rotY);
	*y = m_posY - sin(m_rotX);
	*z = m_posZ - cos(m_rotX) * cos(m_rotY);
}

/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <e6732201@est.fib.upc.edu>    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <math.h>
#include <stdio.h>

#include "frustum.h"
#include "node.h"
#include "triangle.h"

frustum::frustum(double *pjm, double *mvm)
{
	double normal;
	double clip[16];
	
	for (int i = 0; i < 16; i++) m_mvm[i] = mvm[i];
	
	clip[ 0] = mvm[ 0] * pjm[ 0] + mvm[ 1] * pjm[ 4] + mvm[ 2] * pjm[ 8] + mvm[ 3] * pjm[12];
	clip[ 1] = mvm[ 0] * pjm[ 1] + mvm[ 1] * pjm[ 5] + mvm[ 2] * pjm[ 9] + mvm[ 3] * pjm[13];
	clip[ 2] = mvm[ 0] * pjm[ 2] + mvm[ 1] * pjm[ 6] + mvm[ 2] * pjm[10] + mvm[ 3] * pjm[14];
	clip[ 3] = mvm[ 0] * pjm[ 3] + mvm[ 1] * pjm[ 7] + mvm[ 2] * pjm[11] + mvm[ 3] * pjm[15];
	clip[ 4] = mvm[ 4] * pjm[ 0] + mvm[ 5] * pjm[ 4] + mvm[ 6] * pjm[ 8] + mvm[ 7] * pjm[12];
	clip[ 5] = mvm[ 4] * pjm[ 1] + mvm[ 5] * pjm[ 5] + mvm[ 6] * pjm[ 9] + mvm[ 7] * pjm[13];
	clip[ 6] = mvm[ 4] * pjm[ 2] + mvm[ 5] * pjm[ 6] + mvm[ 6] * pjm[10] + mvm[ 7] * pjm[14];
	clip[ 7] = mvm[ 4] * pjm[ 3] + mvm[ 5] * pjm[ 7] + mvm[ 6] * pjm[11] + mvm[ 7] * pjm[15];
	clip[ 8] = mvm[ 8] * pjm[ 0] + mvm[ 9] * pjm[ 4] + mvm[10] * pjm[ 8] + mvm[11] * pjm[12];
	clip[ 9] = mvm[ 8] * pjm[ 1] + mvm[ 9] * pjm[ 5] + mvm[10] * pjm[ 9] + mvm[11] * pjm[13];
	clip[10] = mvm[ 8] * pjm[ 2] + mvm[ 9] * pjm[ 6] + mvm[10] * pjm[10] + mvm[11] * pjm[14];
	clip[11] = mvm[ 8] * pjm[ 3] + mvm[ 9] * pjm[ 7] + mvm[10] * pjm[11] + mvm[11] * pjm[15];
	clip[12] = mvm[12] * pjm[ 0] + mvm[13] * pjm[ 4] + mvm[14] * pjm[ 8] + mvm[15] * pjm[12];
	clip[13] = mvm[12] * pjm[ 1] + mvm[13] * pjm[ 5] + mvm[14] * pjm[ 9] + mvm[15] * pjm[13];
	clip[14] = mvm[12] * pjm[ 2] + mvm[13] * pjm[ 6] + mvm[14] * pjm[10] + mvm[15] * pjm[14];
	clip[15] = mvm[12] * pjm[ 3] + mvm[13] * pjm[ 7] + mvm[14] * pjm[11] + mvm[15] * pjm[15];
	
	// right plane
	m_frustum[0][0] = clip[ 3] - clip[ 0];
	m_frustum[0][1] = clip[ 7] - clip[ 4];
	m_frustum[0][2] = clip[11] - clip[ 8];
	m_frustum[0][3] = clip[15] - clip[12];
	
	// left plane 
	m_frustum[1][0] = clip[ 3] + clip[ 0];
	m_frustum[1][1] = clip[ 7] + clip[ 4];
	m_frustum[1][2] = clip[11] + clip[ 8];
	m_frustum[1][3] = clip[15] + clip[12];
	
	// bottom plane 
	m_frustum[2][0] = clip[ 3] + clip[ 1];
	m_frustum[2][1] = clip[ 7] + clip[ 5];
	m_frustum[2][2] = clip[11] + clip[ 9];
	m_frustum[2][3] = clip[15] + clip[13];
	
	// top plane
	m_frustum[3][0] = clip[ 3] - clip[ 1];
	m_frustum[3][1] = clip[ 7] - clip[ 5];
	m_frustum[3][2] = clip[11] - clip[ 9];
	m_frustum[3][3] = clip[15] - clip[13];
	
	// far plane
	m_frustum[4][0] = clip[ 3] - clip[ 2];
	m_frustum[4][1] = clip[ 7] - clip[ 6];
	m_frustum[4][2] = clip[11] - clip[10];
	m_frustum[4][3] = clip[15] - clip[14];
	
	// near plane
	m_frustum[5][0] = clip[ 3] + clip[ 2];
	m_frustum[5][1] = clip[ 7] + clip[ 6];
	m_frustum[5][2] = clip[11] + clip[10];
	m_frustum[5][3] = clip[15] + clip[14];
	
	// normalize the planes
// 	for (int i = 0; i < 6; i++)
// 	{
// 		normal = sqrt(m_frustum[i][0] * m_frustum[i][0] + m_frustum[i][1] * m_frustum[i][1] + m_frustum[i][2] * m_frustum[i][2]);
// 		m_frustum[i][0] /= normal;
// 		m_frustum[i][1] /= normal;
// 		m_frustum[i][2] /= normal;
// 		m_frustum[i][3] /= normal;
// 	}
}

void frustum::setTriangleStatus(triangle *t) const
{
	int p;
	
	triangle::FRUSTUMSTATUS s = triangle::UNKNOWN;
	
	if (t -> parent() && t -> parent() -> frustumStatus() == triangle::COMPLETELYINSIDE) s = triangle::COMPLETELYINSIDE;
	else if (t -> parent() && t -> parent() -> frustumStatus() == triangle::OUTSIDE) s = triangle::OUTSIDE;
	else
	{
		int count = 0;
		for (p = 0; p < 4; p++)
		{
			int aux = 0;
			
			if (m_frustum[p][0] * t->apex()->getX() + m_frustum[p][1] * t->apex()->getHeight() + m_frustum[p][2] * t->apex()->getZ() + m_frustum[p][3] > 0) aux++;
			
			if (m_frustum[p][0] * t->leftVertex()->getX() + m_frustum[p][1] * t->leftVertex()->getHeight() + m_frustum[p][2] * t->leftVertex()->getZ() + m_frustum[p][3] > 0) aux++;
		
			if (m_frustum[p][0] * t->rightVertex()->getX() + m_frustum[p][1] * t->rightVertex()->getHeight() + m_frustum[p][2] * t->rightVertex()->getZ() + m_frustum[p][3] > 0) aux++;

			if (aux == 0)
			{
				// All three points of a triangle are behind the plane p so it is not visible
				s = triangle::OUTSIDE;
				break;
			}
			else if (aux == 3)
			{
				// All three points of a triangle are in front of the plane p
				count++;
			}
		}
		
		if (count == 4)
		{
			// All three points are in front of all the planes
			s = triangle::COMPLETELYINSIDE;
		}
		else if (s == triangle::UNKNOWN) s = triangle::INSIDE;
	}
	
	if (s == triangle::INSIDE)
	{
	}
	
	t->setFrustumStatus(s);
	
	if (t -> leftTriangle())
	{
		setTriangleStatus(t -> leftTriangle());
		setTriangleStatus(t -> rightTriangle());
	}
}

const double *frustum::modelViewMatrix() const
{
	return m_mvm;
}

#include <qstring.h>

#include "geopoint.h"

geoPoint::geoPoint(int degreesX, int minutesX, double secondsX, int degreesY, int minutesY, double secondsY) : m_degreesX(degreesX), m_minutesX(minutesX), m_secondsX(secondsX), m_degreesY(degreesY), m_minutesY(minutesY), m_secondsY(secondsY)
{
}

QString geoPoint::asString() const
{
	return QString("X %1:%2:%3  Y %4:%5:%6").arg(m_degreesX).arg(m_minutesX).arg(m_secondsX).arg(m_degreesY).arg(m_minutesY).arg(m_secondsY);
}

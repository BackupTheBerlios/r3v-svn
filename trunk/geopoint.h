class geoPoint
{
	public:
		geoPoint(int degreesX, int minutesX, double secondsX, int degreesY, int minutesY, double secondsY);
		
		QString asString() const;
	
	private:
		int m_degreesX;
		int m_minutesX;
		double m_secondsX;
		int m_degreesY;
		int m_minutesY;
		double m_secondsY;
};

#include "Timer.h"
#include <algorithm>

Timer::Timer(float timeSMax, float timeSCurrent) :
	m_TimeSMax(timeSMax), m_TimeSCurrent(timeSCurrent) {

}


void Timer::setTimeMax(float timeSMax)
{
	m_TimeSMax = timeSMax;
}

void Timer::countUp(float dT) {
	if (m_TimeSCurrent < m_TimeSMax) {
		m_TimeSCurrent += dT;
		m_TimeSCurrent = std::min(m_TimeSCurrent, m_TimeSMax);
	}
}


void Timer::countDown(float dT) {
	if (m_TimeSCurrent > 0.0f) {
		m_TimeSCurrent -= dT;
		m_TimeSCurrent = std::max(0.0f, m_TimeSCurrent);
	}
}

void Timer::resetToZero() {
	m_TimeSCurrent = 0.0f;
}


void Timer::resetToMax() {
	m_TimeSCurrent = m_TimeSMax;
}



bool Timer::timeSIsZero() const {
	return (m_TimeSCurrent <= 0.0f);
}


bool Timer::timeSIsGreaterOrEqual(float timeSCheck) const {
	return (m_TimeSCurrent >= timeSCheck);
}

float Timer::getCurrentTime() const
{
	return m_TimeSCurrent;
}

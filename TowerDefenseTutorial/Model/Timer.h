#pragma once

class Timer
{
private:
	float m_TimeSMax;
	float m_TimeSCurrent;

public:
	Timer(float timeSMax, float timeSCurrent = 0.0f);

	void setTimeMax(float timeSMax);
	void countUp(float dT);
	void countDown(float dT);
	void resetToZero();
	void resetToMax();
	bool timeSIsZero() const;
	bool timeSIsGreaterOrEqual(float timeSCheck) const;
	float getCurrentTime() const;
};

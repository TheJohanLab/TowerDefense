#pragma once

class Timer
{
private:
	float timeSMax;
	float timeSCurrent;

public:
	Timer(float setTimeSMax, float setTimeSCurrent = 0.0f);

	void countUp(float dT);
	void countDown(float dT);
	void resetToZero();
	void resetToMax();
	bool timeSIsZero();
	bool timeSIsGreaterOrEqual(float timeSCheck);
};

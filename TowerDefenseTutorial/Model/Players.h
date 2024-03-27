#pragma once

#include <iostream>

class Player
{
private:
	uint8_t m_LifePoints;
	uint8_t m_Score;
	uint8_t m_Coins;

public:
	Player(uint8_t MaxlifePoints)
		:m_LifePoints(MaxlifePoints), m_Score(0), m_Coins(200)
	{}

	~Player() {}

	const uint8_t* getLifePoints() const { return &m_LifePoints; }
	void setLifePoints(uint8_t lifePoints) { m_LifePoints = lifePoints; }
};
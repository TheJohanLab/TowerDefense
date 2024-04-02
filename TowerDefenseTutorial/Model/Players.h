#pragma once

#include <iostream>

class Player
{
private:

	uint8_t m_LifePoints;
	uint8_t m_Score;
	uint8_t m_maxGems;
	uint8_t m_Gems;

public:
	Player(uint8_t MaxlifePoints, uint8_t maxGems)
		:m_LifePoints(MaxlifePoints), m_Score(0), m_Gems(maxGems), m_maxGems(maxGems)
	{}

	~Player() {}

	uint8_t getLifePoints() const { return m_LifePoints; }
	uint8_t getGems() const { return m_Gems; }
	void setGems(uint8_t gems) { m_Gems = gems;  }
	uint8_t getMaxGems() const { return m_maxGems; }
	void setLifePoints(uint8_t lifePoints) { m_LifePoints = lifePoints; }
};
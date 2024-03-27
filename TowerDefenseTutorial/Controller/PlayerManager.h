#pragma once

#include <functional>
#include "../Model/Players.h"

class PlayerManager
{

private:
	Player m_Player;


public:
	std::function<void()> onPlayersDeath;

	PlayerManager(uint8_t maxLifePoints);
	~PlayerManager();

	void setOnPlayersDeathCallback(std::function<void()> callback);

	const uint8_t* getpPlayerLifePoints();

	void takeDamages(uint8_t damages);



};


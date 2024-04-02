#pragma once

#include <functional>
#include "../Model/Players.h"
#include "../Model/Timer.h"

class PlayerManager
{

private:
	Player m_Player;
	Timer m_gemsRegenerationTimer;

public:
	std::function<void()> onPlayersDeath;

	PlayerManager(uint8_t maxLifePoints, uint8_t maxGems);
	~PlayerManager();

	void setOnPlayersDeathCallback(std::function<void()> callback);

	const Player* getPlayer() const;

	uint8_t getCurrentGems() const;
	void takeDamages(uint8_t damages);
	void loseGems(uint8_t gems);

	void update(float dT);


};


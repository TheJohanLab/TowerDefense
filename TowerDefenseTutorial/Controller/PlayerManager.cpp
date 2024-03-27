#include "PlayerManager.h"

PlayerManager::PlayerManager(uint8_t maxLifePoints)
	:m_Player(Player(maxLifePoints))
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::setOnPlayersDeathCallback(std::function<void()> callback)
{
	onPlayersDeath = callback;
}

const uint8_t* PlayerManager::getpPlayerLifePoints()
{
	return m_Player.getLifePoints();
}

void PlayerManager::takeDamages(uint8_t damages)
{
	uint8_t newHealthPoints = std::max(0, *getpPlayerLifePoints() - damages);

	m_Player.setLifePoints(newHealthPoints);

	if (newHealthPoints == 0)
		onPlayersDeath();
}



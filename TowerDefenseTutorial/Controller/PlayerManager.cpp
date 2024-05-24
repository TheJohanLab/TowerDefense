#include "PlayerManager.h"
#include <algorithm>
#include "../Utils/Utils.h"

PlayerManager::PlayerManager(uint8_t maxLifePoints, uint8_t maxGems)
	:m_Player(Player(maxLifePoints, maxGems)), m_gemsRegenerationTimer(Timer(float(GEM_REGENERATION / 1000.0f )))
{
	m_gemsRegenerationTimer.resetToMax();
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::setOnPlayersDeathCallback(std::function<void()> callback)
{
	onPlayersDeath = callback;
}

const Player* PlayerManager::getPlayer() const
{
	return &m_Player;
}

uint8_t PlayerManager::getCurrentGems() const
{
	return m_Player.getGems();
}

void PlayerManager::takeDamages(uint8_t damages)
{
	uint8_t newHealthPoints = std::max(0, m_Player.getLifePoints() - damages);

	m_Player.setLifePoints(newHealthPoints);

	if (newHealthPoints == 0)
		onPlayersDeath();
}

void PlayerManager::loseGems(uint8_t gems)
{
	m_Player.setGems(std::max(m_Player.getGems() - gems, 0));
}

void PlayerManager::update(float dT)
{
	if (m_Player.getGems() < m_Player.getMaxGems())
	{
		m_gemsRegenerationTimer.countDown(dT);

		if (m_gemsRegenerationTimer.timeSIsZero())
		{
			m_Player.setGems(std::min(uint8_t(m_Player.getGems()+1), m_Player.getMaxGems()));
			m_gemsRegenerationTimer.resetToMax();
		}
	}
}



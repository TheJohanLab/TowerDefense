#include "GameStatus.h"

GameStatus::GameStatus(GameState gameState)
	:m_LifePoints(m_MaxLifePoints), m_GameState(gameState)
{
}

GameStatus::~GameStatus()
{
}

GameState& GameStatus::getGameState()
{
	return m_GameState;
}

void GameStatus::setGameState(GameState gameState)
{
	m_GameState = gameState;
}

uint8_t* GameStatus::getpHealth()
{
	return &m_LifePoints;
}



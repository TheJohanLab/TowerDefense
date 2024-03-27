#include "GameStatus.h"

GameStatus::GameStatus()
	:m_LifePoints(m_MaxLifePoints), m_GameState(GameState::STOPPED)
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



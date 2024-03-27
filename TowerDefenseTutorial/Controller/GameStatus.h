#pragma once
#include <iostream>

#include "../Utils/Utils.h"

class GameStatus
{
private:
	const int m_MaxLifePoints = MAX_LIFE_POINTS;
	uint8_t m_LifePoints;
	GameState m_GameState;

public:
	GameStatus();
	~GameStatus();


	GameState& getGameState();
	void setGameState(GameState gameState);

	static bool isGamePaused();

};


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
	GameStatus(GameState gameState);
	~GameStatus();


	GameState& getGameState();
	void setGameState(GameState gameState);

	uint8_t* getpHealth();
	static bool isGamePaused();


};


#pragma once

#include "GameStatus.h"
#include "SDL2/SDL.h"

class Game;

class GameLoop
{
private:
	GameStatus& m_GameStatus;
	Game& m_GameManager;

public:
	GameLoop(Game& game, GameStatus& gameStatus);
	~GameLoop();

	void start(SDL_Renderer* renderer);
	void pause();
	void resume();
	void stop();
};


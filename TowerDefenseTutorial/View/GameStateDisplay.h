#pragma once

#include "SDL2/SDL.h"
#include "../Utils/Utils.h"
#include "../Model/Timer.h"

class GameStateDisplay
{
private:

	const int maxTimerValue = 10;

	uint16_t m_WindowWidth;
	uint16_t m_WindowHeight;

	SDL_Texture* m_startLevelTexture = nullptr;
	SDL_Texture* m_startWaveTexture = nullptr;
	SDL_Texture* m_VictoryTexture = nullptr;
	SDL_Texture* m_GameOverTexture = nullptr;

	GameState* m_GameState;

	Timer m_showDisplayTimer;

public:
	GameStateDisplay(SDL_Renderer* renderer, GameState* gameState, uint16_t windowWidth, uint16_t windowHeight);
	~GameStateDisplay();

	void draw(SDL_Renderer* renderer) const;
	void resetTimer();
	void countDownTimer(float dT);

private:

	void drawDisplay(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t alpha = 255) const;

};


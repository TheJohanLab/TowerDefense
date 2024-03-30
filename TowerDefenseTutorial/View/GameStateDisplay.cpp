#include "GameStateDisplay.h"
#include "../Controller/TextureLoader.h"
#include "Window.h"

#include <iostream>

GameStateDisplay::GameStateDisplay(SDL_Renderer* renderer, GameState* gameState, uint16_t windowWidth, uint16_t windowHeight)
	:m_GameState(gameState), m_showDisplayTimer(10), m_WindowWidth(windowWidth), m_WindowHeight(windowHeight)
{
	m_VictoryTexture = TextureLoader::loadTexture(renderer, "Victory.bmp");
	m_GameOverTexture = TextureLoader::loadTexture(renderer, "GameOver.bmp");
	m_startLevelTexture = TextureLoader::loadTexture(renderer, "StartLevel.bmp");
	m_startWaveTexture = TextureLoader::loadTexture(renderer, "StartWave.bmp");
	m_showDisplayTimer.resetToZero();
}

GameStateDisplay::~GameStateDisplay()
{
}

void GameStateDisplay::draw(SDL_Renderer* renderer) const
{
	switch (*m_GameState)
	{
	case GameState::INIT:
		drawDisplay(renderer, m_startLevelTexture, 150);
		break;
	case GameState::WAITING:
		if (!m_showDisplayTimer.timeSIsZero())
			drawDisplay(renderer, m_startWaveTexture, m_showDisplayTimer.getCurrentTime() * (150 / maxTimerValue));
		break;
	case GameState::VICTORY:
		drawDisplay(renderer, m_VictoryTexture, 150);
		break;
	case GameState::GAMEOVER:
		drawDisplay(renderer, m_GameOverTexture, 150);
		break;
	default:
		break;
	}
	
}

void GameStateDisplay::resetTimer()
{
	m_showDisplayTimer.resetToMax();
}

void GameStateDisplay::countDownTimer(float dT)
{
	m_showDisplayTimer.countDown(dT);
}

void GameStateDisplay::drawDisplay(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t alpha) const
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	SDL_Rect rect =
	{
		(m_WindowWidth / 2) - w/2, 
		(m_WindowHeight /2) - h/2,
		w,
		h
	};

	SDL_SetTextureAlphaMod(texture, alpha);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
}


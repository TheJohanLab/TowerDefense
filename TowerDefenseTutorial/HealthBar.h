#pragma once

#include <iostream>
#include "Vector2D.h"
#include "SDL2/SDL.h"

class HealthBar
{
private:
	uint8_t m_MaxHealth;
	uint8_t m_CurrentHealth;

public:
	HealthBar();
	HealthBar(uint8_t maxHealth);
	~HealthBar();

	void setHealth(uint8_t healthValue);

	void draw(SDL_Renderer* renderer, float posX, float posY) const;
};


#pragma once

#define BAT_MAX_HEALTH 3
#define BAT_DAMAGES 1
#define BAT_SPEED 150
#define BAT_SIZE 24


#include "Unit.h"

class Bat : public Unit
{
protected:

public:
	Bat(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> onDestroyedCallback)
		:Unit(renderer, pos, onDestroyedCallback, TextureLoader::loadTexture(renderer, "Bat.bmp"),
			BAT_DAMAGES, BAT_SPEED * 0.01f, BAT_SIZE * 0.01f, BAT_MAX_HEALTH, SpriteAnimation(0,6,100,36,32))
	{}
};

#pragma once

#define MUSHROOM_MAX_HEALTH 6
#define MUSHROOM_DAMAGES 2
#define MUSHROOM_SPEED 60
#define MUSHROOM_SIZE 30


#include "Unit.h"

class Mushroom : public Unit
{
protected:

public:
	Mushroom(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> onDestroyedCallback)
		:Unit(renderer, pos, onDestroyedCallback, TextureLoader::loadTexture(renderer, "mushroom.bmp"), 
			MUSHROOM_DAMAGES, MUSHROOM_SPEED*0.01f, MUSHROOM_SIZE*0.01f, MUSHROOM_MAX_HEALTH, SpriteAnimation(0, 7, 100, 48, 45))
	{}
};

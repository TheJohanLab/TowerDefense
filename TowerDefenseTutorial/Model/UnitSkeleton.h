#pragma once

#define MAX_HEALTH 10
#define DAMAGES 3
#define SPEED 50
#define SIZE 40

#include "Unit.h"

class Skeletton : public Unit
{
protected:

public:
	Skeletton(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> onDestroyedCallback)
		:Unit(renderer, pos, onDestroyedCallback, TextureLoader::loadTexture(renderer, "skeletton.bmp"),
			DAMAGES, SPEED * 0.01f, SIZE * 0.01f, MAX_HEALTH, SpriteAnimation(0, 8, 100, 52, 45))
	{}
};

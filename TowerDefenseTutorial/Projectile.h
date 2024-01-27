#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Unit.h"

class Projectile
{
public:
	Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal);
	void update(float dT);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool getCollisionOccured() const;

private:
	Vector2D pos, directionNormal;
	static const float speed, size, distanceTraveledMax;
	float distanceTraveled = 0.0f;

	SDL_Texture* texture = nullptr;

	bool collisionOccured = false;
};


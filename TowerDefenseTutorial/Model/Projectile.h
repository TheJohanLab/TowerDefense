#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "../Utils/Vector2D.h"
#include "../Controller/TextureLoader.h"
#include "Unit.h"

class Projectile
{
public:
	Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal, int offsetX, int offsetY, SDL_Texture* projectileTexture);
	void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool getCollisionOccured() const;

private:
	Vector2D m_Pos, m_DirectionNormal;
	static const float speed, size, distanceTraveledMax;
	float m_DistanceTraveled = 0.0f;
	int m_OffsetX, m_OffsetY;

	SDL_Texture* m_Texture = nullptr;

	bool m_CollisionOccured = false;

	void checkCollisions(std::vector<std::shared_ptr<Unit>>& listUnits);
};


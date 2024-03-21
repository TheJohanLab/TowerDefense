#pragma once
#include <vector>
#include <memory>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"
#include "Timer.h"
#include "HealthBar.h"

#define MAX_HEALTH (uint8_t)10
class Game;


class Unit
{
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	bool isAlive() const;
	Vector2D getPos() const;

	void removeHealth(int damage);

private:
	Vector2D pos;
	static const float speed;
	static const float size;
	const int maxHealth = MAX_HEALTH;
	HealthBar m_HealthBar;

	SDL_Texture* texture = nullptr;

	Timer hitTimer;

	int currentHealth = maxHealth;
};
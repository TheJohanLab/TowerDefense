#pragma once

#include <memory>
#include "TextureLoader.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "MathAddon.h"
#include "Unit.h"

class Turret
{
public:
	Turret(SDL_Renderer* renderer, Vector2D setPos);
	
	void update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y) const;

private:
	Vector2D pos;
	float angle;
	static const float speedAngular, weaponRange;

	std::weak_ptr<Unit> targetUnit;
	SDL_Texture
		* textureMain = nullptr,
		* textureShadow = nullptr;

	void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected, int offset, int tileSize);
	std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);
};


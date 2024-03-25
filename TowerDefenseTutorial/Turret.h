#pragma once

#include <memory>
#include "TextureLoader.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "MathAddon.h"
#include "Unit.h"
#include "Projectile.h"
#include "Timer.h"

class Turret
{
public:
	Turret(SDL_Renderer* renderer, Vector2D setPos);
	
	void update(SDL_Renderer* renderer, float dT, 
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y) const;

private:
	Vector2D pos;
	float angle;
	static const float speedAngular, weaponRange;
	Timer timerWeapon;

	std::weak_ptr<Unit> targetUnit;
	SDL_Texture
		* textureMain = nullptr,
		* textureShadow = nullptr,
		* textureRange = nullptr;

	void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected, int offset, int tileSize);
	void drawTurretRange(SDL_Renderer* renderer, int tileSize);
	std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);
	bool updateAngle(float dT);
	void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);
};


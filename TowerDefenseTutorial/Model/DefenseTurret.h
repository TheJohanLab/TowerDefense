#pragma once

#include "Defense.h"
#include <memory>
#include "SDL2/SDL.h"
#include "../Controller/TextureLoader.h"
#include "../Utils/Vector2D.h"
#include "../Utils/MathAddon.h"
#include "Unit.h"
#include "Projectile.h"
#include "Timer.h"

class Turret : public Defense
{
private:
	SDL_Texture* m_TurretHeadTexture = nullptr;
	SDL_Texture* m_ProjectileTexture = nullptr;
public:

	Turret(SDL_Renderer* renderer, Vector2D pos);
	~Turret();

	virtual void update(SDL_Renderer* renderer, float dT,
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles) override;
	virtual void draw(SDL_Renderer* renderer, int tileSize) override;

private:
	void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);
	void drawTurretHead(SDL_Renderer* renderer, uint8_t tileSize) const;
	void drawTurretBase(SDL_Renderer* renderer, uint8_t tileSize) const;

public:
	/*Turret(SDL_Renderer* renderer, Vector2D setPos);
	
	void update(SDL_Renderer* renderer, float dT, 
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y) const;*/

private:
	//Vector2D pos;
	//float angle;
	//static const float speedAngular, weaponRange;
	//Timer timerWeapon;

	//std::weak_ptr<Unit> targetUnit;
	//SDL_Texture
	//	* m_TurretHeadTexture = nullptr,
	//	* m_TurretBasetexture = nullptr,
	//	* m_TurretWeaponRangeTexture = nullptr;

	//void drawMovingTexture(SDL_Renderer* renderer, SDL_Texture* textureSelected, int tileSize);
	//void drawStaticTexture(SDL_Renderer* renderer, SDL_Texture* textureSelected, int tileSize);
	//void drawRangeTexture(SDL_Renderer* renderer, SDL_Texture* textureSelected, int tileSize, int alpha = 255);
	//std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);
	//bool updateAngle(float dT);
	//void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);
};


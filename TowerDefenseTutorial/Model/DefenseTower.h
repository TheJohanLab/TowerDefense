#pragma once

#include "SDL2/SDL.h"
#include "Defense.h"

class Tower : public Defense
{
private:
	SDL_Texture* m_ProjectileTexture = nullptr;

public:
	Tower(SDL_Renderer* renderer, Vector2D pos);
	~Tower();

	virtual void update(SDL_Renderer* renderer, float dT,
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles) override ;
	virtual void draw(SDL_Renderer* renderer, int tileSize) override;

private:
	void shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles);
	void drawTower(SDL_Renderer* renderer, uint8_t tileSize) const;
	
};


#pragma once

#include "Defense.h"
#include "Timer.h"
#include "SpriteAnimation.h"

class Explosion: public Defense
{
private:
	Timer m_BombTimer, m_ExplosionTimer;
	SDL_Texture* m_ExplosionTexture;
	bool m_Exploded;

	SpriteAnimation m_Animation;
	bool m_BombRedState = false;

public:
	Explosion(SDL_Renderer* renderer, Vector2D pos);
	~Explosion();

	virtual void update(SDL_Renderer* renderer, float dT,
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles) override;
	virtual void draw(SDL_Renderer* renderer, int tileSize) override;

private:

	void drawBomb(SDL_Renderer* renderer, uint8_t tileSize) ;
	void drawExplosion(SDL_Renderer* renderer, uint8_t tileSize) const;

};


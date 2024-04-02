#pragma once

#include <memory>
#include "SDL2/SDL.h"
#include "../Utils/Vector2D.h"
#include "../Model/Timer.h"
#include "Unit.h"
#include "Projectile.h"

class Defense
{
protected:
	Vector2D m_Pos;
	Timer m_TimerWeapon, m_PlacementCooldown;
	uint8_t m_Damages;
	uint8_t m_WeaponRange;
	float m_WeaponAngle, m_SpeedAngular;
	uint8_t m_ItemPrice;
	float m_Cooldown;
	bool m_Alive = true;

	std::weak_ptr<Unit> targetUnit;

	SDL_Texture* m_Texture = nullptr;
	SDL_Texture* m_TexturePreview = nullptr;
	SDL_Texture* m_WeaponRangeTexture = nullptr;

	Defense(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* texturePreview, Vector2D pos, 
		float timerWeapon, uint8_t damages, uint8_t weaponRange, float speedAngular, uint8_t itemPrice, float placementCooldown);
	std::weak_ptr<Unit> findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits);
	bool updateAngle(float dT);


public:
	
	~Defense();

	virtual void update(SDL_Renderer* renderer, float dT,
		std::vector<std::shared_ptr<Unit>>& listUnits,
		std::vector<Projectile>& listProjectiles) = 0;
	virtual void draw(SDL_Renderer* renderer, int tileSize) = 0;

	bool checkIfOnTile(int x, int y) const;

	void drawWeaponRange(SDL_Renderer* renderer, int tileSize, int posX, int posY) const;

	bool isAlive() const;

	SDL_Texture* getTexture() const;
	SDL_Texture* getPreviewTexture() const;

	bool isCooldownReady() const;
	void countDownCooldown(float dT);
	void resetCooldown();
	float getMaxCooldown() const;
	Timer getCooldownTimer() const;
	uint8_t getPrice() const;
};


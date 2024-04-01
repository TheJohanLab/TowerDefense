#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "SDL2/SDL.h"
#include "../Utils/Vector2D.h"

#include "../Controller/TextureLoader.h"
#include "Timer.h"
#include "HealthBar.h"
#include "SpriteAnimation.h"

#include "../Listeners/OnBaseReachedListener.h"
#include "../Listeners/OnDestroyUnitListener.h"

#define MAX_HEALTH (uint8_t)4
class Game;
class Level;

enum class UnitType
{
	MUSHROOM,
	BAT,
	SKELETTON
};

class Unit
{
public:
	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	bool isAlive() const;
	Vector2D getPos() const;

	void removeHealth(int damage);

protected:
	Unit(SDL_Renderer* renderer, Vector2D setPos, std::function<void(uint8_t)> onDestroyedCallback, SDL_Texture* texture, uint8_t damages, 
		float speed, float size, uint8_t maxHealth, SpriteAnimation animation);

	Vector2D m_Pos;
	const float m_Speed;
	const float m_Size;
	const uint8_t m_Damages;
	const uint8_t m_MaxHealth;
	int currentHealth = m_MaxHealth;
	HealthBar m_HealthBar;

	Vector2D m_CurrDirection;

	SDL_Texture* m_Texture = nullptr;
	SpriteAnimation m_Animation;

	Timer m_HitTimer;


	std::function<void(uint8_t)> m_onUnitDestroyedCallback;
	OnBaseReachedListener onBaseReachedObserver;
	OnDestroyUnitListener onDestroyUnitObserver;

};
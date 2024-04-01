#include "DefenseTower.h"
#include "../Controller/TextureLoader.h"
#include "../Utils/MathAddon.h"

Tower::Tower(SDL_Renderer* renderer, Vector2D pos)
	:Defense(renderer, TextureLoader::loadTexture(renderer, "Wall.bmp"), pos, 2.0f, 1, 4, MathAddon::angleDegToRad(180.0f))
{
	m_ProjectileTexture = TextureLoader::loadTexture(renderer, "TowerProjectile.bmp");
}

Tower::~Tower()
{
}

void Tower::update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits, std::vector<Projectile>& listProjectiles)
{
	m_TimerWeapon.countDown(dT);

	//check if a target has been found but is no longer alive or is out of weapon range
	if (auto targetUnitSP = targetUnit.lock())
	{
		if (!targetUnitSP->isAlive() || (targetUnitSP->getPos() - m_Pos).magnitude() > m_WeaponRange)
		{
			targetUnit.reset();
		}
	}

	//find the enemy unit
	if (targetUnit.expired())
		targetUnit = findEnemyUnit(listUnits);

	//update the angle and shoot a projectile if needed
	if (updateAngle(dT))
		shootProjectile(renderer, listProjectiles);
}

void Tower::draw(SDL_Renderer* renderer, int tileSize)
{
	drawTower(renderer, tileSize);
	drawWeaponRange(renderer, tileSize);
}


void Tower::drawTower(SDL_Renderer* renderer, uint8_t tileSize) const
{

	int w, h;
	SDL_QueryTexture(m_Texture, NULL, NULL, &w, &h);
	SDL_Rect rect =
	{
		(int)m_Pos.x * tileSize,
		(int)m_Pos.y * tileSize,
		w,
		h
	};
	SDL_RenderCopy(renderer, m_Texture, NULL, &rect);
	
}

void Tower::shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles)
{
	//Shoot a projectile towards the target unit if the weapon timer is ready
	if (m_TimerWeapon.timeSIsZero())
	{
		listProjectiles.push_back(Projectile(renderer, m_Pos, Vector2D(m_WeaponAngle), 0, 0, m_ProjectileTexture));

		m_TimerWeapon.resetToMax();
	}
}




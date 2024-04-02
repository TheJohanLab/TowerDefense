#include "DefenseTurret.h"
#include "../Controller/TextureLoader.h"
#include "../Utils/Utils.h"


//const float Turret::speedAngular = MathAddon::angleDegToRad(180.0f);
//const float Turret::weaponRange = 2.0f;

//Turret::Turret(SDL_Renderer* renderer, Vector2D setPos)
//	:pos(setPos), angle(0.0f), timerWeapon(0.8f)
//{
//	m_TurretHeadTexture = TextureLoader::loadTexture(renderer, "TurretHead.bmp");
//	m_TurretBasetexture = TextureLoader::loadTexture(renderer, "WallTurretBase2.bmp");
//	m_TurretWeaponRangeTexture = TextureLoader::loadTexture(renderer, "TurretRange.bmp");
//}

Turret::Turret(SDL_Renderer* renderer, Vector2D pos)
	:Defense(renderer, 
		TextureLoader::loadTexture(renderer, "Turret.bmp"),
		TextureLoader::loadTexture(renderer, "TurretPreview.bmp"),
		pos, 0.7f, 2, 2, MathAddon::angleDegToRad(180.0f), 2, 2.0f)
{

	m_TurretBaseTexture = TextureLoader::loadTexture(renderer, "TurretBase.bmp");
	m_TurretHeadTexture = TextureLoader::loadTexture(renderer, "TurretHead.bmp");
	m_ProjectileTexture = TextureLoader::loadTexture(renderer, "Projectile.bmp");
}

Turret::~Turret()
{
}

void Turret::update(SDL_Renderer* renderer, float dT, 
	std::vector<std::shared_ptr<Unit>>& listUnits,
	std::vector<Projectile>& listProjectiles)
{
	/* angle += speedAngular * dT;
	if (angle > MathAddon::angleDegToRad(360.0f))
		angle -= MathAddon::angleDegToRad(360.0f);
	*/


	m_PlacementCooldown.countDown(dT);
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

void Turret::draw(SDL_Renderer* renderer, int tileSize)
{
	drawTurretBase(renderer, tileSize);
	drawTurretHead(renderer, tileSize);
	//drawWeaponRange(renderer, tileSize);
}

//bool Turret::updateAngle(float dT)
//{
//	if (auto targetUnitSP = targetUnit.lock())
//	{
//		//determine the direction normal to the target
//		Vector2D directionNormalToTarget = (targetUnitSP->getPos() - pos).normalize();
//
//		//Determine the angle to the target
//		float angleToTarget = directionNormalToTarget.angleBetween(Vector2D(angle));
//
//		//update the angle as required
//
//		//determine the angle to move this frame
//		float angleMove = -copysign(speedAngular * dT, angleToTarget);
//		if (abs(angleMove) > abs(angleToTarget))
//		{
//			//It will point directly at its target this frame
//			angle = directionNormalToTarget.angle();
//			return true;
//		}
//		else
//		{
//			//It won't reach its target this frame
//			angle += angleMove;
//		}
//		
//	}
//
//	return false;
//}

void Turret::shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles)
{
	//Shoot a projectile towards the target unit if the weapon timer is ready
	if (m_TimerWeapon.timeSIsZero())
	{
		listProjectiles.push_back(Projectile(renderer, m_Pos, Vector2D(m_WeaponAngle), (TILE_SIZE * 0.4), -(TILE_SIZE*0.25), m_ProjectileTexture));

		m_TimerWeapon.resetToMax();
	}
	//(int)(pos.x * tileSize) - (tileSize / 2) + ((tileSize-w) / 2),
//(int)(pos.y * tileSize) - (tileSize / 2) - (h * 0.7),
}

void Turret::drawTurretHead(SDL_Renderer* renderer, uint8_t tileSize) const
{
	if (renderer != nullptr && m_TurretHeadTexture != nullptr)
	{
		// Draw the image at the turret's position and angle
		int w, h;
		SDL_QueryTexture(m_TurretHeadTexture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			((int)m_Pos.x * tileSize)  + (w / 2),
			((int)m_Pos.y * tileSize)  - (h * 0.7),
			w,
			h
		};

		SDL_RenderCopyEx(renderer, m_TurretHeadTexture, NULL, &rect,
			MathAddon::angleRadToDeg(m_WeaponAngle), NULL, SDL_FLIP_NONE);
	}
}

//void Turret::draw(SDL_Renderer* renderer, int tileSize)
//{
//	//drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
//	drawStaticTexture(renderer, m_TurretBasetexture, tileSize);
//	drawMovingTexture(renderer, m_TurretHeadTexture, tileSize);
//	drawRangeTexture(renderer, m_TurretWeaponRangeTexture, tileSize, 30);
//}

/*bool Turret::checkIfOnTile(int x, int y) const
{
	return ((int)pos.x == x && (int)pos.y == y);
}*/

//void Turret::drawMovingTexture(SDL_Renderer* renderer, SDL_Texture* textureSelected, int tileSize)
//{
//	if (renderer != nullptr && textureSelected != nullptr)
//	{
//		// Draw the image at the turret's position and angle
//		int w, h;
//		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
//		SDL_Rect rect =
//		{
//			(int)(pos.x * tileSize) - (tileSize / 2) + (w / 2),
//			(int)(pos.y * tileSize) - (tileSize / 2) - (h * 0.7),
//			w,
//			h
//		};
//		
//		SDL_RenderCopyEx(renderer, textureSelected, NULL, &rect,
//			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
//	}
//}

void Turret::drawTurretBase(SDL_Renderer* renderer, uint8_t tileSize) const
{
	if (renderer != nullptr && m_TurretBaseTexture != nullptr)
	{
		// Draw the image at the turret's position and angle
		int w, h;
		SDL_QueryTexture(m_TurretBaseTexture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)m_Pos.x * tileSize ,
			(int)m_Pos.y * tileSize - w*0.35,
			w,
			h
		};

		SDL_RenderCopy(renderer, m_TurretBaseTexture, NULL, &rect);
	}
}

//void Turret::drawRangeTexture(SDL_Renderer* renderer, SDL_Texture* textureSelected, int tileSize, int alpha)
//{
//	if (renderer != nullptr && textureSelected != nullptr)
//	{
//		// Draw the image at the turret's position and angle
//		int w, h;
//		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
//		SDL_Rect rect =
//		{
//			(int)(pos.x * tileSize) - weaponRange * tileSize,
//			(int)(pos.y * tileSize) - weaponRange * tileSize,
//			weaponRange * 2 * tileSize,
//			weaponRange * 2 * tileSize
//		};
//
//		SDL_SetTextureAlphaMod(textureSelected, alpha);
//		SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
//	}
//}

//std::weak_ptr<Unit> Turret::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits)
//{
//	//find the closest Enemy for this turret
//
//	std::weak_ptr<Unit> closestUnit;
//	float closestDistance = 0.0f;
//
//	//Loop through the entire list of units
//	for (auto& selectedUnit : listUnits)
//	{
//		if (selectedUnit != nullptr)
//		{
//			//calculate the distance to the enemy unit
//			float currentDistance = (pos - selectedUnit->getPos()).magnitude();
//			//Check of the unit is within range, and no closest unit has been detected 
//			//or the selected unit is closer than the previous closest unit
//			if (currentDistance <= weaponRange &&
//				(closestUnit.expired() || currentDistance < closestDistance))
//			{
//				closestUnit = selectedUnit;
//				closestDistance = currentDistance;
//			}
//
//		}
//	}
//	return closestUnit;
//}


//bool Turret::checkIfOnTile(int x, int y) const
//{
//	return ((int)pos.x == x && (int)pos.y == y);
//}
#include "Defense.h"

Defense::Defense(SDL_Renderer* renderer, SDL_Texture* texture, Vector2D pos, float timerWeapon, uint8_t damages, uint8_t weaponRange, float speedAngular)
	:m_Texture(texture), m_Pos(pos), m_TimerWeapon(Timer(timerWeapon)), m_Damages(damages), 
	m_WeaponRange(weaponRange), m_SpeedAngular(speedAngular),
	m_WeaponAngle(0.0f)
{
	m_WeaponRangeTexture = TextureLoader::loadTexture(renderer, "Range.bmp");
}

void Defense::drawWeaponRange(SDL_Renderer* renderer, int tileSize) const
{

	if (renderer != nullptr && m_WeaponRangeTexture != nullptr)
	{
		// Draw the image at the turret's position and angle
		int w, h;
		SDL_QueryTexture(m_WeaponRangeTexture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)(m_Pos.x * tileSize) - m_WeaponRange * tileSize,
			(int)(m_Pos.y * tileSize) - m_WeaponRange * tileSize,
			m_WeaponRange * 2 * tileSize,
			m_WeaponRange * 2 * tileSize
		};

		SDL_SetTextureAlphaMod(m_WeaponRangeTexture, 30);
		SDL_RenderCopy(renderer, m_WeaponRangeTexture, NULL, &rect);
	}
	
}

bool Defense::isAlive() const
{
	return m_Alive;
}

SDL_Texture* Defense::getTexture() const
{
	return m_Texture;
}

Defense::~Defense()
{
}

bool Defense::checkIfOnTile(int x, int y) const
{
	return ((int)m_Pos.x == x && (int)m_Pos.y == y);
}


std::weak_ptr<Unit> Defense::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits)
{
	//find the closest Enemy for this turret

	std::weak_ptr<Unit> closestUnit;
	float closestDistance = 0.0f;

	//Loop through the entire list of units
	for (auto& selectedUnit : listUnits)
	{
		if (selectedUnit != nullptr)
		{
			//calculate the distance to the enemy unit
			float currentDistance = (m_Pos - selectedUnit->getPos()).magnitude();
			//Check of the unit is within range, and no closest unit has been detected 
			//or the selected unit is closer than the previous closest unit
			if (currentDistance <= m_WeaponRange &&
				(closestUnit.expired() || currentDistance < closestDistance))
			{
				closestUnit = selectedUnit;
				closestDistance = currentDistance;
			}

		}
	}
	return closestUnit;
}

bool Defense::updateAngle(float dT)
{
	if (auto targetUnitSP = targetUnit.lock())
	{
		//determine the direction normal to the target
		Vector2D directionNormalToTarget = (targetUnitSP->getPos() - m_Pos).normalize();

		//Determine the angle to the target
		float angleToTarget = directionNormalToTarget.angleBetween(Vector2D(m_WeaponAngle));

		//update the angle as required

		//determine the angle to move this frame
		float angleMove = -copysign(m_SpeedAngular * dT, angleToTarget);
		if (abs(angleMove) > abs(angleToTarget))
		{
			//It will point directly at its target this frame
			m_WeaponAngle = directionNormalToTarget.angle();
			return true;
		}
		else
		{
			//It won't reach its target this frame
			m_WeaponAngle += angleMove;
		}

	}

	return false;
}
#include "Turret.h"


const float Turret::speedAngular = MathAddon::angleDegToRad(180.0f);
const float Turret::weaponRange = 5.0f;

Turret::Turret(SDL_Renderer* renderer, Vector2D setPos)
	:pos(setPos), angle(0.0f), timerWeapon(1.0f)
{
	textureMain = TextureLoader::loadTexture(renderer, "Turret.bmp");
	textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
}

void Turret::update(SDL_Renderer* renderer, float dT, 
	std::vector<std::shared_ptr<Unit>>& listUnits,
	std::vector<Projectile>& listProjectiles)
{
	/* angle += speedAngular * dT;
	if (angle > MathAddon::angleDegToRad(360.0f))
		angle -= MathAddon::angleDegToRad(360.0f);
	*/

	//Update the timer
	timerWeapon.countDown(dT);

	//check if a target has been found but is no longer alive or is out of weapon range
	if (auto targetUnitSP = targetUnit.lock()) 
	{
		if (!targetUnitSP->isAlive() || (targetUnitSP->getPos() - pos).magnitude() > weaponRange)
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

bool Turret::updateAngle(float dT)
{
	if (auto targetUnitSP = targetUnit.lock())
	{
		//determine the direction normal to the target
		Vector2D directionNormalToTarget = (targetUnitSP->getPos() - pos).normalize();

		//Determine the angle to the target
		float angleToTarget = directionNormalToTarget.angleBetween(Vector2D(angle));

		//update the angle as required

		//determine the angle to move this frame
		float angleMove = -copysign(speedAngular * dT, angleToTarget);
		if (abs(angleMove) > abs(angleToTarget))
		{
			//It will point directly at its target this frame
			angle = directionNormalToTarget.angle();
			return true;
		}
		else
		{
			//It won't reach its target this frame
			angle += angleMove;
		}
		
	}

	return false;
}

void Turret::shootProjectile(SDL_Renderer* renderer, std::vector<Projectile>& listProjectiles)
{
	//Shoot a projectile towards the target unit if the weapon timer is ready
	if (timerWeapon.timeSIsZero())
	{
		listProjectiles.push_back(Projectile(renderer, pos, Vector2D(angle)));

		timerWeapon.resetToMax();
	}
}

void Turret::draw(SDL_Renderer* renderer, int tileSize)
{
	drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
	drawTextureWithOffset(renderer, textureMain, 0, tileSize);
}

bool Turret::checkIfOnTile(int x, int y) const
{
	return ((int)pos.x == x && (int)pos.y == y);
}

void Turret::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected, int offset, int tileSize)
{
	if (renderer != nullptr && textureSelected != nullptr)
	{
		// Draw the image at the turret's position and angle
		int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)(pos.x * tileSize) - w / 2 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w,
			h
		};
		SDL_RenderCopyEx(renderer, textureSelected, NULL, &rect,
			MathAddon::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
	}
}

std::weak_ptr<Unit> Turret::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits)
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
			float currentDistance = (pos - selectedUnit->getPos()).magnitude();
			//Check of the unit is within range, and no closest unit has been detected 
			//or the selected unit is closer than the previous closest unit
			if (currentDistance <= weaponRange &&
				(closestUnit.expired() || currentDistance < closestDistance))
			{
				closestUnit = selectedUnit;
				closestDistance = currentDistance;
			}

		}
	}
	return closestUnit;
}

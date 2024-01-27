#include "Projectile.h"


const float Projectile::speed = 10.0f;
const float Projectile::size = 0.2f;
const float Projectile::distanceTraveledMax = 5.0;

Projectile::Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal)
	:pos(setPos), directionNormal(setDirectionNormal)
{
	texture = TextureLoader::loadTexture(renderer, "Projectile.bmp");
}

void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits)
{
	//move the projectile forward
	float distanceMoved = speed * dT;
	pos += directionNormal * distanceMoved;

	distanceTraveled += distanceMoved;
	if (distanceTraveled >= distanceTraveledMax)
		collisionOccured = true;

	checkCollisions(listUnits);
}

void Projectile::draw(SDL_Renderer* renderer, int tileSize)
{
	if (renderer != nullptr)
	{
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)(pos.x * tileSize) - w / 2,
			(int)(pos.y * tileSize) - h / 2,
			w,
			h
		};
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}

bool Projectile::getCollisionOccured() const
{
	return collisionOccured;
}

void Projectile::checkCollisions(std::vector<std::shared_ptr<Unit>>& listUnits)
{
	//Check for the collision for any of the units
	if (!collisionOccured)
	{
		//Check if this overlap any of the enemy units or not
		for (int count = 0; count < listUnits.size() && !collisionOccured; count++)
		{
			auto& selectedUnit = listUnits[count];
			if (selectedUnit != nullptr && selectedUnit->checkOverlap(pos, size))
			{
				selectedUnit->removeHealth(1);
				collisionOccured = true;
			}
		}
	}
}

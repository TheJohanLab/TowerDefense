#include "Projectile.h"


const float Projectile::speed = 10.0f;
const float Projectile::size = 0.2f;
const float Projectile::distanceTraveledMax = 5.0;

Projectile::Projectile(SDL_Renderer* renderer, Vector2D setPos, Vector2D setDirectionNormal, int offsetX, int offsetY, SDL_Texture* projectileTexture)
	:m_Pos(setPos), m_DirectionNormal(setDirectionNormal), m_Texture(projectileTexture), m_OffsetX(offsetX), m_OffsetY(offsetY)
{
}

void Projectile::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits)
{
	//move the projectile forward
	float distanceMoved = speed * dT;
	m_Pos += m_DirectionNormal * distanceMoved;

	m_DistanceTraveled += distanceMoved;
	if (m_DistanceTraveled >= distanceTraveledMax)
		m_CollisionOccured = true;

	checkCollisions(listUnits);
}


void Projectile::draw(SDL_Renderer* renderer, int tileSize)
{
	if (renderer != nullptr)
	{
		int w, h;
		SDL_QueryTexture(m_Texture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)(m_Pos.x * tileSize) - tileSize/2 - w/2 + m_OffsetX,
			(int)(m_Pos.y * tileSize) -  tileSize/2 - h/2 + m_OffsetY,
			//(int)(pos.x * tileSize) - (tileSize / 2) + ((tileSize-w) / 2),
			//(int)(pos.y * tileSize) - (tileSize / 2) - (h * 0.7),
			w,
			h
		};
		SDL_RenderCopy(renderer, m_Texture, NULL, &rect);
	}
}

bool Projectile::getCollisionOccured() const
{
	return m_CollisionOccured;
}

void Projectile::checkCollisions(std::vector<std::shared_ptr<Unit>>& listUnits)
{
	//Check for the collision for any of the units
	if (!m_CollisionOccured)
	{
		//Check if this overlap any of the enemy units or not
		for (int count = 0; count < listUnits.size() && !m_CollisionOccured; count++)
		{
			auto& selectedUnit = listUnits[count];
			if (selectedUnit != nullptr && selectedUnit->checkOverlap(m_Pos, size))
			{
				selectedUnit->removeHealth(1);
				m_CollisionOccured = true;
			}
		}
	}
}

#include "DefenseExplosion.h"


Explosion::Explosion(SDL_Renderer* renderer, Vector2D pos)
	:Defense(renderer, 
		TextureLoader::loadTexture(renderer, "Bomb.bmp"), 
		TextureLoader::loadTexture(renderer, "BombPreview.bmp"), 
		pos, 2.0f, 2, 5, 0.0f),
	m_ExplosionTimer(0.8f), m_BombTimer(2.0f), m_Exploded(false),
	m_Animation({0,8,100,32,32})
{
	m_ExplosionTexture = TextureLoader::loadTexture(renderer, "Explosion.bmp");
	m_BombTimer.resetToMax();
}

Explosion::~Explosion()
{
}

void Explosion::update(SDL_Renderer* renderer, float dT, std::vector<std::shared_ptr<Unit>>& listUnits, std::vector<Projectile>& listProjectiles)
{
	m_BombTimer.countDown(dT);
	if (m_BombTimer.timeSIsZero() )
	{
		if (!m_Exploded)
		{
			m_Exploded = true;
			m_ExplosionTimer.resetToMax();

			std::weak_ptr<Unit> unitInRange;

			//Loop through the entire list of units
			for (auto& selectedUnit : listUnits)
			{
				if (selectedUnit != nullptr)
				{
					//calculate the distance to the enemy unit
					float currentDistance = (m_Pos - selectedUnit->getPos()).magnitude();
					//Check of the unit is within range, and no closest unit has been detected 
					//or the selected unit is closer than the previous closest unit
					if (currentDistance <= m_WeaponRange)
					{
						unitInRange = selectedUnit;
						if (auto targetUnitSP = unitInRange.lock())
						{
							if (targetUnitSP->isAlive())
								targetUnitSP->removeHealth(m_Damages);
						}
					}

				}
			}
		}
		else if (m_ExplosionTimer.timeSIsZero())
			m_Alive = false;

		m_ExplosionTimer.countDown(dT);
	}
}

void Explosion::draw(SDL_Renderer* renderer, int tileSize)
{
	if (m_Exploded)
		drawExplosion(renderer, tileSize);
	else
	{
		drawBomb(renderer, tileSize);
		//drawWeaponRange(renderer, tileSize);
	}
}

void Explosion::drawBomb(SDL_Renderer* renderer, uint8_t tileSize)
{
	if ((SDL_GetTicks() / 1000) % 2 == 0)
	{
		int colorMod = m_BombRedState ? 255 : 0;
		m_BombRedState = !m_BombRedState;
		SDL_SetTextureColorMod(m_Texture, 255, colorMod, colorMod);
	}


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

void Explosion::drawExplosion(SDL_Renderer* renderer, uint8_t tileSize) const
{
	SDL_Rect src = {
		m_Animation.spriteWidth * static_cast<int>((SDL_GetTicks() / m_Animation.speed) % m_Animation.frames),
		m_Animation.index * m_Animation.spriteHeight,
		m_Animation.spriteWidth,
		m_Animation.spriteHeight
	};
	SDL_Rect dst = {
		(int)((m_Pos.x * tileSize) - (m_Animation.spriteWidth / 2) * m_WeaponRange),
		(int)((m_Pos.y * tileSize) - (m_Animation.spriteHeight / 2 * m_WeaponRange)),
		m_Animation.spriteWidth * m_WeaponRange,
		m_Animation.spriteHeight * m_WeaponRange };
	SDL_RenderCopy(renderer, m_ExplosionTexture, &src, &dst);
}
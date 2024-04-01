#include "Unit.h"
#include "../Controller/Game.h"
#include "../Controller/Level.h"
#include <cmath>




Unit::Unit(SDL_Renderer* renderer, Vector2D setPos, std::function<void(uint8_t)> onDestroyedCallback, SDL_Texture* texture, 
	uint8_t damages, float speed, float size, uint8_t maxHealth, SpriteAnimation animation) :
	m_Pos(setPos), m_HitTimer(0.25f), m_HealthBar(maxHealth), m_MaxHealth(maxHealth), m_CurrDirection({0.0, 0.0}), m_onUnitDestroyedCallback(onDestroyedCallback),
	m_Texture(texture), m_Damages(damages), m_Speed(speed), m_Size(size), m_Animation(animation)
{

}

void Unit::update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits) {
	m_HitTimer.countDown(dT);

	//Determine the distance to the target from the unit's current position.
	float distanceToTarget = (level.getTargetPos() - m_Pos).magnitude();


	//Determine the distance to move this frame.
	float distanceMove = m_Speed * dT;
	if (distanceMove > distanceToTarget)
		distanceMove = distanceToTarget;

	//Find the normal from the flow field.
	Vector2D directionNormal(level.getFlowNormal((int)m_Pos.x, (int)m_Pos.y));

	if (directionNormal.x == 0 && directionNormal.y == 0)
		std::cout;
	else if ( directionNormal.x != m_CurrDirection.x || directionNormal.y != m_CurrDirection.y)
	{
		float ent;
		if ((abs(modf(m_Pos.x, &ent) - 0.5f) < 0.05f) && (abs(modf(m_Pos.y, &ent) - 0.5f) < 0.05f) || m_CurrDirection.x == 0.0 && m_CurrDirection.y == 0.0f)
			m_CurrDirection = directionNormal;
	}
	




	// If this reached the target tile, then modify directionNormal to point to the target Tile.
	if ((int)m_Pos.x == (int)level.getTargetPos().x && (int)m_Pos.y == (int)level.getTargetPos().y)
	{
		level.onTargetReached(1);
		currentHealth = 0;
		//m_CurrDirection = (level.getTargetPos() - pos).normalize();
	}

	Vector2D posAdd = m_CurrDirection * distanceMove;

	//Check if the new position would overlap any other units or not.
	/*bool moveOk = true;
	for (int count = 0; count < listUnits.size() && moveOk; count++) 
	{
		auto& unitSelected = listUnits[count];
		if (unitSelected != nullptr && unitSelected.get() != this &&
			unitSelected->checkOverlap(m_Pos, m_Size))
		{
				//They overlap so check and see if this unit is moving towards or away 
				//from the unit it overlaps.
				Vector2D directionToOther = (unitSelected->m_Pos - m_Pos);
				//Ensure that they're not directly on top of each other.
				if (directionToOther.magnitude() > 0.01f) {
					//Check the angle between the units positions and the direction that this unit 
					//is traveling.  Ensure that this unit isn't moving directly towards the other 
					//unit (by checking the angle between).
					Vector2D normalToOther(directionToOther.normalize());
					float angleBtw = abs(normalToOther.angleBetween(directionNormal));
					if (angleBtw < 3.14159265359f / 4.0f)
						//Don't allow the move.
						moveOk = false;
				}
		}
	}*/

	//if (moveOk) {
		//Check if it needs to move in the x direction.  If so then check if the new x position, plus an amount of spacing 
		//(to keep from moving too close to the wall) is within a wall or not and update the position as required.
		//const float spacing = 0.70f;
		//const float spacing = 0.1f;
		//int x = (int)(pos.x + posAdd.x + copysign(spacing, posAdd.x));
		//int y = (int)(pos.y);
		//if (posAdd.x != 0.0f && level.isTileWall(x, y) == false)
		m_Pos.x += posAdd.x;


		//Do the same for the y direction.
		//x = (int)(pos.x);
		//y = (int)(pos.y + posAdd.y + copysign(spacing, posAdd.y));
		//if (posAdd.y != 0.0f && level.isTileWall(x, y) == false)
		m_Pos.y += posAdd.y;
	//}
}



void Unit::draw(SDL_Renderer* renderer, int tileSize) {
	if (renderer != nullptr) {

		//Set the texture's draw color to red if this unit was hit recently
		if (!m_HitTimer.timeSIsZero())
			SDL_SetTextureColorMod(m_Texture, 255, 0, 0);
		else
			SDL_SetTextureColorMod(m_Texture, 255, 255, 255);


		//Draw a rectangle at the unit's position.
		//int w, h;
		//SDL_QueryTexture(m_Texture, NULL, NULL, &w, &h);
		SDL_Rect src = {
			m_Animation.spriteWidth* static_cast<int>((SDL_GetTicks() / m_Animation.speed) % m_Animation.frames),
			m_Animation.index * m_Animation.spriteHeight,
			m_Animation.spriteWidth,
			m_Animation.spriteHeight
		};
		SDL_Rect dst = {
			(int)((m_Pos.x * tileSize) - m_Animation.spriteWidth / 2),
			(int)((m_Pos.y * tileSize) - m_Animation.spriteHeight / 2),
			m_Animation.spriteWidth,
			m_Animation.spriteHeight };
		SDL_RenderCopy(renderer, m_Texture, &src, &dst);

		
 		//Draw the healthbar above every unit's position
		m_HealthBar.draw(renderer, ((m_Pos.x * tileSize) - m_Animation.spriteWidth / 2), ((m_Pos.y * tileSize) - m_Animation.spriteHeight / 2));

	}
}


bool Unit::checkOverlap(Vector2D posOther, float sizeOther) {
	return (posOther - m_Pos).magnitude() <= (sizeOther + m_Size) / 2.0f;
}

bool Unit::isAlive() const
{
	return currentHealth > 0;
}

Vector2D Unit::getPos() const
{
	return m_Pos;
}



void Unit::removeHealth(int damage)
{
	if (m_HitTimer.timeSIsZero() && damage > 0)
	{
		currentHealth -= damage;
		if (currentHealth <= 0)
		{
			//onDestroyUnitObserver.observe(1);
			m_onUnitDestroyedCallback(m_Damages);
			currentHealth = 0;
		}

		m_HealthBar.setHealth(currentHealth);
		m_HitTimer.resetToMax();
	}

}

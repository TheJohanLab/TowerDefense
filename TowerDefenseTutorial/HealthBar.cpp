#include "HealthBar.h"

HealthBar::HealthBar()
{
	std::cout << "Healthbar par défaut" << "\n";
}

HealthBar::HealthBar(uint8_t maxHealth)
	: m_MaxHealth(maxHealth), m_CurrentHealth(maxHealth) 
{
	std::cout << "Healthbar value" << "\n";
}

HealthBar::~HealthBar()
{
	std::cout << "Destructeur healthbar" << "\n";
}

void HealthBar::setHealth(uint8_t healthValue)
{
	if (healthValue > m_MaxHealth)
		m_CurrentHealth = m_MaxHealth;
	else if (healthValue < 0)
		m_CurrentHealth = 0;
	else
		m_CurrentHealth = healthValue;
}

void HealthBar::draw(SDL_Renderer* renderer, float posX, float posY) const
{
	if (renderer != nullptr) {
		
		
		//Draw a rectangle at the unit's position.
		int w, h;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		//SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)((posX)) + 12,
			(int)((posY)) - 8,
			25 * m_CurrentHealth/m_MaxHealth,
			6 };
		SDL_RenderFillRect(renderer, &rect);
		//SDL_RenderCopy(renderer, texture, NULL, &rect);
		//SDL_RenderPresent(renderer);
	}
}

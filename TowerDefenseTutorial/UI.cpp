#include "UI.h"
#include "TextureLoader.h"
#include <algorithm>

void UI::drawBackground(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{

		// Draw Gray UI background
		SDL_SetRenderDrawColor(renderer, 184, 184, 184, 255);

		SDL_Rect rect = {
			0,
			(int)m_UIHeight-10,
			(int)m_UIWidth,
			(int)(m_UIHeight / 0.8) * 0.25
		};
		SDL_RenderFillRect(renderer, &rect);

		// Draw Background seperators
		SDL_SetRenderDrawColor(renderer, 84, 84, 84, 150);

		rect = {0,(int)m_UIHeight - 10, (int)m_UIWidth,10};
		SDL_RenderFillRect(renderer, &rect);
		rect = { 0, (int)(m_UIHeight / 0.8 - 10), (int)m_UIWidth, 10 };
		SDL_RenderFillRect(renderer, &rect);
		rect = { 0, (int)m_UIHeight ,10 ,(int)(m_UIHeight / 0.8) };
		SDL_RenderFillRect(renderer, &rect);
		rect = { (int)m_UIWidth - 10, (int)m_UIHeight, 10, (int)(m_UIHeight / 0.8) };
		SDL_RenderFillRect(renderer, &rect);
		rect = { (int)(m_UIWidth / 4), (int)m_UIHeight, 10, (int)(m_UIHeight / 0.8) };
		SDL_RenderFillRect(renderer, &rect);
		rect = { (int)(m_UIWidth / 2), (int)m_UIHeight, 10, (int)(m_UIHeight / 0.8) };
		SDL_RenderFillRect(renderer, &rect);



	}
}

void UI::drawItems(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{
		// Draw the Wall texture
		int w, h;
		SDL_QueryTexture(m_WallTexture, NULL, NULL, &w, &h);

		SDL_Rect rect =
		{
			(int)(m_UIWidth / 8) - w / 2,
			(int)(m_UIHeight + (m_UIHeight / 0.8) * 0.1) - h / 2,
			w,
			h
		};
		SDL_RenderCopy(renderer, m_WallTexture, NULL, &rect);

		// Draw the Turret texture
		SDL_QueryTexture(m_TurretTexture, NULL, NULL, &w, &h);


		rect =
		{
			(int)(m_UIWidth * 3 / 8) - w / 2,
			(int)(m_UIHeight +(m_UIHeight / 0.8) * 0.1) - h / 2,
			w,
			h
		};
		SDL_RenderCopy(renderer, m_TurretTexture, NULL, &rect);
	}
}

void UI::drawHealth(SDL_Renderer* renderer) const
{
	if (renderer != nullptr)
	{
		
	}
}

void UI::drawCoins(SDL_Renderer* renderer) const
{
}

UI::UI(SDL_Renderer* renderer, int UIWidth, int uiHeight)
	:m_UIWidth(UIWidth), m_UIHeight(uiHeight),
	m_Health(15), m_Coins(150), m_SelectedItem(item::Wall)
{
	std::cout << UIWidth << ", " << uiHeight << "\n";
	std::cout << (int)m_UIWidth << ", " << (int)m_UIHeight << "\n";
	std::cout << "UI Constructeur\n";
	m_WallTexture = TextureLoader::loadTexture(renderer, "Tile Wall.bmp");
	m_TurretTexture = TextureLoader::loadTexture(renderer, "Turret.bmp");
}

UI::~UI()
{
	std::cout << "UI Destr\n";
}

void UI::draw(SDL_Renderer* renderer) const
{
	drawBackground(renderer);
	drawCoins(renderer);
	drawHealth(renderer);
	drawItems(renderer);
}

void UI::updateHealth(uint8_t health)
{
	m_Health = health;
}

void UI::updateCoins(uint8_t coins)
{
	m_Coins = coins;
}

void UI::selectItem(item selectedItem)
{
	m_SelectedItem = selectedItem;
}

uint8_t UI::getPlayersHealth() const
{
	return m_Health;
}

void UI::purchaseTurret()
{
	m_Coins -= itemPrice[m_SelectedItem];
	m_Coins = std::max((int)m_Coins, 0);
}

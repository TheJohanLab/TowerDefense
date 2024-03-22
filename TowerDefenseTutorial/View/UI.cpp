#include "UI.h"
#include "../TextureLoader.h"
#include <algorithm>
#include <string>

UI* UI::instance = nullptr;



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
	if (renderer != nullptr && m_uiFont != nullptr)
	{
		SDL_Color color = { 50, 50, 50, 255 }; 
		
		
		SDL_Surface* surface = TTF_RenderText_Solid(m_uiFont, std::to_string(m_Health).c_str(), color);
		if (surface == nullptr) {
			SDL_Log("Erreur lors de la création de la surface de texte : %s", TTF_GetError());
			return;
		}

		// Créer une texture à partir de la surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr) {
			SDL_Log("Erreur lors de la création de la texture : %s", SDL_GetError());
			SDL_FreeSurface(surface);
			return;
		}

		int texW, texH;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

		SDL_Rect dstRect = { 100, 100, texW, texH };
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	}
}

void UI::drawCoins(SDL_Renderer* renderer) const
{

}


UI* UI::getInstance()
{
	if (!instance) {
		instance = new UI();
	}
	return instance;
}

UI::~UI()
{
	std::cout << "UI Destr\n";
}

void UI::initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight)
{
	m_uiFont = TTF_OpenFont("C:/_Projets/Cpp/TowerDefenseTutorial/TowerDefenseTutorial/Dependencies/Fonts/super_ocean/SuperOcean.ttf", 24); 
	if (m_uiFont == nullptr) {
		SDL_Log("Erreur lors du chargement de la police de caractères : %s", TTF_GetError());
		TTF_Quit();
		return;
	}
	
	m_renderer = renderer;
	m_UIWidth = windowsWidth;
	m_UIHeight = windowsHeight * 0.8;

	m_Health = 15;
	m_Coins = 150;
	m_SelectedItem = item::Wall;

	if (renderer != nullptr)
	{
		m_WallTexture = TextureLoader::loadTexture(m_renderer, "Tile Wall.bmp");
		m_TurretTexture = TextureLoader::loadTexture(m_renderer, "Turret.bmp");
	}

}



void UI::draw(SDL_Renderer* renderer) const
{
	drawBackground(renderer);
	drawCoins(renderer);
	drawHealth(renderer);
	drawItems(renderer);
}

void UI::updateHealth(uint8_t damages)
{
	m_Health -= damages;
	m_Health = std::max(0, (int)m_Health);
}

void UI::updateCoins(uint8_t coinsReceived)
{
	m_Coins += coinsReceived;
	std::cout << m_Coins << "\n";
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

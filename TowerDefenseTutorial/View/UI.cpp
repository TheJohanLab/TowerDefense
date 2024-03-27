#include "UI.h"
#include "../Controller/TextureLoader.h"
#include <algorithm>
#include <string>
#include "../Controller/Game.h"

UI* UI::instance = nullptr;

Shop* UI::getShop()
{
	return &m_Shop;
}

void UI::initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight, Game* game)
{
	m_Game = game;
	m_Health = m_Game->getpHealth();

	m_uiFont = TTF_OpenFont("C:/_Projets/Cpp/TowerDefenseTutorial/TowerDefenseTutorial/Dependencies/Fonts/sunny_spells_basic/Sunny Spells Basic.ttf", 24);
	if (m_uiFont == nullptr) {
		SDL_Log("Erreur lors du chargement de la police de caractères : %s", TTF_GetError());
		TTF_Quit();
		return;
	}


	m_renderer = renderer;
	m_UIWidth = windowsWidth;
	m_UIHeight = windowsHeight * 0.8;

	m_SelectedItem = itemEnum::WallItem;

	if (renderer != nullptr)
	{
		m_WallTexture = TextureLoader::loadTexture(m_renderer, "TileWall2.bmp");
		m_WallSelectedTexture = TextureLoader::loadTexture(m_renderer, "TileWallSelected.bmp");
		m_TurretTexture = TextureLoader::loadTexture(m_renderer, "Turret2.bmp");
		m_TurretSelectedTexture = TextureLoader::loadTexture(m_renderer, "TurretSelected.bmp");
	}

}

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
		SDL_Texture* selectedWallTexture;
		SDL_Texture* selectedTurretTexture;
		if (m_SelectedItem == itemEnum::WallItem)
		{
			selectedWallTexture = m_WallSelectedTexture;
			selectedTurretTexture = m_TurretTexture;
			SDL_SetTextureAlphaMod(selectedWallTexture, 128);
			SDL_SetTextureAlphaMod(selectedTurretTexture, 255);
			//SDL_SetTextureColorMod(m_WallTexture, 255, 0, 0);
			//SDL_SetTextureColorMod(m_TurretTexture, 255, 255, 255);
		}
		else
		{
			selectedWallTexture = m_WallTexture;
			selectedTurretTexture = m_TurretSelectedTexture;
			SDL_SetTextureAlphaMod(selectedWallTexture, 255);
			SDL_SetTextureAlphaMod(selectedTurretTexture, 128);
		}

		SDL_QueryTexture(selectedWallTexture, NULL, NULL, &w, &h);
		SDL_Rect rect =
		{
			(int)(m_UIWidth / 8) - w ,
			(int)(m_UIHeight + (m_UIHeight / 0.8) * 0.1) - h ,
			w*2,
			h*2
		};		

		SDL_RenderCopy(renderer, selectedWallTexture, NULL, &rect);

		// Draw the Turret texture
		SDL_QueryTexture(selectedTurretTexture, NULL, NULL, &w, &h);


		rect =
		{
			(int)(m_UIWidth * 3 / 8) - w ,
			(int)(m_UIHeight +(m_UIHeight / 0.8) * 0.1) - h ,
			w*2,
			h*2
		};
		SDL_RenderCopy(renderer, selectedTurretTexture, NULL, &rect);
	}
}

void UI::drawHealth(SDL_Renderer* renderer) const
{
	if (renderer != nullptr && m_uiFont != nullptr)
	{
		SDL_Color color = { 50, 50, 50, 255 }; 

		SDL_Surface* surface = TTF_RenderText_Solid(m_uiFont, std::to_string(*m_Health).c_str(), color);
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

		SDL_Rect dstRect = { m_UIWidth / 2 + 50, m_UIHeight + 50, texW, texH };
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	}
}

void UI::drawCoins(SDL_Renderer* renderer) const
{
	SDL_Color color = { 50, 50, 50, 255 };

	SDL_Surface* surface = TTF_RenderText_Solid(m_uiFont, std::to_string(m_Shop.getMoneyAmount()).c_str(), color);
	if (surface == nullptr) {
		SDL_Log("Erreur lors de la création de la surface de texte : %s", TTF_GetError());
		return;
	}

	// Créer une texture à partir de la surface
	SDL_Texture * m_CoinTexture = SDL_CreateTextureFromSurface(renderer, surface);
	if (m_CoinTexture == nullptr) {
		SDL_Log("Erreur lors de la création de la texture : %s", SDL_GetError());
		SDL_FreeSurface(surface);
		return;
	}
	SDL_FreeSurface(surface);

	int texW, texH;
	SDL_QueryTexture(m_CoinTexture, NULL, NULL, &texW, &texH);

	SDL_Rect dstRect = { m_UIWidth * 2/3 + 50, m_UIHeight + 50, texW, texH };
	SDL_RenderCopy(renderer, m_CoinTexture, NULL, &dstRect);
	
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
}





void UI::draw(SDL_Renderer* renderer) const
{
	drawBackground(renderer);
	drawCoins(renderer);
	drawHealth(renderer);
	drawItems(renderer);
}


void UI::selectItem(itemEnum selectedItem, int x, int y)
{
	m_SelectedItem = selectedItem;
	purchaseTurret();
}


void UI::purchaseTurret()
{
	
	//m_Coins -= itemPrice[m_SelectedItem];
	//m_Coins = std::max((int)m_Coins, 0);
}

void UI::setItemSelectionZone(const itemEnum& itemEnum, const ItemSelectionZone& zone)
{
	m_mapItemSelectionZone[itemEnum] = zone;
}

const ItemSelectionZone& UI::getItemSelectionZone(itemEnum itemKey)
{
	if (m_mapItemSelectionZone.count(itemKey))
		return m_mapItemSelectionZone.at(itemKey);
}

itemEnum* UI::getSelectedItem()
{
	return &m_SelectedItem;
}

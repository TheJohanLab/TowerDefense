#pragma once

#include <iostream>
#include <map>
#include "SDL2/SDL.h"
#include "SDL_ttf.h"
#include "../Utils/Vector2D.h"
#include "../Utils/Utils.h"
#include "../Model/Shop.h"
#include "../Controller/ItemSelectionZone.h"

class Game;
class UI
{

private:
	static UI* instance;
	UI() {};

	Game* m_Game = nullptr;
	Shop m_Shop;
	std::map<itemEnum, ItemSelectionZone> m_mapItemSelectionZone;
	SDL_Renderer* m_renderer = nullptr;
	TTF_Font* m_uiFont = nullptr;

	size_t m_UIWidth;
	size_t m_UIHeight;
	uint8_t* m_Health = nullptr;

	itemEnum m_SelectedItem;

	SDL_Texture* m_WallTexture = nullptr;
	SDL_Texture* m_WallSelectedTexture = nullptr;
	SDL_Texture* m_TurretTexture = nullptr;
	SDL_Texture* m_TurretSelectedTexture = nullptr;
	SDL_Texture* m_CoinTexture = nullptr;



public:
	
	static UI* getInstance(); 
	~UI();

	Shop* getShop();
	void initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight, Game* game);

	void draw(SDL_Renderer* renderer) const;

	void selectItem(itemEnum selectedItem, int x, int y);

	void updateHealth(uint8_t health);
	uint8_t getPlayersHealth() const;
	void purchaseTurret();

	void setItemSelectionZone(const itemEnum& itemEnum, const ItemSelectionZone& zone);
	const ItemSelectionZone& getItemSelectionZone(itemEnum itemSelectionZone);
	itemEnum* getSelectedItem();
	

	std::map<itemEnum, uint8_t> itemPrice = {
	{itemEnum::WallItem, 10},
	{itemEnum::TurretItem, 15}
	};

private:

	void drawBackground(SDL_Renderer* renderer) const;
	void drawItems(SDL_Renderer* renderer) const;
	void drawHealth(SDL_Renderer* renderer) const;
	void drawCoins(SDL_Renderer* renderer) const;
	
};


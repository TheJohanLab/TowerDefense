#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL_ttf.h"
#include "../Utils/Vector2D.h"
#include "../Utils/Utils.h"
#include "../Model/Shop.h"
#include "../Controller/ItemSelectionZone.h"

class UI
{

private:

	struct UIItem
	{
		SDL_Texture* texture;
		uint8_t price;
	};

	static UI* instance;
	UI() {};

	
	std::map<itemEnum, ItemSelectionZone> m_mapItemSelectionZone;
	SDL_Renderer* m_renderer = nullptr;
	TTF_Font* m_uiFont = nullptr;

	size_t m_UIWidth;
	size_t m_UIHeight;
	size_t m_WindowWidh;
	size_t m_WindowHeight;

	const uint8_t *m_Health;
	const Shop *m_Shop;

	itemEnum m_SelectedItem;

	SDL_Texture* m_BackgroundTexture = nullptr;
	SDL_Texture* m_WallTexture = nullptr;
	SDL_Texture* m_TurretTexture = nullptr;
	SDL_Texture* m_EmptyHeartTexture = nullptr;
	SDL_Texture* m_HalfHeartTexture = nullptr;
	SDL_Texture* m_FullHeartTexture = nullptr;
	SDL_Texture* m_GemTexture = nullptr;
	SDL_Texture* m_EmptyGemTexture = nullptr;

	std::vector<UIItem> m_UIItems;



public:
	
	static UI* getInstance(); 
	~UI();

	void initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight, int UIWidth, int UIHeight, 
		const uint8_t* playerLifePoints, const Shop* shop);

	void draw(SDL_Renderer* renderer) const;

	void selectItem(itemEnum selectedItem, int x, int y);
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
	void drawItemPrice(SDL_Renderer* renderer, UIItem Item, SDL_Rect itemPos) const;
	void drawHearts(SDL_Renderer* renderer) const;
	void drawGems(SDL_Renderer* renderer) const;
	
};


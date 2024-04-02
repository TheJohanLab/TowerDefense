#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL_ttf.h"
#include "../Utils/Vector2D.h"
#include "../Model/Shop.h"
#include "../Controller/ItemSelectionZone.h"
#include "../Model/Defense.h"
#include "../Controller/PlayerManager.h"

class UI
{

private:

	struct UIItem
	{
		itemEnum itemEnum;
		Defense* objectItem;
	};

	static UI* instance;
	UI() {};

	
	std::map<itemEnum, ItemSelectionZone> m_mapItemSelectionZone;
	SDL_Renderer* m_renderer = nullptr;

	size_t m_UIWidth;
	size_t m_UIHeight;
	size_t m_WindowWidh;
	size_t m_WindowHeight;

	const Player* m_Player;
	const Shop *m_Shop;

	itemEnum m_SelectedItem;

	SDL_Texture* m_BackgroundTexture = nullptr;
	SDL_Texture* m_WallTexture = nullptr;
	SDL_Texture* m_TurretTexture = nullptr;
	SDL_Texture* m_BombTexture = nullptr;
	SDL_Texture* m_EmptyHeartTexture = nullptr;
	SDL_Texture* m_HalfHeartTexture = nullptr;
	SDL_Texture* m_FullHeartTexture = nullptr;
	SDL_Texture* m_GemTexture = nullptr;
	SDL_Texture* m_EmptyGemTexture = nullptr;
	SDL_Texture* m_ItemSelectorTexture = nullptr;

	std::vector<UIItem> m_UIItems;


public:
	
	static UI* getInstance(); 
	~UI();

	void initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight, int UIWidth, int UIHeight, 
		const Player* player, const Shop* shop);

	void update(float dT);
	void draw(SDL_Renderer* renderer) const;

	void selectItem(itemEnum selectedItem, int x, int y);
	void setItemSelectionZone(const itemEnum& itemEnum, const ItemSelectionZone& zone);
	const ItemSelectionZone& getItemSelectionZone(itemEnum itemSelectionZone);
	itemEnum* getSelectedItem();
	
private:

	void drawBackground(SDL_Renderer* renderer) const;
	void drawItems(SDL_Renderer* renderer) const;
	void drawItemPrice(SDL_Renderer* renderer, const Defense& item, SDL_Rect itemPos) const;
	void drawHearts(SDL_Renderer* renderer) const;
	void drawGems(SDL_Renderer* renderer) const;
	void drawItemSelector(SDL_Renderer* renderer) const;
	void drawCooldown(SDL_Renderer* renderer, UIItem item) const;
	
};


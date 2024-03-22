#pragma once

#include <iostream>
#include "SDL2/SDL.h"
#include <map>
#include "../Vector2D.h"
#include "SDL_ttf.h"

class UI
{
	enum item
	{
		Wall,
		Turret
	};

private:
	static UI* instance;
	UI() {};

	SDL_Renderer* m_renderer = nullptr;
	TTF_Font* m_uiFont = nullptr;

	size_t m_UIWidth;
	size_t m_UIHeight;
	uint8_t m_Health;
	size_t m_Coins;

	item m_SelectedItem;

	SDL_Texture* m_WallTexture = nullptr;
	SDL_Texture* m_TurretTexture = nullptr;

	void drawBackground(SDL_Renderer* renderer) const;
	void drawItems(SDL_Renderer* renderer) const;
	void drawHealth(SDL_Renderer* renderer) const;
	void drawCoins(SDL_Renderer* renderer) const;

public:
	
	static UI* getInstance(); 
	~UI();

	void initUI(SDL_Renderer* renderer, int windowsWidth, int windowsHeight);

	void draw(SDL_Renderer* renderer) const;
	void updateHealth(uint8_t health);
	void updateCoins(uint8_t coins);

	void selectItem(item selectedItem);

	uint8_t getPlayersHealth() const;
	void purchaseTurret();

	std::map<item, uint8_t> itemPrice = {
	{item::Wall, 10},
	{item::Turret, 15}
	};

	
};


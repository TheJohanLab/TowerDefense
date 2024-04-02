#pragma once

#include "SDL2/SDL.h"
#include "../Controller/Level.h"
#include "../Listeners/OnMouseMotionListener.h"
#include "../Utils/Utils.h"
#include "../Utils/Vector2D.h"
#include "../Model/Shop.h"
#include <vector>

class ItemPlacementPreview : public OnMouseMotionListener
{
private:
	itemEnum* m_ItemSelected = nullptr;

	SDL_Texture* m_TowerPreviewTexture = nullptr;
	SDL_Texture* m_TurretPreviewTexture = nullptr;
	SDL_Texture* m_BombPreviewTexture = nullptr;
	SDL_Texture* m_RangePreviewTexture = nullptr;

	SDL_Rect m_PlayingZone;

	Vector2D m_LastPreviewPos;
	Vector2D* m_PreviewPos = new Vector2D(0.0f,0.0f);
	Level& m_Level;
	Shop& m_Shop;

	bool m_Buildable = false;
	bool m_PreviewEnabled = false;

	int m_TurretPreviewOffsetX = 8;
	int m_TurretPreviewOffsetY = -10;

public:
	ItemPlacementPreview(SDL_Renderer* renderer, Level& level, Shop& shop, int x, int y, int w, int h);
	~ItemPlacementPreview();

	void draw(SDL_Renderer* renderer, int TileSize) const;
	void onMove(int x, int y) override;

	void disablePreview();

	bool isItemPlacementEnabled() const;

private:
	void drawItemPreview(int x, int y);
	void drawItemInfos(int x, int y);
	bool isOnPlayingZone(int mouseX, int mouseYl) const;

};


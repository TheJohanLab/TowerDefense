#pragma once

#include <vector>
#include "SDL2/SDL.h"
#include "../Listeners/OnMouseMotionListener.h"
#include "../Utils/Utils.h"
#include "../Controller/ItemSelectionZone.h"
#include "../Controller/Level.h"

struct itemInfo
{
	SDL_Texture* texture;
	const ItemSelectionZone* zone;
};

struct itemRange
{
	itemEnum itemEnum;
	Vector2D pos;
};

class ItemsInfoPreview : public OnMouseMotionListener
{
private:
	const Level* m_Level;
	std::vector<itemInfo> m_itemInfos;
	const ItemSelectionZone* m_PlayingZone = nullptr;
	itemEnum* m_SelectedItem = nullptr;
	itemInfo* m_ItemHover = nullptr;
	itemRange m_ItemRange;

public:

	ItemsInfoPreview(SDL_Renderer* renderer, const Level* level);
	~ItemsInfoPreview();

	void draw(SDL_Renderer* renderer, int tileSize) const;
	void onMove(int x, int y) override;

	void setPlayingZone(const ItemSelectionZone* itemZone);
	void addItemInfoZone(const itemEnum& item, const ItemSelectionZone* itemZone);

private:

	void drawItemStats(SDL_Renderer* renderer, int tileSize) const;
	void drawItemRange(SDL_Renderer* renderer, int tileSize) const;
};


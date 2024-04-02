#include "ItemsInfoPreview.h"
#include "../Controller/TextureLoader.h"
#include "UI.h"
#include "../Controller/ItemSelectionZone.h"

ItemsInfoPreview::ItemsInfoPreview(SDL_Renderer* renderer, const Level* level)
	:m_Level(level), m_ItemRange({ itemEnum::None, Vector2D(0.0f, 0.0f) })
{
	m_itemInfos.push_back({ TextureLoader::loadTexture(renderer, "StartLevel.bmp"), nullptr});
	m_itemInfos.push_back({ TextureLoader::loadTexture(renderer, "StartLevel.bmp"), nullptr});
	m_itemInfos.push_back({ TextureLoader::loadTexture(renderer, "StartLevel.bmp"), nullptr});


	UI* ui = UI::getInstance();
	m_SelectedItem = ui->getSelectedItem();
}

ItemsInfoPreview::~ItemsInfoPreview()
{
}

void ItemsInfoPreview::draw(SDL_Renderer* renderer, int tileSize) const
{
	if (renderer != nullptr )
	{
		if (m_ItemHover != nullptr)
			drawItemStats(renderer, tileSize);
		else if (m_ItemRange.itemEnum != itemEnum::None)
			drawItemRange(renderer, tileSize);
	}
	

}

void ItemsInfoPreview::onMove(int x, int y)
{
	if (*m_SelectedItem == itemEnum::None)
	{
		int tileX = x / TILE_SIZE;
		int tileY = y / TILE_SIZE;
		if (m_PlayingZone != nullptr && m_PlayingZone->isOnZone(x, y))
		{
			if (m_Level->isTileWall(tileX, tileY))
			{
				m_ItemRange.itemEnum = itemEnum::TowerItem;
				m_ItemRange.pos.x = tileX;
				m_ItemRange.pos.y = tileY;
				return;
			}
			if (m_Level->isTurret(tileX, tileY))
			{
				m_ItemRange.itemEnum = itemEnum::TurretItem;
				m_ItemRange.pos.x = tileX;
				m_ItemRange.pos.y = tileY;
				return;
			}
		}
		else
		{
			for (auto& itemInfo : m_itemInfos)
			{
				if (itemInfo.zone->isOnZone(x, y))
				{
					m_ItemHover = &itemInfo;
					return;
				}
			}
		}


	}
	
	m_ItemRange.itemEnum = itemEnum::None;
	m_ItemHover = nullptr;
}

void ItemsInfoPreview::addItemInfoZone(const itemEnum& item, const ItemSelectionZone* itemZone)
{
	m_itemInfos[(int)item].zone = itemZone;
}

void ItemsInfoPreview::setPlayingZone(const ItemSelectionZone* playingZone)
{
	m_PlayingZone = playingZone;
}

void ItemsInfoPreview::drawItemStats(SDL_Renderer* renderer, int tileSize) const
{
		int w, h;
		SDL_Texture* currentTexture = m_ItemHover->texture;
		SDL_QueryTexture(currentTexture, NULL, NULL, &w, &h);
		SDL_Rect itemHoverZone = m_ItemHover->zone->getZone();


		SDL_Rect rect =
		{
			itemHoverZone.x,
			itemHoverZone.y - h,
			w,
			h
		};


		SDL_RenderCopy(renderer, currentTexture, NULL, &rect);

	
}

void ItemsInfoPreview::drawItemRange(SDL_Renderer* renderer, int tileSize) const
{
	if (m_ItemRange.itemEnum == itemEnum::TurretItem)
		Items::getItemData(itemEnum::TurretItem)->drawWeaponRange(renderer, tileSize, m_ItemRange.pos.x, m_ItemRange.pos.y);
	Items::getItemData(itemEnum::TowerItem)->drawWeaponRange(renderer, tileSize, m_ItemRange.pos.x, m_ItemRange.pos.y);
}
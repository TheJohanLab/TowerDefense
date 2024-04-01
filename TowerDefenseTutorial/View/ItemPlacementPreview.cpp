#include "ItemPlacementPreview.h"

#include "UI.h"
#include "../Controller/TextureLoader.h"
#include "../Controller/Pathfinding.h"



ItemPlacementPreview::ItemPlacementPreview(SDL_Renderer* renderer, Level& level, Shop& shop, int x, int y, int w, int h)
	:m_PlayingZone({x, y, w, h}), m_Level(level), m_Shop(shop)
{
	UI* ui = UI::getInstance();
	m_ItemSelected = ui->getSelectedItem();

	m_TowerPreviewTexture = TextureLoader::loadTexture(renderer, "WallPreview.bmp");
	m_TurretPreviewTexture = TextureLoader::loadTexture(renderer, "TurretPreview.bmp");
	m_BombPreviewTexture = TextureLoader::loadTexture(renderer, "BombPreview.bmp");
	m_RangePreviewTexture = TextureLoader::loadTexture(renderer, "TurretRange.bmp");
	SDL_SetTextureAlphaMod(m_TowerPreviewTexture, 128);
	SDL_SetTextureAlphaMod(m_TurretPreviewTexture, 128);
}



ItemPlacementPreview::~ItemPlacementPreview()
{
}

void ItemPlacementPreview::draw(SDL_Renderer* renderer, int tileSize) const
{
	if (renderer != nullptr && m_PreviewPos != nullptr)
	{
		Vector2D posMouse((float)m_PreviewPos->x / tileSize, (float)m_PreviewPos->y / tileSize);
		int w, h;
		SDL_Texture* currentTexture;
		int offsetX = 0;
		int offsetY = 0;
		switch (*m_ItemSelected)
		{
		case itemEnum::TurretItem:
			currentTexture = m_TurretPreviewTexture;
			offsetX = m_TurretPreviewOffsetX;
			offsetY = m_TurretPreviewOffsetY;
			break;
		case itemEnum::TowerItem:
			currentTexture = m_TowerPreviewTexture;
			break;
		case itemEnum::ExplosionItem:
			currentTexture = m_BombPreviewTexture;
			break;
		default:
			currentTexture = m_TowerPreviewTexture;
			break;

		}


		SDL_QueryTexture(currentTexture, NULL, NULL, &w, &h);
		m_Buildable ? SDL_SetTextureColorMod(currentTexture, 255, 255, 255) : SDL_SetTextureColorMod(currentTexture, 255, 0, 0);

		SDL_Rect rect =
		{
			(int)posMouse.x * tileSize + offsetX,
			(int)posMouse.y * tileSize + offsetY,
			w,
			h
		};
		SDL_RenderCopy(renderer, currentTexture, NULL, &rect);
	}

}

void ItemPlacementPreview::onMove(int x, int y)
{
	//std::cout << "x : " << x << ", y : " << y << "\n";

	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;

	if (m_Shop.isBuyable(*m_ItemSelected) &&
		*m_ItemSelected != itemEnum::None && isOnPlayingZone(x, y) &&
		(*m_ItemSelected == itemEnum::TowerItem && !m_Level.isTileWall(tileX, tileY) && !m_Level.isTurret(tileX, tileY)) ||
		(*m_ItemSelected == itemEnum::ExplosionItem && !m_Level.isTileWall(tileX, tileY) && !m_Level.isTurret(tileX, tileY)) ||
		(*m_ItemSelected == itemEnum::TurretItem && m_Level.isTileWall(tileX, tileY) && !m_Level.isTurret(tileX, tileY)) &&
		!m_Level.isEnemyOnTile(tileX, tileY) &&
		!m_Level.isTileTarget(tileX, tileY) &&
		!m_Level.isTileSpawner(tileX, tileY) &&
		!m_Level.isTileObstruct(tileX, tileY) &&
		!m_Level.isPathObstructed(tileX, tileY)
		)
	{
		m_LastPreviewPos.x = x;
		m_LastPreviewPos.y = y;
		m_PreviewPos = &m_LastPreviewPos;
		m_Buildable = true;
	}
	else if (*m_ItemSelected != itemEnum::None && isOnPlayingZone(x, y)) 
	{
		m_LastPreviewPos.x = x;
		m_LastPreviewPos.y = y;
		m_PreviewPos = &m_LastPreviewPos;
		m_Buildable = false;
	}
	else
		m_PreviewPos = nullptr;

	
}

bool ItemPlacementPreview::isItemPlacementEnabled() const
{
	return m_Buildable;
}


bool ItemPlacementPreview::isOnPlayingZone(int mouseX, int mouseY) const
{
	if (mouseX > m_PlayingZone.x && mouseX < (m_PlayingZone.x + m_PlayingZone.w) &&
		mouseY > m_PlayingZone.y && mouseY < (m_PlayingZone.y + m_PlayingZone.h))
		return true;

	return false;
}
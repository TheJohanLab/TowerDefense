#include "LevelView.h"
#include "../Controller/TextureLoader.h"

LevelView::LevelView(SDL_Renderer* renderer, uint8_t tileCountX, uint8_t tileCountY)
    :m_TilesCountX(tileCountX), m_TilesCountY(tileCountY)
{
	m_BackGroundTileSetTextures = TextureLoader::loadTexture(renderer, "BackGroundTileSet.bmp");
	m_ObstructTileSetTextures = TextureLoader::loadTexture(renderer, "ObstructObjectsTileSet.bmp");
	//m_SpawnerTileSetTextures = TextureLoader::loadTexture(renderer, "spawner.bmp");
	m_TargetTileSetTextures = TextureLoader::loadTexture(renderer, "TargetTileSet.bmp");
}

LevelView::~LevelView()
{
}

void LevelView::loadStaticTiles(const std::vector<std::vector<Tile>>& levelLayersTiles)
{
    m_LevelLayersTiles = levelLayersTiles;
}



void LevelView::draw(SDL_Renderer* renderer) const
{
    drawTiles(renderer, m_BackGroundTileSetTextures, (int)TileType::BUILDABLE, TILE_SIZE, TILE_SIZE);
    //drawTiles(renderer, m_SpawnerTileSetTextures, (int)TileType::ENEMYSPAWNER, TILE_SIZE, TILE_SIZE);
    drawTiles(renderer, m_TargetTileSetTextures, (int)TileType::TARGET, TILE_SIZE, TILE_SIZE);
}

void LevelView::drawTiles(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t layer, uint8_t tileSizeX, uint8_t tileSizeY, int offsetX, int offsetY) const
{
    int nbOfTiles = m_TilesCountX * m_TilesCountY;
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);

    for (int i = 0; i < nbOfTiles; i++)
    {
        Tile currTile = m_LevelLayersTiles[layer][i];
        if (currTile.tileIndex != 0xFF)
        {
            uint8_t srcX = currTile.tileIndex % 10;
            uint8_t srcY = currTile.tileIndex / 10;
            SDL_Rect srcRect = {srcX * tileSizeX, srcY * tileSizeY, tileSizeX, tileSizeY};
            SDL_Rect dstRect = { 
                (i % m_TilesCountX) * TILE_SIZE + offsetX,
                (i / m_TilesCountX)* TILE_SIZE + offsetY,
                tileSizeX,
                tileSizeY
            };
            SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
        }
    }

}



void LevelView::drawObstructTiles(SDL_Renderer* renderer) const
{
    uint8_t tileSizeX = 45;
    uint8_t tileSizeY = 55;
    int offsetX = -(tileSizeX - TILE_SIZE) / 2;
    int offsetY = -(tileSizeY - TILE_SIZE);

    drawTiles(renderer, m_ObstructTileSetTextures, (int)TileType::OBSTRUCT, tileSizeX, tileSizeY, offsetX, offsetY);
}

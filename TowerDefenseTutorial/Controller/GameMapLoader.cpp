#include "GameMapLoader.h"
#include <fstream>

GameMapLoader::GameMapLoader(int tileCountX, int tileCountY)
	:m_TileCountX(tileCountX), m_TileCountY(tileCountY)
{
}

GameMapLoader::~GameMapLoader()
{
}

void GameMapLoader::reloadLevelMap(std::vector<Tile>& levelTiles)
{
	levelTiles = m_levelMapCopy;
}

const std::vector<std::vector<Tile>>& GameMapLoader::loadMap(std::vector<Tile>& listTiles, const char* path)
{
	
	m_LevelLayersTiles.reserve(NUMBER_OF_LAYER * m_TileCountX * m_TileCountY);

	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	int tens, units;
	
	for (int layerNb = 0; layerNb < NUMBER_OF_LAYER; layerNb++)
	{
		m_LevelLayersTiles.emplace_back(std::vector<Tile>());

		for (int y = 0; y < m_TileCountY; y++)
		{
			for (int x = 0; x < m_TileCountX; x++)
			{
				mapFile.get(tile);
				if (tile == '-')
				{
					mapFile.ignore();
					addTiles(listTiles, 0xFF, layerNb, x, y);
				}
				else
				{
					tens = atoi(&tile) * 10;
					mapFile.get(tile);
					units = atoi(&tile);
					addTiles(listTiles, tens + units, layerNb, x, y);
				}

				mapFile.ignore();
			}
			mapFile.ignore();
		}

		mapFile.ignore();

	}
	
	mapFile.close();

	m_levelMapCopy = listTiles;
	return m_LevelLayersTiles;
}


void GameMapLoader::addTiles(std::vector<Tile>& listTiles, uint8_t tileValue, uint8_t layer, uint8_t x, uint8_t y)
{
	if (layer == (int)TileType::BUILDABLE)
		listTiles.push_back(Tile{ m_MapLayerType.at(layer) });
	else if (tileValue != 0xFF)
		updateTilesType(listTiles, tileValue, layer, x, y);
		
	m_LevelLayersTiles[layer].emplace_back(Tile{ m_MapLayerType.at(layer), tileValue });
}

void GameMapLoader::updateTilesType(std::vector<Tile>& levelTiles, uint8_t tileValue, uint8_t layer, uint8_t x, uint8_t y)
{
	levelTiles[x + (y * m_TileCountX)] = m_MapLayerType.at(layer);
}


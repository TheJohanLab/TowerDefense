#include "GameMap.h"
#include <fstream>

GameMap::GameMap(int tileCountX, int tileCountY)
	:m_TileCountX(tileCountX), m_TileCountY(tileCountY)
{
}

GameMap::~GameMap()
{
}

void GameMap::loadMap(std::vector<Tile>& listTiles, const char* path)
{
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	//int srcX, srcY;

	for (int y = 0; y < m_TileCountY; y++)
	{
		for (int x = 0; x < m_TileCountX; x++)
		{
			mapFile.get(tile);
			//srcY = atoi(&tile) * TILE_SIZE;
			//mapFile.get(tile);
			//srcX = atoi(&tile) * TILE_SIZE;
			setTile(listTiles, tile);
			mapFile.ignore();
		}
		mapFile.ignore();
	}

	/*
	mapFile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(tile);

			if (tile == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}
	*/
	mapFile.close();
}


void GameMap::setTile(std::vector<Tile>& listTiles, const char tile)
{
	listTiles.push_back(Tile{ m_MapTileType.at(tile)});
}
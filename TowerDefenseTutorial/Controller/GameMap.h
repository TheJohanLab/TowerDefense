#pragma once

#include <vector>
#include <map>
#include "../Utils.h"

class GameMap
{

private:

	std::map<char, TileType> m_MapTileType = {
		{'0', TileType::EMPTY },
		{'1', TileType::WALL },
		{'2', TileType::TURRET },
		{'3', TileType::ENEMYSPAWNER },
		{'4', TileType::TARGET },
		{'5', TileType::FOREST },
	};

	int m_TileCountX, m_TileCountY;

public:
	GameMap(int tileCountX, int tileCountY);
	~GameMap();

	void loadMap(std::vector<Tile>& listTiles, const char* path);

private:
	void setTile(std::vector<Tile>& listTiles, const char tile);
};


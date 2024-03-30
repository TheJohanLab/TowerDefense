#pragma once

#include <vector>
#include <map>
#include "../Utils/Utils.h"

class GameMapLoader
{

private:

	std::map<uint8_t, TileType> m_MapLayerType = {
		{0, TileType::BUILDABLE },
		{1, TileType::OBSTRUCT },
		{2, TileType::ENEMYSPAWNER },
		{3, TileType::TARGET },
		{4, TileType::WALL },
		{5, TileType::TURRET }
	};

	int m_TileCountX, m_TileCountY;
	std::vector<std::vector<Tile>> m_LevelLayersTiles;
	std::vector<Tile> m_levelMapCopy;

public:
	GameMapLoader(int tileCountX, int tileCountY);
	~GameMapLoader();

	void reloadLevelMap(std::vector<Tile>& levelTiles);
	const std::vector<std::vector<Tile>>& loadMap(std::vector<Tile>& levelTiles, const char* path);

private:
	void addTiles(std::vector<Tile>& levelTiles, uint8_t tileValue, uint8_t layer, uint8_t x, uint8_t y);
	void updateTilesType(std::vector<Tile>& levelTiles, uint8_t tileValue, uint8_t layer, uint8_t x, uint8_t y);
};


#pragma once
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include "SDL2/SDL.h"

#include "TextureLoader.h"
#include "Pathfinding.h"
#include "GameMapLoader.h"

#include "../Model/Unit.h"
#include "../Utils/Utils.h"
#include "../Utils/Vector2D.h"
/*
struct Tile {
	TileType type = TileType::EMPTY;
	int flowDirectionX = 0;
	int flowDirectionY = 0;
	unsigned char flowDistance = FLOW_DISTANCE_MAX;
};
*/


class Level
{
private:
	GameMapLoader* m_GameMapLoader;
	Pathfinding* m_PathFinding;

	std::vector<Tile> m_ListTiles;
	std::vector<std::shared_ptr<Unit>>* m_ListUnits;
	const int tileCountX, tileCountY;

	int m_TargetPosX = 0, m_TargetPosY = 0;

	SDL_Texture* textureTileWall = nullptr;



public:	

	Level(SDL_Renderer* renderer, int tileCountX, int tileCountY);
	~Level();

	void pausePathfinding();
	void startPathfinding();

	std::function<void(uint8_t)> onTargetReached;
	std::function<void(uint8_t)> onUnitDestroyed;

	void setOnTargetReachedCallback(std::function<void(uint8_t)> callback);
	void setOnUnitDestroyedCallback(std::function<void(uint8_t)> callback);

	void resetMapTiles();
	const std::vector<std::vector<Tile>>& loadLevelMap(const char* path);

	void setListUnits(std::vector<std::shared_ptr<Unit>>* listUnits);
	void drawWalls(SDL_Renderer* renderer, int tileSize);

	Vector2D getRandomEnemySpawnerLocation() const;

	bool isTileObstruct(int x, int y) const;
	bool isTileTarget(int x, int y) const;
	bool isTileSpawner(int x, int y) const;

	bool isTileWall(int x, int y) const;
	void setTileWall(int x, int y);
	void removeWall(int x, int y);

	bool isTurret(int x, int y) const;
	void setTurret(int x, int y);
	void removeTurret(int x, int y);

	Vector2D getTargetPos() const;
	Vector2D getFlowNormal(int x, int y);

	bool isPathObstructed(int x, int y) const;
	bool isEnemyOnTile(int x, int y);

private:

	TileType getTileType(int x, int y) const;
	void setTileType(int x, int y, TileType tileType);

	void assignTargetPos(std::vector <Tile> targetLayerTiles);
	void initializeEnemySpawners();


		
};
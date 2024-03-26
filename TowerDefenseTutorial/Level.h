#pragma once
#include <queue>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"
#include "Utils.h"
#include "Controller/Pathfinding.h"
#include "Controller/GameMap.h"
#include "Unit.h"
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


public:	

	Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY);
	~Level();

	void setListUnits(std::vector<std::shared_ptr<Unit>>* listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	void drawTrees(SDL_Renderer* renderer, int tileSize);

	Vector2D getRandomEnemySpawnerLocation() const;

	bool isTileForest(int x, int y) const;
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
	void drawTile(SDL_Renderer* renderer, int x, int y, int tileSize);
	//void calculateFlowField();
	//void calculateDistances();
	//void calculateFlowDirections();
	TileType getTileType(int x, int y) const;
	void setTileType(int x, int y, TileType tileType);

	void assignTargetPos();
	void initializeEnemySpawners();

	GameMap* m_GameMap;
	Pathfinding* m_PathFinding;

	std::vector<Tile> listTiles;
	std::vector<std::shared_ptr<Unit>>* m_ListUnits;
	const int tileCountX, tileCountY;

	int targetX = 0, targetY = 0;

	SDL_Texture 
		*textureTileWall = nullptr,
		*textureTileTurretBase = nullptr,
		*textureTileTarget = nullptr,
		*textureTileEmpty = nullptr,
		*textureTileTree = nullptr,
		*textureTileEnemySpawner = nullptr,
		*textureTileArrowUp = nullptr,
		*textureTileArrowUpRight = nullptr,
		*textureTileArrowRight = nullptr,
		*textureTileArrowDownRight = nullptr,
		*textureTileArrowDown = nullptr,
		*textureTileArrowDownLeft = nullptr,
		*textureTileArrowLeft = nullptr,
		*textureTileArrowUpLeft = nullptr;
		
};
#pragma once
#include <queue>
#include <vector>
#include <string>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"

class Turret;

class Level
{
private:
	
	enum class TileType: char 
	{	
		EMPTY,
		WALL,
		TURRET,
		ENEMYSPAWNER,
		TARGET
	};
	
	static const unsigned char flowDistanceMax = 255;

	struct Tile {
		TileType type = TileType::EMPTY;
		int flowDirectionX = 0;
		int flowDirectionY = 0;
		unsigned char flowDistance = flowDistanceMax;
	};


public:
	Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY, Vector2D target);
	void draw(SDL_Renderer* renderer, int tileSize);

	Vector2D getRandomEnemySpawnerLocation() const;

	bool isTileTarget(int x, int y) const;

	bool isTileWall(int x, int y) const;
	void setTileWall(int x, int y);
	void removeWall(int x, int y);

	bool isTurret(const std::vector<Turret>& listTurrets, int x, int y) const;
	void setTurret(int x, int y);
	void removeTurret(int x, int y);

	Vector2D getTargetPos() const;
	Vector2D getFlowNormal(int x, int y);


private:
	void drawTile(SDL_Renderer* renderer, int x, int y, int tileSize);
	void calculateFlowField();
	void calculateDistances();
	void calculateFlowDirections();
	TileType getTileType(int x, int y) const;
	void setTileType(int x, int y, TileType tileType);

	void initializeEnemySpawners();


	std::vector<Tile> listTiles;
	const int tileCountX, tileCountY;

	const int targetX = 0, targetY = 0;

	SDL_Texture* textureTileWall = nullptr,
		*textureTileTarget = nullptr,
		*textureTileEmpty = nullptr,
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
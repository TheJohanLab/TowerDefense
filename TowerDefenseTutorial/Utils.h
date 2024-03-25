#pragma once

#define INITIAL_MONEY_AMOUNT 200
#define TILE_SIZE 32
#define FLOW_DISTANCE_MAX 255

enum itemEnum
{
	WallItem,
	TurretItem,
	None
};



enum class TileType : char
{
	EMPTY,
	WALL,
	TURRET,
	ENEMYSPAWNER,
	TARGET
};

struct Tile {
	TileType type = TileType::EMPTY;
	int flowDirectionX = 0;
	int flowDirectionY = 0;
	unsigned char flowDistance = FLOW_DISTANCE_MAX;
};
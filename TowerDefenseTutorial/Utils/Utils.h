#pragma once

#define MAX_LIFE_POINTS 10
#define INITIAL_MONEY_AMOUNT 200
#define TILE_SIZE 32
#define FLOW_DISTANCE_MAX 255
#define SPAWN_TIMER_MS 200 

#define NUMBER_OF_LAYER 4

enum itemEnum
{
	WallItem,
	TurretItem,
	ExplosionItem,
	None
};



enum class TileType : char
{
	BUILDABLE,		//0
	OBSTRUCT,		//1
	ENEMYSPAWNER,	//2
	TARGET,			//3
	WALL,			//4
	TURRET,			//5
	NONE			//6
};


struct Tile {
	Tile()
		:type(TileType::BUILDABLE) {}
	Tile(TileType tileType)
		:type(tileType){}
	Tile(TileType tileType, uint8_t index)
		:type(tileType), tileIndex(index) {}

	TileType type;
	uint8_t tileIndex = 0;
	int flowDirectionX = 0;
	int flowDirectionY = 0;
	unsigned char flowDistance = FLOW_DISTANCE_MAX;
};

enum class GameState : char
{
	INIT,
	PAUSED,
	RUNNING,
	STOPPED,
	WAITING,
	VICTORY,
	GAMEOVER
};
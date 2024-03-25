#pragma once

#include <vector>
#include "../Vector2D.h"
#include "../Utils.h"

class Pathfinding
{
private:
	int m_TileCountX, m_TileCountY;
	Vector2D m_TargetPos;
	static const unsigned char m_FlowDistanceMax = 255;
	unsigned char m_FlowDistance = m_FlowDistanceMax;

	void calculateDistances(std::vector<Tile>& listTiles);
	void calculateFlowDirections(std::vector<Tile>& listTiles);
	
public:
	Pathfinding(int tileCountX, int tileCountY, Vector2D targetPos);
	~Pathfinding();

	void calculateFlowField(std::vector<Tile>& listTiles);

	bool isPathObstructed(std::vector<Tile> listTiles, int x, int y);
};


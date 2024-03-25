#include "Pathfinding.h"
#include <queue>
#include "../Level.h"

Pathfinding::Pathfinding(int tileCountX, int tileCountY, Vector2D targetPos)
    :m_TileCountX(tileCountX), m_TileCountY(tileCountY), m_TargetPos(targetPos)
{
}

Pathfinding::~Pathfinding()
{

}

void Pathfinding::calculateFlowField(std::vector<Tile>& listTiles)
{
    int indexTarget = m_TargetPos.x + m_TargetPos.y * m_TileCountX;
    if (indexTarget > -1 && indexTarget < listTiles.size() &&
        m_TargetPos.x > -1 && m_TargetPos.x < m_TileCountX &&
        m_TargetPos.y > -1 && m_TargetPos.y < m_TileCountY) {

        //Reset the tile flow data.
        for (auto& tileSelected : listTiles) {
            tileSelected.flowDirectionX = 0;
            tileSelected.flowDirectionY = 0;
            tileSelected.flowDistance = FLOW_DISTANCE_MAX;
        }

        //Calculate the flow field.
        calculateDistances(listTiles);
        calculateFlowDirections(listTiles);
    }
}

bool Pathfinding::isPathObstructed(std::vector<Tile> listTilesCpy, int x, int y)
{
    for (auto& tileSelected : listTilesCpy) {
        tileSelected.flowDistance = FLOW_DISTANCE_MAX;
    }

    listTilesCpy[x + y * m_TileCountX].type = TileType::WALL;
    
    calculateFlowField(listTilesCpy);

    for (Tile tile : listTilesCpy)
    {
        if (tile.flowDistance == FLOW_DISTANCE_MAX && tile.type == TileType::EMPTY)
            return true;
    }
    return false;
}

void Pathfinding::calculateDistances(std::vector<Tile>& listTiles)
{
    int indexTarget = (int)m_TargetPos.x + (int)m_TargetPos.y * m_TileCountX;

    //Create a queue that will contain the indices to be checked.
    std::queue<int> listIndicesToCheck;
    //Set the target tile's flow value to 0 and add it to the queue.
    listTiles[indexTarget].flowDistance = 0;
    listIndicesToCheck.push(indexTarget);

    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

    //Loop through the queue and assign distance to each tile.
    while (listIndicesToCheck.empty() == false) {
        int indexCurrent = listIndicesToCheck.front();
        listIndicesToCheck.pop();

        //Check each of the neighbors;
        for (int count = 0; count < 4; count++) {
            int neighborX = listNeighbors[count][0] + indexCurrent % m_TileCountX;
            int neighborY = listNeighbors[count][1] + indexCurrent / m_TileCountX;
            int indexNeighbor = neighborX + neighborY * m_TileCountX;

            //Ensure that the neighbor exists and isn't a wall.
            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                neighborX > -1 && neighborX < m_TileCountX &&
                neighborY > -1 && neighborY < m_TileCountY &&
                listTiles[indexNeighbor].type != TileType::WALL &&
                listTiles[indexNeighbor].type != TileType::TURRET) {

                //Check if the tile has been assigned a distance yet or not.
                if (listTiles[indexNeighbor].flowDistance == FLOW_DISTANCE_MAX) {
                    //If not the set it's distance and add it to the queue.
                    listTiles[indexNeighbor].flowDistance = listTiles[indexCurrent].flowDistance + 1;
                    listIndicesToCheck.push(indexNeighbor);
                }
            }
        }
    }
}


void Pathfinding::calculateFlowDirections(std::vector<Tile>& listTiles)
{
    const int listNeighbors[][2] = { { -1, 0}, { 1, 0 }, { 0, -1 }, { 0, 1 } };

    for (int indexCurrent = 0; indexCurrent < listTiles.size(); indexCurrent++) {
        //Ensure that the tile has been assigned a distance value.
        if (listTiles[indexCurrent].flowDistance != FLOW_DISTANCE_MAX) {
            //Set the best distance to the current tile's distance.
            unsigned char flowFieldBest = listTiles[indexCurrent].flowDistance;

            //Check each of the neighbors;
            for (int count = 0; count < 4; count++) {
                int offsetX = listNeighbors[count][0];
                int offsetY = listNeighbors[count][1];

                int neighborX = offsetX + indexCurrent % m_TileCountX;
                int neighborY = offsetY + indexCurrent / m_TileCountX;
                int indexNeighbor = neighborX + neighborY * m_TileCountX;

                //Ensure that the neighbor exists.
                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < m_TileCountX &&
                    neighborY > -1 && neighborY < m_TileCountY) {
                    //If the current neighbor's distance is lower than the best then use it.
                    if (listTiles[indexNeighbor].flowDistance < flowFieldBest) {
                        flowFieldBest = listTiles[indexNeighbor].flowDistance;
                        listTiles[indexCurrent].flowDirectionX = offsetX;
                        listTiles[indexCurrent].flowDirectionY = offsetY;
                    }
                }
            }
        }
    }
}

#include "Level.h"
#include "../Model/Unit.h"
#include "../Model/Wall.h"

Level::Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY)
    : tileCountX(setTileCountX), tileCountY(setTileCountY),
    m_GameMapLoader(new GameMapLoader(setTileCountX, setTileCountY)),
    m_PathFinding(new Pathfinding(setTileCountX, setTileCountY))
{
    textureTileWall = TextureLoader::loadTexture(renderer, "Wall.bmp");

    size_t listTilesSize = (size_t)tileCountX * tileCountY;
    m_ListTiles.reserve(listTilesSize);

}


Level::~Level()
{
    delete m_PathFinding;
    delete m_GameMapLoader;
}

void Level::setOnTargetReachedCallback(std::function<void(int)> callback)
{
    onTargetReached = callback;
}


const std::vector<std::vector<Tile>>& Level::loadLevelMap(const char* path)
{
    const std::vector<std::vector<Tile>>& levelLayers = m_GameMapLoader->loadMap(m_ListTiles, path);
    assignTargetPos(levelLayers[(int)TileType::TARGET]);
    m_PathFinding->setTargetPos(Vector2D{ (float)m_TargetPosX, (float)m_TargetPosY });
    m_PathFinding->startPathfinding(m_ListTiles);
    return levelLayers;
}



void Level::setListUnits(std::vector<std::shared_ptr<Unit>>* listUnits)
{
    m_ListUnits = listUnits;
}

void Level::drawWalls(SDL_Renderer* renderer, int tileSize) {

    
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTileWall(x, y) || isTurret(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileWall, NULL, NULL, &w, &h);
                SDL_Rect rect = 
                {
                    x * tileSize + tileSize / 2 - w / 2,
                    y * tileSize + tileSize / 2 - h / 2,
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect);
            }
        }
    }

 
}

Vector2D Level::getRandomEnemySpawnerLocation() const
{
    // Create a list of all tiles that are enemy spawners
    std::vector<int> listSpawnerIndices;

    for (int count = 0; count < m_ListTiles.size(); count++)
    {
        auto& tileSelected = m_ListTiles[count];
        if (tileSelected.type == TileType::ENEMYSPAWNER)
            listSpawnerIndices.push_back(count);
    }

    //If one or more spawners are found, pick one at random and output its center position
    if (!listSpawnerIndices.empty())
    {
        int index = listSpawnerIndices[rand() % listSpawnerIndices.size()];
        return Vector2D((float)(index % tileCountX) + 0.5f, (float)(index / tileCountX) + 0.5f);
    }

    return Vector2D(0.5f, 0.5f);
    
}

bool Level::isTileObstruct(int x, int y) const
{
    return (getTileType(x, y) == TileType::OBSTRUCT);
}

bool Level::isTileTarget(int x, int y) const
{
    return (getTileType(x, y) == TileType::TARGET);
}

bool Level::isTileSpawner(int x, int y) const
{
    return (getTileType(x, y) == TileType::ENEMYSPAWNER);
}


bool Level::isTileWall(int x, int y) const
{
    return (getTileType(x, y) == TileType::WALL);
}


void Level::setTileWall(int x, int y) 
{
    TileType tileType = getTileType(x, y);
    if (tileType != TileType::ENEMYSPAWNER && tileType != TileType::TARGET)
        setTileType(x, y, TileType::WALL);

    

}

void Level::removeWall(int x, int y)
{
    if (getTileType(x, y) == TileType::WALL)
        setTileType(x, y,TileType::BUILDABLE);
}



bool Level::isTurret( int x, int y) const
{
    return getTileType(x, y) == TileType::TURRET;
}

void Level::setTurret(int x, int y)
{
    if (getTileType(x, y) == TileType::WALL)
        setTileType(x, y, TileType::TURRET);
}

void Level::removeTurret(int x, int y)
{
    if (getTileType(x, y) == TileType::TURRET)
        setTileType(x, y, TileType::WALL);
}


TileType Level::getTileType(int x, int y) const
{
    int index = x + y * tileCountX;
    if (index > -1 && index < m_ListTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return m_ListTiles[index].type;

    return TileType::BUILDABLE;
}

void Level::setTileType(int x, int y, TileType tileType)
{
    int index = x + y * tileCountX;
    if (index > -1 && index < m_ListTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
    {
        m_ListTiles[index].type = tileType;
        m_PathFinding->calculateFlowField(m_ListTiles);
    }
    
}

void Level::assignTargetPos(std::vector <Tile> targetLayerTiles)
{
    for (int i = 0; i < targetLayerTiles.size() - 1; i++)
    {
        if (targetLayerTiles[i].tileIndex == 4)
        {
            m_TargetPosX = i % tileCountX;
            m_TargetPosY = i / tileCountX;
            return;
        }
    }

    std::cerr << "No Target tile was found\n";
}

void Level::initializeEnemySpawners()
{
    //Add an enemy spawner at each corner
    int xMax = tileCountX - 1;
    int yMax = tileCountY - 1;

    setTileType(0, 0, TileType::ENEMYSPAWNER);
    setTileType(xMax, 0, TileType::ENEMYSPAWNER);
    setTileType(0, yMax, TileType::ENEMYSPAWNER);
    setTileType(xMax, yMax, TileType::ENEMYSPAWNER);
}


Vector2D Level::getTargetPos() const{
    return Vector2D((float)m_TargetPosX + 0.5, (float)m_TargetPosY + 0.5);
}


Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < m_ListTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float)m_ListTiles[index].flowDirectionX, (float)m_ListTiles[index].flowDirectionY).normalize();

    return Vector2D();
}

bool Level::isPathObstructed(int x, int y) const
{
   return m_PathFinding->isPathObstructed(m_ListTiles, x, y);
}

bool Level::isEnemyOnTile(int x, int y)
{
    for (const std::shared_ptr<Unit> unit : *m_ListUnits)
    {
        if (unit && (int)unit->getPos().x == x && (int)unit->getPos().y == y)
            return true;
    }

    return false;
}

#include "Level.h"
#include "Unit.h"

Level::Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY)
    : tileCountX(setTileCountX), tileCountY(setTileCountY),
    m_GameMap(new GameMap(setTileCountX, setTileCountY))
{
    textureTileWall = TextureLoader::loadTexture(renderer, "Wall.bmp");
    textureTileTurretBase = TextureLoader::loadTexture(renderer, "WallTurretBase.bmp");
    textureTileTree = TextureLoader::loadTexture(renderer, "Tree.bmp");
    textureTileTarget = TextureLoader::loadTexture(renderer, "Tile Target.bmp");
    textureTileEnemySpawner = TextureLoader::loadTexture(renderer, "Tile Enemy Spawner.bmp");

    textureTileEmpty = TextureLoader::loadTexture(renderer, "Path.bmp");
    textureTileArrowUp = TextureLoader::loadTexture(renderer, "Tile Arrow Up.bmp");
    textureTileArrowUpRight = TextureLoader::loadTexture(renderer, "Tile Arrow Up Right.bmp");
    textureTileArrowRight = TextureLoader::loadTexture(renderer, "Tile Arrow Right.bmp");
    textureTileArrowDownRight = TextureLoader::loadTexture(renderer, "Tile Arrow Down Right.bmp");
    textureTileArrowDown = TextureLoader::loadTexture(renderer, "Tile Arrow Down.bmp");
    textureTileArrowDownLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Down Left.bmp");
    textureTileArrowLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Left.bmp");
    textureTileArrowUpLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Up Left.bmp");

    size_t listTilesSize = (size_t)tileCountX * tileCountY;
    listTiles.reserve(listTilesSize);
    //listTiles.assign(listTilesSize, Tile{});
    //setTileType(targetX, targetY, TileType::TARGET);

    //initializeEnemySpawners();

    m_GameMap->loadMap(listTiles, "Data/Levels/Level1.map");
    assignTargetPos();
    m_PathFinding = new Pathfinding(setTileCountX, setTileCountY, Vector2D{(float)targetX, (float)targetY});

    //calculateFlowField();
    //pathFinding->calculateFlowField(listTiles);
}


Level::~Level()
{
    delete m_PathFinding;
    delete m_GameMap;
}



void Level::setListUnits(std::vector<std::shared_ptr<Unit>>* listUnits)
{
    m_ListUnits = listUnits;
}

void Level::draw(SDL_Renderer* renderer, int tileSize) {

    /*
    //Draw the tile's background color.
    for (int y = 0; y < tileCountY; y++)
    {
        for (int x = 0; x < tileCountX; x++)
        {
            if ((x + y) % 2 == 0)
                SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
            else
                SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);

            SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    */
    
   //Uncomment to draw the flow field.
   for (int count = 0; count < listTiles.size(); count++)
      drawTile(renderer, (count % tileCountX), (count / tileCountX), tileSize);
   

    //Draw the spawner tiles.
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (getTileType(x, y) == TileType::ENEMYSPAWNER) {
                SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
                SDL_RenderCopy(renderer, textureTileEnemySpawner, NULL, &rect);
            }
        }
    }

    //Draw the target tile.
    if (textureTileTarget != nullptr) {
        SDL_Rect rect = { targetX * tileSize, targetY * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureTileTarget, NULL, &rect);
    }
    
    //Draw the wall tiles.
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

void Level::drawTrees(SDL_Renderer* renderer, int tileSize)
{
    //draw forest tiles
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTileForest(x, y)) {
                int w, h;
                SDL_QueryTexture(textureTileTree, NULL, NULL, &w, &h);
                SDL_Rect rect =
                {
                    x * tileSize - ((w - tileSize) / 2),
                    y * tileSize - (h - tileSize),
                    w,
                    h
                };
                SDL_RenderCopy(renderer, textureTileTree, NULL, &rect);
            }
        }
    }
}

Vector2D Level::getRandomEnemySpawnerLocation() const
{
    // Create a list of all tiles that are enemy spawners
    std::vector<int> listSpawnerIndices;

    for (int count = 0; count < listTiles.size(); count++) 
    {
        auto& tileSelected = listTiles[count];
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

bool Level::isTileForest(int x, int y) const
{
    return (getTileType(x, y) == TileType::FOREST);
}

bool Level::isTileTarget(int x, int y) const
{
    return (x == targetX && y == targetY);
}

bool Level::isTileSpawner(int x, int y) const
{
    return (getTileType(x, y) == TileType::ENEMYSPAWNER);
}



void Level::drawTile(SDL_Renderer* renderer, int x, int y, int tileSize) {
    //Set the default texture image to be empty.
    SDL_Texture* textureSelected = textureTileEmpty;
    /*
    //Ensure that the input tile exists.
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        Tile& tileSelected = listTiles[index];
        
        //Select the correct tile texture based on the flow direction.
        if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUp;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownRight;
        else if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDown;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpLeft;

    }
    */
    //Draw the tile.
    if (textureSelected != nullptr) {
        SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
    }
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
        setTileType(x, y,TileType::EMPTY);
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
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return listTiles[index].type;

    return TileType::EMPTY;
}

void Level::setTileType(int x, int y, TileType tileType)
{
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
    {
        listTiles[index].type = tileType;
        //calculateFlowField();
        m_PathFinding->calculateFlowField(listTiles);
    }
    
}

void Level::assignTargetPos()
{
    for (int i = 0; i < listTiles.size() - 1; i++)
    {
        if (listTiles[i].type == TileType::TARGET )
        {
            targetX = i % tileCountX;
            targetY = i / tileCountX;
            return;
        }
    }

    std::cout << "No Target tile was found\n";
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
    return Vector2D((float)targetX + 0.5, (float)targetY + 0.5);
}

/*
void Level::calculateFlowField() {
    //Ensure the target is in bounds.
    int indexTarget = targetX + targetY * tileCountX;
    if (indexTarget > -1 && indexTarget < listTiles.size() &&
        targetX > -1 && targetX < tileCountX &&
        targetY > -1 && targetY < tileCountY) {

        //Reset the tile flow data.
        for (auto& tileSelected : listTiles) {
            tileSelected.flowDirectionX = 0;
            tileSelected.flowDirectionY = 0;
            tileSelected.flowDistance = FLOW_DISTANCE_MAX;
        }

        //Calculate the flow field.
        calculateDistances();
        calculateFlowDirections();
    }
}


void Level::calculateDistances() {
    int indexTarget = targetX + targetY * tileCountX;

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
            int neighborX = listNeighbors[count][0] + indexCurrent % tileCountX;
            int neighborY = listNeighbors[count][1] + indexCurrent / tileCountX;
            int indexNeighbor = neighborX + neighborY * tileCountX;

            //Ensure that the neighbor exists and isn't a wall.
            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() && 
                neighborX > -1 && neighborX < tileCountX &&
                neighborY > -1 && neighborY < tileCountY &&
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


void Level::calculateFlowDirections() {
    //The offset of the neighboring tiles to be checked.
    /*const int listNeighbors[][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1} };

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

                int neighborX = offsetX + indexCurrent % tileCountX;
                int neighborY = offsetY + indexCurrent / tileCountX;
                int indexNeighbor = neighborX + neighborY * tileCountX;

                //Ensure that the neighbor exists.
                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < tileCountX &&
                    neighborY > -1 && neighborY < tileCountY) {
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
*/


Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float)listTiles[index].flowDirectionX, (float)listTiles[index].flowDirectionY).normalize();

    return Vector2D();
}

bool Level::isPathObstructed(int x, int y) const
{
   return m_PathFinding->isPathObstructed(listTiles, x, y);
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

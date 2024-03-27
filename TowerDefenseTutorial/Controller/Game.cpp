#include "Game.h"
#include "../View/UI.h"
#include "UnitFactory.h"
#include <cstdlib>


Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    m_Level(renderer, windowWidth / TILE_SIZE, (windowHeight * 0.8) / TILE_SIZE),
    m_InputManager(new InputManager()),
    m_GameStatus(new GameStatus()),
    m_GameLoop(new GameLoop(*this, *m_GameStatus)),
    m_LevelManager(new LevelLoaderManager("Data/Levels/Levels.xml")),
    m_PlayerManager(new PlayerManager(MAX_LIFE_POINTS))
{

    if (window != nullptr && renderer != nullptr) {
        
        std::srand(std::time(nullptr));

        m_Level.setListUnits(&m_ListUnits);
        m_Level.setOnTargetReachedCallback([this](int damages) { m_PlayerManager->takeDamages(damages); });

        m_UI = UI::getInstance();
        m_UI->initUI(renderer, windowWidth, windowHeight, m_PlayerManager->getpPlayerLifePoints());
        m_Shop = m_UI->getShop();

        ItemSelectionZone wallZone( 0, windowHeight*0.8, windowWidth / 4, windowHeight * 0.2, itemEnum::WallItem);
        ItemSelectionZone TurretZone( windowWidth/4, windowHeight*0.8, windowWidth / 4, windowHeight * 0.2, itemEnum::TurretItem);
        ItemSelectionZone PlayingZone( 0, 0, windowWidth, windowHeight * 0.8);
        wallZone.setOnClickListener([this](itemEnum item, int mouseButtonStatus, int x, int y) { if (mouseButtonStatus == 1) m_UI->selectItem(item, x, y) ; });
        TurretZone.setOnClickListener([this](itemEnum item, int mouseButtonStatus, int x, int y) { if (mouseButtonStatus == 1) m_UI->selectItem(item,  x, y); });
        PlayingZone.setOnClickListener([this, renderer](itemEnum item, int mouseButtonStatus, int x, int y) { processEvents(renderer, mouseButtonStatus, x, y); });

        m_InputManager->addItemSelectionZone(&wallZone);
        m_InputManager->addItemSelectionZone(&TurretZone);
        m_InputManager->addItemSelectionZone(&PlayingZone);

        m_SelectedItem = m_UI->getSelectedItem();

        m_ItemPlacementPreview = new ItemPlacementPreview(renderer, m_Level, *m_Shop, 0, 0, windowWidth, tileSize * 14);
        m_InputManager->setMouseMovementCallback([this](int x, int y) { m_ItemPlacementPreview->onMove(x, y); });

        m_PlayerManager->setOnPlayersDeathCallback([this, renderer]() { gameOver(renderer); });

        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");

        loadNextLevel();
        startLevel();

        m_GameLoop->start(renderer);

    }
}


Game::~Game() {

    TextureLoader::deallocateTextures();
    delete m_InputManager;
    delete m_ItemPlacementPreview;
    delete m_GameLoop;
    delete m_LevelManager;
    delete m_PlayerManager;
}

void Game::loadNextLevel()
{
    m_LevelData = m_LevelManager->loadNextLevel();


    m_WaveTimer.setTimeMax(m_LevelData->timerBeforeNextWave[0]);
    m_TotalWaves = m_LevelData->totalWavesNb;
    m_WaveIndex = 0;
    
}

void Game::clearLevel()
{
    m_ListUnits.clear();
    m_WaveIndex = 0;
}

void Game::startLevel()
{
    m_WaveTimer.resetToZero();
    m_WaveIndex = -1;
    m_GameStatus->setGameState(GameState::RUNNING);
}

void Game::handleEvents(SDL_Renderer* renderer, GameState& gameState)
{
    m_InputManager->handleEvents(renderer, gameState);
}



void Game::processEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY) {
    //bool mouseDownThisFrame = false;
    
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    if (m_Shop->isBuyable(*m_SelectedItem) && mouseButtonStatus > 0) {
        switch (mouseButtonStatus) {
        case SDL_BUTTON_LEFT:
            
            if (m_ItemPlacementPreview->isItemPlacementEnabled())
            {
                switch (*m_SelectedItem) {
                case itemEnum::WallItem:
                    //Add wall at the mouse position.
                    if (!m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
                    {
                        m_Level.setTileWall((int)posMouse.x, (int)posMouse.y);
                        m_Shop->purchaseItem(*m_SelectedItem);
                    }
                    break;
                case itemEnum::TurretItem:
                    //Add the selected unit at the mouse position.
                    //if (mouseDownThisFrame)
                    if (m_Level.isTileWall((int)posMouse.x, (int)posMouse.y) && !m_Level.isTurret((int)posMouse.x, (int)posMouse.y))
                    {
                        addTurret(renderer, posMouse);
                        m_Shop->purchaseItem(*m_SelectedItem);
                    }
                    break;
                }
            
                
            }
            break;
        case SDL_BUTTON_RIGHT:
            
            //Remove turret is on tile
            if (removeTurretsAtMousePosition(posMouse))
            {
                m_Shop->sellItem(itemEnum::TurretItem);
                break;
            }

            if (m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
            {
                m_Level.removeWall((int)posMouse.x, (int)posMouse.y);
                m_Shop->sellItem(itemEnum::WallItem);
            }

            break;
        }
    }
}


void Game::update(SDL_Renderer* renderer, float dT) {
    
    handleWaves(renderer, dT);
    handleSpawnUnits(renderer, dT);

    updateUnits(dT);

    for (auto& turretSelected : m_ListTurrets)
        turretSelected.update(renderer, dT, m_ListUnits, m_ListProjectiles);

    updateProjectiles(dT);
    
}


void Game::updateUnits(float dT) {
    //Loop through the list of units and update all of them.
    auto it = m_ListUnits.begin();
    while (it != m_ListUnits.end()) {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(dT, m_Level, m_ListUnits);

            //Check if the unit is still alive.  If not then erase it and don't increment the iterator.
            if ((*it)->isAlive() == false) {
                it = m_ListUnits.erase(it);
                increment = false;
            }
        }

        if (increment)
            it++;
    }
}

void Game::updateProjectiles(float dT)
{
    auto projIt = m_ListProjectiles.begin();
    while (projIt != m_ListProjectiles.end())
    {
        projIt->update(dT, m_ListUnits);

        //Check if the projectile has collided or not
        if (projIt->getCollisionOccured())
            projIt = m_ListProjectiles.erase(projIt);
        else
            projIt++;
    }
        
}

void Game::updateWaveTimer(SDL_Renderer* renderer, float dT)
{

    m_WaveTimer.countDown(dT);
    if (m_WaveTimer.timeSIsZero())
    {
        m_WaveIndex++;
        for (int i = 0; i < m_LevelData->listUnits.size(); i++)
            m_SpawnUnitCount += m_LevelData->unitsNbPerWave[m_WaveIndex][i].count;

        m_WaveTimer.setTimeMax(m_LevelData->timerBeforeNextWave[m_WaveIndex]);
        m_WaveTimer.resetToMax();
        m_SpawnTimer.resetToMax();
    }

}

void Game::handleSpawnUnits(SDL_Renderer* renderer, float dT)
{
    if (m_SpawnUnitCount > 0)
        spawnUnits(renderer, m_LevelData->unitsNbPerWave[m_WaveIndex], dT);

}

void Game::spawnUnits(SDL_Renderer* renderer, std::vector<UnitCounter>& listEnemies, float dT)
{
    m_SpawnTimer.countDown(dT);

    if (m_SpawnTimer.timeSIsZero())
    {
        if (listEnemies.size() == 0)
            return;

        uint8_t enemyIndex = std::rand() % listEnemies.size();

        //Delete the enemies'type that are fully spawned
        if (listEnemies[enemyIndex].count == 0)
            listEnemies.erase(listEnemies.begin() + enemyIndex);
        else
        {
            addUnit(renderer, m_Level.getRandomEnemySpawnerLocation(), listEnemies[enemyIndex].unitType);
            listEnemies[enemyIndex].count--;
            m_SpawnUnitCount--;
            m_SpawnTimer.resetToMax();
        }
        
    }
}



void Game::draw(SDL_Renderer* renderer) {
    //Draw.
    //Set the draw color to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //Clear the screen.
    SDL_RenderClear(renderer);

    
    
    //Draw everything here.
    //Draw the m_Level.
    m_Level.draw(renderer, tileSize);

    //Draw the enemy units.
    for (auto& unitSelected : m_ListUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, tileSize);
    

    //Draw the forest
    m_Level.drawTrees(renderer, tileSize);

    //Draw the turrets
    for (auto& turretSelected : m_ListTurrets)
        turretSelected.draw(renderer, tileSize);

    //Draw the projectiles
    for (auto& projectileSelected : m_ListProjectiles)
        projectileSelected.draw(renderer, tileSize);

    //Draw the overlay.
    /*if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }*/

    m_ItemPlacementPreview->draw(renderer, tileSize);
    m_UI->draw(renderer);
    //Send the image to the window.
    SDL_RenderPresent(renderer);
}



void Game::addUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type) {

    std::shared_ptr<Unit> pUnit = UnitFactory::createUnit(renderer, pos, type);
    if (pUnit != nullptr)
        m_ListUnits.emplace_back(std::make_shared<Unit>(renderer, pos));
    else
        std::cout << "Erreur lors de la creation de l'unit de type " << (char)type << "\n";
}

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse)
{
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5);
    m_ListTurrets.emplace_back(Turret(renderer, pos));
    m_Level.setTurret((int)posMouse.x, (int)posMouse.y);
}



bool Game::removeTurretsAtMousePosition(Vector2D posMouse)
{
    for (auto it = m_ListTurrets.begin(); it != m_ListTurrets.end();)
    {
        if (it->checkIfOnTile((int)posMouse.x, (int)posMouse.y))
        {
            m_Level.removeTurret((int)posMouse.x, (int)posMouse.y);
            it = m_ListTurrets.erase(it);
            return true;
        }
        else
            it++;
    }

    return false;
       
}

void Game::handleWaves(SDL_Renderer* renderer, float dT)
{
    if (m_WaveIndex != m_TotalWaves-1)
    {
        updateWaveTimer(renderer, dT);
    }
    else
    {
        if (m_ListUnits.empty())
           m_GameStatus->setGameState(GameState::WAITING);
    }
}

bool Game::isGameFinished() const
{
    return m_LevelManager->isGameFinished();
}

void Game::drawVictoryScreen(SDL_Renderer* renderer) const
{
    m_GameStatus->setGameState(GameState::VICTORY);
    SDL_RenderClear(renderer);

    SDL_Texture* texture = TextureLoader::loadTexture(renderer, "Victory.bmp");
    SDL_SetTextureAlphaMod(texture, 125);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect rect = {
        0,
        0,
        w,
        h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

void Game::gameOver(SDL_Renderer* renderer) const
{
    m_GameStatus->setGameState(GameState::GAMEOVER);
    SDL_RenderClear(renderer);

    SDL_Texture* texture = TextureLoader::loadTexture(renderer, "GameOver.bmp");
    SDL_SetTextureAlphaMod(texture, 125);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect rect = {
        0,
        0,
        w,
        h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

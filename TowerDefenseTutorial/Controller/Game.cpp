#include "Game.h"
#include <cstdlib>
#include "../View/UI.h"
#include "UnitFactory.h"
#include "DefenseFactory.h"
#include "../Model/Defense.h"
#include "../Model/DefenseTower.h"
#include "../Model/DefenseTurret.h"
#include "../Model/DefenseExplosion.h"
#include "../Model/Items.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight, int playingAreaWidth, int playingAreaHeight) 
    :
    m_Level(renderer, playingAreaWidth / TILE_SIZE, playingAreaHeight / TILE_SIZE),
    m_InputManager(new InputManager()),
    m_KeyBoardManager(new KeyboardManager()),
    m_GameStatus(new GameStatus(GameState::INIT)),
    m_GameLoop(new GameLoop(*this, *m_GameStatus)),
    m_LevelManager(new LevelLoaderManager("Data/Levels/Levels.xml")),
    m_PlayerManager(new PlayerManager(MAX_LIFE_POINTS, MAX_GEMS)),
    m_LevelView(new LevelView(renderer, playingAreaWidth / TILE_SIZE, playingAreaHeight / TILE_SIZE)),
    m_GameStateDisplay(new GameStateDisplay(renderer, &m_GameStatus->getGameState(), windowWidth, windowHeight)),
    m_Shop(new Shop(m_PlayerManager))
{

    if (window != nullptr && renderer != nullptr) {

        initGame(renderer, windowWidth, windowHeight, playingAreaWidth, playingAreaHeight);
    }
    else
        std::cerr << "Failed to initialise window or renderer\n";
}


Game::~Game() {

    TextureLoader::deallocateTextures();
    delete m_InputManager;
    delete m_ItemPlacementPreview;
    delete m_ItemsInfoPreview;
    delete m_GameLoop;
    delete m_LevelManager;
    delete m_PlayerManager;
    delete m_LevelView;
    delete m_GameStateDisplay;
}

void Game::initGame(SDL_Renderer* renderer, int windowWidth, int windowHeight, int playingAreaWidth, int playingAreaHeight)
{
    std::srand(std::time(nullptr));

    Items::initItems(renderer);

    m_Level.setListUnits(&m_ListUnits);
    m_Level.setOnTargetReachedCallback([this](int damages) { m_PlayerManager->takeDamages(damages); });

    m_LevelView->loadStaticTiles(m_Level.loadLevelMap("Data/Levels/Level2.map"));

    m_UI = UI::getInstance();
    m_UI->initUI(renderer, windowWidth, windowHeight, windowWidth - playingAreaWidth, windowHeight - playingAreaHeight, 
        m_PlayerManager->getPlayer(), m_Shop);

    ItemSelectionZone TowerZone(30, playingAreaHeight+30, 120, 100, itemEnum::TowerItem);
    ItemSelectionZone TurretZone(185, playingAreaHeight + 30, 120, 100, itemEnum::TurretItem);
    ItemSelectionZone BombZone(335, playingAreaHeight + 30, 120, 100, itemEnum::ExplosionItem);
    ItemSelectionZone PlayingZone(0, 0, windowWidth, windowHeight * 0.8);
    TowerZone.setOnClickListener([this](itemEnum item, int mouseButtonStatus, int x, int y) { if (mouseButtonStatus == 1) m_UI->selectItem(item, x, y); });
    TurretZone.setOnClickListener([this](itemEnum item, int mouseButtonStatus, int x, int y) { if (mouseButtonStatus == 1) m_UI->selectItem(item, x, y); });
    BombZone.setOnClickListener([this](itemEnum item, int mouseButtonStatus, int x, int y) { if (mouseButtonStatus == 1) m_UI->selectItem(item, x, y); });
    PlayingZone.setOnClickListener([this, renderer](itemEnum item, int mouseButtonStatus, int x, int y) { processMouseEvents(renderer, mouseButtonStatus, x, y); });
    //UI
    m_InputManager->addItemSelectionZone(&TowerZone);
    m_InputManager->addItemSelectionZone(&TurretZone);
    m_InputManager->addItemSelectionZone(&BombZone);
    //Game
    m_InputManager->addItemSelectionZone(&PlayingZone);

    m_SelectedItem = m_UI->getSelectedItem();

    m_ItemPlacementPreview = new ItemPlacementPreview(renderer, m_Level, *m_Shop, 0, 0, windowWidth, tileSize * 14);
    m_ItemsInfoPreview = new ItemsInfoPreview(renderer, &m_Level);
    m_ItemsInfoPreview->addItemInfoZone(itemEnum::TowerItem, &TowerZone);
    m_ItemsInfoPreview->addItemInfoZone(itemEnum::TurretItem, &TurretZone);
    m_ItemsInfoPreview->addItemInfoZone(itemEnum::ExplosionItem, &BombZone);
    m_ItemsInfoPreview->setPlayingZone(&PlayingZone);

    m_InputManager->setMouseMovementCallback([this](int x, int y) {
        m_ItemPlacementPreview->onMove(x, y);
        m_ItemsInfoPreview->onMove(x, y); });
    m_InputManager->setKeyPressedCallback([this](int key) { processKeyboardEvents(key); });

    m_PlayerManager->setOnPlayersDeathCallback([this, renderer]() { gameOver(renderer); });

    

    m_GameLoop->start(renderer);
}


void Game::loadNextWave()
{
    m_WaveData = m_LevelManager->loadNextWave();

    m_AssaultTimer.setTimeMax(m_WaveData.timerBeforeNextAssault[0]);
    m_AssaultTimer.resetToZero();
    m_SpawnTimer.resetToZero();
    m_TotalAssaults = m_WaveData.totalAssaultsNb;
    m_AssaultIndex = -1;
    
}

void Game::clearLevel()
{
    m_ListUnits.clear();
    m_ListDefenses.clear();
    m_Level.resetMapTiles();
    //m_ListWalls.clear();
}

void Game::startLevel()
{
    clearLevel();
    m_WaveData = m_LevelManager->loadLevel(0);
    m_TotalAssaults = m_WaveData.totalAssaultsNb;
    m_AssaultTimer.resetToZero();
    m_SpawnTimer.resetToZero();
    m_AssaultIndex = -1;
    m_Level.startPathfinding();
    m_GameStatus->setGameState(GameState::RUNNING);
}

void Game::startWave()
{
    loadNextWave();
    //m_Level.startPathfinding();
    m_GameStatus->setGameState(GameState::RUNNING);
}

void Game::waitForNextWave()
{
    m_GameStatus->setGameState(GameState::WAITING);
    m_GameStateDisplay->resetTimer();
    //m_Level.pausePathfinding();
}


void Game::handleEvents(SDL_Renderer* renderer, GameState& gameState)
{
    m_InputManager->handleEvents(renderer, gameState);
}





void Game::processMouseEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY) {
    //bool mouseDownThisFrame = false;
    
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    if (m_Shop->isBuyable(*m_SelectedItem) && mouseButtonStatus > 0) {
        switch (mouseButtonStatus) {
        case SDL_BUTTON_LEFT:

            if (m_ItemPlacementPreview->isItemPlacementEnabled())
            {
                Defense* selectedItem = Items::getItemData(*m_SelectedItem);
                switch (*m_SelectedItem) {
                case itemEnum::TowerItem:
                    if (selectedItem->getCooldownTimer().timeSIsZero())
                    {
                        selectedItem->resetCooldown();
                        //Add wall at the mouse position.
                        if (!m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
                        {
                            addDefense<Tower>(renderer, posMouse);
                            m_Level.setTileWall((int)posMouse.x, (int)posMouse.y);
                            m_Shop->purchaseItem(*m_SelectedItem);
                            *m_SelectedItem = itemEnum::None;
                            m_ItemPlacementPreview->disablePreview();
                        }

                    }
                    break;
                case itemEnum::TurretItem:
                    //Add the selected unit at the mouse position.
                    if (selectedItem->getCooldownTimer().timeSIsZero())
                    {
                        selectedItem->resetCooldown();
                        if (m_Level.isTileWall((int)posMouse.x, (int)posMouse.y) && !m_Level.isTurret((int)posMouse.x, (int)posMouse.y))
                        {
                            addDefense<Turret>(renderer, posMouse);
                            m_Level.setTurret((int)posMouse.x, (int)posMouse.y);
                            //addTurret(renderer, posMouse);
                            m_Shop->purchaseItem(*m_SelectedItem);
                            *m_SelectedItem = itemEnum::None;
                            m_ItemPlacementPreview->disablePreview();
                        }
                    }
                    break;
                case itemEnum::ExplosionItem:
                    if (selectedItem->getCooldownTimer().timeSIsZero())
                    {
                        selectedItem->resetCooldown();
                        if (!m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
                        {
                            addDefense<Explosion>(renderer, posMouse);
                            //m_Level.setTileWall((int)posMouse.x, (int)posMouse.y);
                            m_Shop->purchaseItem(*m_SelectedItem);
                            *m_SelectedItem = itemEnum::None;
                            m_ItemPlacementPreview->disablePreview();
                        }
                    }
                    break;
                }


            }
            break;
        }
    };

    if (mouseButtonStatus == SDL_BUTTON_RIGHT)
    {
        //TODO Refactor this part
        //  -> Add a collection for each type of defense to handle individually each erase
            
        //Remove turret is on tile
        if (m_Level.isTurret((int)posMouse.x, (int)posMouse.y) || m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
        {
            removeTurretsAtMousePosition(posMouse);
            m_Level.clearTile((int)posMouse.x, (int)posMouse.y);
                //m_Shop->sellItem(itemEnum::TurretItem);*
                //m_Level.removeTurret((int)posMouse.x, (int)posMouse.y);
                
        }
        /*else if (m_Level.isTileWall((int)posMouse.x, (int)posMouse.y))
        {
            m_Level.removeWall((int)posMouse.x, (int)posMouse.y);
            //m_Shop->sellItem(itemEnum::TowerItem);
        }*/

            
        
    }
}

void Game::processKeyboardEvents(int key)
{
    switch (key)
    {
    case SDLK_RETURN:
        if (m_GameStatus->getGameState() == GameState::INIT)
            startLevel();
        else if (m_GameStatus->getGameState() == GameState::WAITING)
            startWave();
        else if (m_GameStatus->getGameState() == GameState::VICTORY || m_GameStatus->getGameState() == GameState::GAMEOVER)
            startLevel();
        break;
    case SDLK_ESCAPE:
        if (m_GameStatus->getGameState() == GameState::VICTORY || m_GameStatus->getGameState() == GameState::GAMEOVER)
            m_GameStatus->setGameState(GameState::STOPPED);
        break;
    }
}


void Game::update(SDL_Renderer* renderer, float dT) {
    
    handleWave(renderer, dT);
    handleSpawnUnits(renderer, dT);

    updateUnits(dT);
    updateDefenses(renderer, dT);

    updateProjectiles(dT);
    updateGameStateDisplay(dT);

    m_PlayerManager->update(dT);
    m_UI->update(dT);
    
}


void Game::updateUnits(float dT) {
    //Loop through the list of units and update all of them.
    auto it = m_ListUnits.begin();
    while (it != m_ListUnits.end()) 
    {
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

void Game::updateDefenses(SDL_Renderer* renderer, float dT)
{
    auto it = m_ListDefenses.begin();
    while (it != m_ListDefenses.end())
    {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(renderer, dT, m_ListUnits, m_ListProjectiles);

            //Check if the defense is still alive.  If not then erase it and don't increment the iterator.
            if ((*it)->isAlive() == false) {
                it = m_ListDefenses.erase(it);
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

void Game::updateAssaultTimer(SDL_Renderer* renderer, float dT)
{

    m_AssaultTimer.countDown(dT);
    if (m_AssaultTimer.timeSIsZero())
    {
        m_AssaultIndex++;
        for (int i = 0; i < m_WaveData.listUnits.size(); i++)
            m_SpawnUnitCount += m_WaveData.unitsNbPerAssault[m_AssaultIndex][i].count;

        m_AssaultTimer.setTimeMax(m_WaveData.timerBeforeNextAssault[m_AssaultIndex]);
        m_AssaultTimer.resetToMax();
        //m_SpawnTimer.resetToMax();
        
    }

}

void Game::updateGameStateDisplay(float dT)
{
    m_GameStateDisplay->countDownTimer(dT);

}



void Game::handleSpawnUnits(SDL_Renderer* renderer, float dT)
{
    if (m_SpawnUnitCount > 0)
        spawnUnits(renderer, m_WaveData.unitsNbPerAssault[m_AssaultIndex], dT);

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

    
    m_LevelView->draw(renderer);
    //m_Level.drawWalls(renderer, tileSize);

    //Draw the enemy units.
    for (auto& unitSelected : m_ListUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, tileSize);

    m_LevelView->drawObstructTiles(renderer);

    //Draw the turrets
    for (auto& defenseSelected : m_ListDefenses)
        defenseSelected->draw(renderer, tileSize);

    //Draw the projectiles
    for (auto& projectileSelected : m_ListProjectiles)
        projectileSelected.draw(renderer, tileSize);

    m_ItemPlacementPreview->draw(renderer, tileSize);
    m_UI->draw(renderer);

    m_ItemsInfoPreview->draw(renderer, tileSize);

    m_GameStateDisplay->draw(renderer);

    SDL_RenderPresent(renderer);
}



void Game::addUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type) {

    std::shared_ptr<Unit> pUnit = UnitFactory::createUnit(renderer, pos, type, [this](uint8_t gems) {
        //m_Shop->addMoney(gems); 
      });

    if (pUnit != nullptr)
        //m_ListUnits.emplace_back(std::make_shared<Unit>(renderer, pos));
        m_ListUnits.emplace_back(pUnit);
    else
        std::cerr << "Erreur lors de la creation de l'unit de type " << (char)type << "\n";
}

template<typename T>
void Game::addDefense(SDL_Renderer* renderer, Vector2D posMouse)
{
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5);
    
    m_ListDefenses.emplace_back(DefenseFactory::createDefense<T>(renderer, posMouse));
    //m_Level.setTileWall((int)posMouse.x, (int)posMouse.y);
}

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse)
{
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5);
    m_ListDefenses.emplace_back(std::make_unique<Tower>(renderer, pos));
    m_Level.setTurret((int)posMouse.x, (int)posMouse.y);
}



bool Game::removeTurretsAtMousePosition(Vector2D posMouse)
{
    bool ret = false;
    for (auto it = m_ListDefenses.begin(); it != m_ListDefenses.end();)
    {
        if ((*it)->checkIfOnTile((int)posMouse.x, (int)posMouse.y))
        {
            m_Level.removeTurret((int)posMouse.x, (int)posMouse.y);
            it = m_ListDefenses.erase(it);
            //return true;
            ret = true;
        }
        else
            it++;
    }

    return ret;
       
}

void Game::handleWave(SDL_Renderer* renderer, float dT)
{
    if (m_GameStatus->getGameState() == GameState::RUNNING)
    {
        if (m_AssaultIndex < m_TotalAssaults-1)
        {
            updateAssaultTimer(renderer, dT);
        }
        else
        {
            if (m_ListUnits.empty())
            {
                if (isGameFinished())
                    m_GameStatus->setGameState(GameState::VICTORY);
                else
                {
                    waitForNextWave();
                }
            }
        }   
    }
}

bool Game::isGameFinished() const
{
    return m_LevelManager->isLevelFinished();
}

void Game::drawVictoryScreen(SDL_Renderer* renderer) const
{
    
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
    /*SDL_RenderClear(renderer);

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
    SDL_RenderPresent(renderer);*/
}

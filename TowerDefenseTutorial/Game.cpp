#include "Game.h"
#include "View/UI.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    placementModeCurrent(PlacementMode::wall),
    level(renderer, windowWidth / tileSize, (windowHeight * 0.8) / tileSize, Vector2D((windowWidth / (tileSize * 2)) , (windowHeight * 0.8) / (tileSize * 2))),
    spawnTimer(0.25f), roundTimer(5.0f),
    m_InputManager(new InputManager())
{

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
        
        std::cout << windowWidth << ", " << windowHeight << "\n";
        m_UI = UI::getInstance();
        m_UI->initUI(renderer, windowWidth, windowHeight);
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

        m_ItemPlacementPreview = new ItemPlacementPreview(renderer, listTurrets, level, *m_Shop, 0, 0, windowWidth, windowHeight * 0.8);
        m_InputManager->setMouseMovementCallback([this](int x, int y) { m_ItemPlacementPreview->onMove(x, y); });

        //Load the overlay texture.
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");

        //Store the current times for the clock.
        auto time1 = std::chrono::system_clock::now();
        auto time2 = std::chrono::system_clock::now();

        //The amount of time for each frame (60 fps).
        const float dT = 1.0f / 60.0f;


        //Start the game loop and run until it's time to stop.
        bool running = true;
        while (running) {
            //Determine how much time has elapsed since the last frame.
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;

                //processEvents(renderer, running);
                m_InputManager->handleEvents(renderer, running);
                update(renderer, dT);
                draw(renderer);
            }
        }
    }
}


Game::~Game() {
    //Clean up.
    TextureLoader::deallocateTextures();
    delete m_InputManager;
    delete m_ItemPlacementPreview;
}



void Game::processEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY) {
    bool mouseDownThisFrame = false;
    
    /*//Process events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouseDownThisFrame = (mouseDownStatus == 0);
            if (event.button.button == SDL_BUTTON_LEFT)
                mouseDownStatus = SDL_BUTTON_LEFT;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mouseDownStatus = SDL_BUTTON_RIGHT;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseDownStatus = 0;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                //Quit the game.
            case SDL_SCANCODE_ESCAPE:
                running = false;
                break;

                //Set the current gamemode.
            case SDL_SCANCODE_1:
                placementModeCurrent = PlacementMode::wall;
                break;
            case SDL_SCANCODE_2:
                placementModeCurrent = PlacementMode::turret;
                break;

                //Show/hide the overlay
            case SDL_SCANCODE_H:
                overlayVisible = !overlayVisible;
                break;
            }
        }
    }
    */

    //Process input from the mouse cursor.
    //int mouseX = 0, mouseY = 0;
    //SDL_GetMouseState(&mouseX, &mouseY);
    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    if (m_Shop->isBuyable(*m_SelectedItem) && mouseButtonStatus > 0) {
        switch (mouseButtonStatus) {
        case SDL_BUTTON_LEFT:
            
            switch (*m_SelectedItem) {
            case itemEnum::WallItem:
                //Add wall at the mouse position.
                if (!level.isTileWall((int)posMouse.x, (int)posMouse.y))
                {
                    level.setTileWall((int)posMouse.x, (int)posMouse.y);
                    m_Shop->purchaseItem(*m_SelectedItem);
                }
                break;
            case itemEnum::TurretItem:
                //Add the selected unit at the mouse position.
                //if (mouseDownThisFrame)
                if (level.isTileWall((int)posMouse.x, (int)posMouse.y) && !level.isTurret(listTurrets, (int)posMouse.x, (int)posMouse.y))
                {
                    addTurret(renderer, posMouse);
                    m_Shop->purchaseItem(*m_SelectedItem);
                }
                break;
            }
            
            break;


        case SDL_BUTTON_RIGHT:
            
            //Remove turret is on tile
            if (removeTurretsAtMousePosition(posMouse))
            {
                m_Shop->sellItem(itemEnum::TurretItem);
                break;
            }

            if (level.isTileWall((int)posMouse.x, (int)posMouse.y))
            {
                level.removeWall((int)posMouse.x, (int)posMouse.y);
                m_Shop->sellItem(itemEnum::WallItem);
            }

            break;
        }
    }
}



void Game::update(SDL_Renderer* renderer, float dT) {
    //Update the units.
    updateUnits(dT);

    //Update the turrets
    for (auto& turretSelected : listTurrets)
        turretSelected.update(renderer, dT, listUnits, listProjectiles);

    //Update the projectiles
    updateProjectiles(dT);

    updateSpawnUnitsIfRequired(renderer, dT);
}


void Game::updateUnits(float dT) {
    //Loop through the list of units and update all of them.
    auto it = listUnits.begin();
    while (it != listUnits.end()) {
        bool increment = true;

        if ((*it) != nullptr) {
            (*it)->update(dT, level, listUnits);

            //Check if the unit is still alive.  If not then erase it and don't increment the iterator.
            if ((*it)->isAlive() == false) {
                it = listUnits.erase(it);
                increment = false;
            }
        }

        if (increment)
            it++;
    }
}

void Game::updateProjectiles(float dT)
{
    auto projIt = listProjectiles.begin();
    while (projIt != listProjectiles.end())
    {
        projIt->update(dT, listUnits);

        //Check if the projectile has collided or not
        if (projIt->getCollisionOccured())
            projIt = listProjectiles.erase(projIt);
        else
            projIt++;
    }
        
}

void Game::updateSpawnUnitsIfRequired(SDL_Renderer* renderer, float dT)
{
    spawnTimer.countDown(dT);

    //Check if the round needs to start.
    if (listUnits.empty() && spawnUnitCount == 0)
    {
        roundTimer.countDown(dT);
        if (roundTimer.timeSIsZero())
        {
            spawnUnitCount = 20;
            roundTimer.resetToMax();
        }
    }

    //Add a unit it needed.
    if (spawnUnitCount > 0 && spawnTimer.timeSIsZero())
    {
        addUnit(renderer, level.getRandomEnemySpawnerLocation());
        spawnUnitCount--;
        spawnTimer.resetToMax();
    }
}



void Game::draw(SDL_Renderer* renderer) {
    //Draw.
    //Set the draw color to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //Clear the screen.
    SDL_RenderClear(renderer);


    
    //Draw everything here.
    //Draw the level.
    level.draw(renderer, tileSize);

    //Draw the enemy units.
    for (auto& unitSelected : listUnits)
        if (unitSelected != nullptr)
            unitSelected->draw(renderer, tileSize);
    
    //Draw the turrets
    for (auto& turretSelected : listTurrets)
        turretSelected.draw(renderer, tileSize);


    //Draw the projectiles
    for (auto& projectileSelected : listProjectiles)
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



void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
    listUnits.emplace_back(std::make_shared<Unit>(renderer, posMouse));
}

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse)
{
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5);
    listTurrets.emplace_back(Turret(renderer, pos));
}



bool Game::removeTurretsAtMousePosition(Vector2D posMouse)
{
    for (auto it = listTurrets.begin(); it != listTurrets.end();)
    {
        if (it->checkIfOnTile((int)posMouse.x, (int)posMouse.y))
        {
            it = listTurrets.erase(it);
            return true;
        }
        else
            it++;
    }

    return false;
       
}
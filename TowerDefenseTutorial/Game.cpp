#include "Game.h"



Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
    placementModeCurrent(PlacementMode::wall), 
    level(renderer, windowWidth / tileSize, windowHeight / tileSize),
    spawnTimer(0.25f), roundTimer(5.0f)
{

    //Run the game.
    if (window != nullptr && renderer != nullptr) {
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

                processEvents(renderer, running);
                update(renderer, dT);
                draw(renderer);
            }
        }
    }
}


Game::~Game() {
    //Clean up.
    TextureLoader::deallocateTextures();
}



void Game::processEvents(SDL_Renderer* renderer, bool& running) {
    bool mouseDownThisFrame = false;

    //Process events.
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


    //Process input from the mouse cursor.
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    //Convert from the window's coordinate system to the game's coordinate system.
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);

    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
        case SDL_BUTTON_LEFT:
            switch (placementModeCurrent) {
            case PlacementMode::wall:
                //Add wall at the mouse position.
                level.setTileWall((int)posMouse.x, (int)posMouse.y, true);
                break;
            case PlacementMode::turret:
                //Add the selected unit at the mouse position.
                if (mouseDownThisFrame)
                    addTurret(renderer, posMouse);
                break;
            }
            break;


        case SDL_BUTTON_RIGHT:
            //Remove wall at the mouse position.
            level.setTileWall((int)posMouse.x, (int)posMouse.y, false);
            //Remove units at the mouse position.
            removeTurretsAtMousePosition(posMouse);
            break;
        }
    }
}



void Game::update(SDL_Renderer* renderer, float dT) {
    //Update the units.
    for (auto unitIt = listUnits.begin(); unitIt != listUnits.end();)
    {
        unitIt->update(dT, level, listUnits);

        if (unitIt->getIsAlive() == false)
           unitIt = listUnits.erase(unitIt);
        else
            unitIt++;

    }


    //Update the turrets
    for (auto& turretSelected : listTurrets)
        turretSelected.update(dT);

    updateSpawnUnitsIfRequired(renderer, dT);
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
            spawnUnitCount = 15;
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
        unitSelected.draw(renderer, tileSize);
    
    //Draw the turrets
    for (auto& turretSelected : listTurrets)
        turretSelected.draw(renderer, tileSize);

    //Draw the overlay.
    if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }

    //Send the image to the window.
    SDL_RenderPresent(renderer);
}



void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
    listUnits.push_back(Unit(renderer, posMouse));
}

void Game::addTurret(SDL_Renderer* renderer, Vector2D posMouse)
{
    Vector2D pos((int)posMouse.x + 0.5f, (int)posMouse.y + 0.5);
    listTurrets.push_back(Turret(renderer, pos));
}



void Game::removeTurretsAtMousePosition(Vector2D posMouse)
{
    for (auto it = listTurrets.begin(); it != listTurrets.end();)
    {
        if (it->checkIfOnTile((int)posMouse.x, (int)posMouse.y))
            it = listTurrets.erase(it);
        else
            it++;
    }
       
}
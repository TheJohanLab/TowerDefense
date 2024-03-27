#include "GameLoop.h"
#include <chrono>
#include "Game.h"


int CustomEventFilter(void* userdata, SDL_Event* event)
{
    GameState gameState = *((GameState*)userdata);
    if ((gameState == GameState::WAITING || gameState == GameState::PAUSED) && (event->type != SDL_KEYUP && event->key.keysym.sym != SDLK_ESCAPE) &&
        event->type != SDL_QUIT) {
        return 0;
    }
    return 1; 
}

GameLoop::GameLoop(Game& game, GameStatus& gameStatus)
    :m_GameManager(game), m_GameStatus(gameStatus)
{
}

GameLoop::~GameLoop()
{
}


void GameLoop::start(SDL_Renderer* renderer)
{
    //Store the current times for the clock.
    auto time1 = std::chrono::system_clock::now();
    auto time2 = std::chrono::system_clock::now();

    //The amount of time for each frame (60 fps).
    const float dT = 1.0f / 60.0f;

    //Start the game loop and run until it's time to stop.
    m_GameStatus.setGameState(GameState::RUNNING);
    SDL_SetEventFilter(CustomEventFilter, &m_GameStatus.getGameState());

    while (m_GameStatus.getGameState() != GameState::STOPPED) {
        //Determine how much time has elapsed since the last frame.
        if (m_GameStatus.getGameState() == GameState::RUNNING)
        {
            time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            //If enough time has passed then do everything required to generate the next frame.
            if (timeDeltaFloat >= dT) {
                //Store the new time for the next frame.
                time1 = time2;
                
                m_GameManager.update(renderer, dT);
                m_GameManager.draw(renderer);
            }
        }
        
        if (m_GameStatus.getGameState() == GameState::WAITING)
        {
            if (!m_GameManager.isGameFinished())
            {
                m_GameManager.clearLevel();
                m_GameManager.loadNextLevel();
                m_GameManager.clearLevel();
            }           
        }

        if (m_GameStatus.getGameState() == GameState::VICTORY)
            m_GameManager.drawVictoryScreen(renderer);
        
        if (m_GameStatus.getGameState() == GameState::GAMEOVER)
            m_GameManager.gameOver(renderer);


        m_GameManager.handleEvents(renderer, m_GameStatus.getGameState());
    }

}

void GameLoop::pause()
{
    m_GameStatus.setGameState(GameState::PAUSED);
}

void GameLoop::resume()
{
    m_GameStatus.setGameState(GameState::RUNNING);
}

void GameLoop::stop()
{
    m_GameStatus.setGameState(GameState::STOPPED);
}



#include "InputManager.h"

InputManager::InputManager()
    :m_Event(new SDL_Event), m_MouseDownStatus(0)
{
}

InputManager::~InputManager()
{
    delete m_Event;
}


void InputManager::handleEvents(SDL_Renderer* renderer, GameState& gameState)
{
    
    //Process events.
    while (SDL_PollEvent(m_Event)) {

        switch (m_Event->type) {
        case SDL_KEYDOWN:
            if (m_Event->key.keysym.sym == SDLK_SPACE)
            {
                gameState = gameState == GameState::RUNNING ? GameState::PAUSED : GameState::RUNNING;
                break;
            }
            if (m_Event->key.keysym.sym == SDLK_RETURN)
            { 
                handleKeyPressed(SDLK_RETURN);
                break;
            }
        case SDL_QUIT:
            gameState = GameState::STOPPED;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (m_Event->button.button == SDL_BUTTON_LEFT)
            {
                m_MouseDownStatus = SDL_BUTTON_LEFT;
                handleLeftMouseClick(m_MouseDownStatus, m_Event->button.x, m_Event->button.y);
            }
            else if (m_Event->button.button == SDL_BUTTON_RIGHT)
            {
                m_MouseDownStatus = SDL_BUTTON_RIGHT;
                handleRightMouseClick(m_MouseDownStatus, m_Event->button.x, m_Event->button.y);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            m_MouseDownStatus = 0;
            break;

        case SDL_MOUSEMOTION:
            handleMouseMotion(m_Event->button.x, m_Event->button.y);
            break;            
        }
        
        
        
    }
}

void InputManager::handleLeftMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const
{
    for (auto zone : m_ItemSelectionZones)
    {
        if (zone->isOnZone(mouseX, mouseY))
            zone->onClick(mouseButtonStatus, mouseX, mouseY);
    }
}

void InputManager::handleRightMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const
{
    for (auto zone : m_ItemSelectionZones)
    {
        if (zone->isOnZone(mouseX, mouseY))
            zone->onClick(mouseButtonStatus, mouseX, mouseY);
    }
}

void InputManager::handleMouseUnClick(int mouseButtonStatus, int mouseX, int mouseY) const
{
}

void InputManager::handleMouseMotion(int mouseX, int mouseY) const
{
    m_OnMouseMoveCallback(mouseX, mouseY);
}

void InputManager::handleKeyPressed(int key) const
{
    m_OnKeyPressedCallback(key);
}

void InputManager::addItemSelectionZone(ItemSelectionZone* zone)
{
    m_ItemSelectionZones.emplace_back(zone);
}

void InputManager::setMouseMovementCallback(std::function<void(int, int)> onMouseMoveCallback)
{
    m_OnMouseMoveCallback = onMouseMoveCallback;
}

void InputManager::setKeyPressedCallback(std::function<void( int)> onKeyPressedCallback)
{
    m_OnKeyPressedCallback = onKeyPressedCallback;
}



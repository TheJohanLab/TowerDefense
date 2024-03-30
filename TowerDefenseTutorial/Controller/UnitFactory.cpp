#include "UnitFactory.h"

std::map<UnitType, std::function<std::shared_ptr<Unit>(SDL_Renderer*, Vector2D, std::function<void(uint8_t)>)>> UnitFactory::m_UnitMap = 
{
    { UnitType::MUSHROOM, &UnitFactory::createMushroom },
    { UnitType::BAT, &UnitFactory::createBat }
};

UnitFactory::UnitFactory()
{
}

UnitFactory::~UnitFactory()
{
}

std::shared_ptr<Unit> UnitFactory::createUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type, std::function<void(uint8_t)> callback)
{
    auto it = m_UnitMap.find(type);
    if (it != m_UnitMap.end()) {
        return it->second(renderer, pos, callback);
    }
    return nullptr;
}

std::shared_ptr<Unit> UnitFactory::createMushroom(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> callback)
{

    return std::make_shared<Unit>(renderer, pos, callback);
    
}

std::shared_ptr<Unit> UnitFactory::createBat(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> callback)
{
    return std::make_shared<Unit>(renderer, pos, callback);
}

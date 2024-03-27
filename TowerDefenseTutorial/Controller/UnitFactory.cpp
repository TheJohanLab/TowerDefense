#include "UnitFactory.h"

std::map<UnitType, std::function<std::shared_ptr<Unit>(SDL_Renderer*, Vector2D)>> UnitFactory::m_UnitMap = {
    { UnitType::MUSHROOM, &UnitFactory::createMushroom }
    
};

UnitFactory::UnitFactory()
{
    //m_UnitMap[UnitType::MUSHROOM] = UnitFactory::createMushroom;
}

UnitFactory::~UnitFactory()
{
}

std::shared_ptr<Unit> UnitFactory::createUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type)
{
    auto it = m_UnitMap.find(type);
    if (it != m_UnitMap.end()) {
        return it->second(renderer, pos);
    }
    return nullptr;
}

std::shared_ptr<Unit> UnitFactory::createMushroom(SDL_Renderer* renderer, Vector2D pos)
{

    return std::make_shared<Unit>(renderer, pos);
    
}
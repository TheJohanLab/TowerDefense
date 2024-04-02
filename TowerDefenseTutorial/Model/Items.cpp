#include "Items.h"

std::map<itemEnum, std::unique_ptr<Defense>> Items::m_ItemMap = std::map<itemEnum, std::unique_ptr<Defense>>();

Items::Items()
{
}

Items::~Items()
{
}

void Items::initItems(SDL_Renderer* renderer)
{
	m_ItemMap[itemEnum::TowerItem] = std::make_unique<Tower>(renderer, Vector2D(0.0f, 0.0f));
	m_ItemMap[itemEnum::TurretItem] = std::make_unique<Turret>(renderer, Vector2D(0.0f, 0.0f));
	m_ItemMap[itemEnum::ExplosionItem] = std::make_unique<Explosion>(renderer, Vector2D(0.0f, 0.0f));
}

const Defense* Items::getItemData(itemEnum item)
{
	if (m_ItemMap.count(item) > 0) {
		return m_ItemMap[item].get();
	}
	else {
		return nullptr;
	}
}

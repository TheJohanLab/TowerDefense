#include "Items.h"

std::map<itemEnum, std::shared_ptr<Defense>> Items::m_ItemMap = std::map<itemEnum, std::shared_ptr<Defense>>();

Items::Items()
{
}

Items::~Items()
{
}

void Items::initItems(SDL_Renderer* renderer)
{
	m_ItemMap[itemEnum::TowerItem] = std::make_shared<Tower>(renderer, Vector2D(0.0f, 0.0f));
	m_ItemMap[itemEnum::TurretItem] = std::make_shared<Turret>(renderer, Vector2D(0.0f, 0.0f));
	m_ItemMap[itemEnum::ExplosionItem] = std::make_shared<Explosion>(renderer, Vector2D(0.0f, 0.0f));
}

Defense* Items::getItemData(itemEnum item)
{
	if (m_ItemMap.count(item) > 0) {
		return m_ItemMap[item].get();
	}
	else {
		return nullptr;
	}
}

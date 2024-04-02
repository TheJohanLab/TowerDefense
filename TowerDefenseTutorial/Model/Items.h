#pragma once

#include <map>
#include <memory>
#include "DefenseTower.h"
#include "DefenseTurret.h"
#include "DefenseExplosion.h"
#include "../Utils/Utils.h"

class Items
{

private:

	static std::map<itemEnum, std::unique_ptr<Defense>> m_ItemMap;

public:
	Items();
	~Items();

	static void initItems(SDL_Renderer* renderer);
	static const Defense* getItemData(itemEnum item);
	

};

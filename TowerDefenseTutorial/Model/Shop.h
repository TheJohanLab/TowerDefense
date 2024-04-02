#pragma once

#include <iostream>
#include <map>
#include "../Utils/Utils.h"
#include "../Model/Items.h"
#include "../Controller/PlayerManager.h"


class Shop
{

private:
	//uint8_t m_Money = 10;
	std::map<itemEnum, uint8_t> m_ItemPrices =
	{
		{itemEnum::TowerItem , 1},
		{itemEnum::TurretItem , 2},
		{itemEnum::ExplosionItem , 3},
	};

	PlayerManager* m_Player;

public:
	Shop(PlayerManager* player);
	~Shop();

	void purchaseItem(itemEnum item);
	//void sellItem(itemEnum item);
	//void addMoney(uint8_t amount);
	bool isBuyable(itemEnum item) const;
	uint16_t getMoneyAmount() const;
	uint8_t getItemPrice(itemEnum itemEnum) const;
};

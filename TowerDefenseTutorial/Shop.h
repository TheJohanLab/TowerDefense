#pragma once

#include <iostream>
#include "Utils.h"
#include <map>


class Shop
{

private:
	uint16_t m_Money = 100;
	std::map<itemEnum, uint16_t> m_ItemPrices =
	{
		{itemEnum::WallItem , 10},
		{itemEnum::TurretItem , 15},
	};

public:
	Shop();
	~Shop();

	void purchaseItem(itemEnum item);
	void sellItem(itemEnum item);
	void addMoney(uint16_t amount);
	bool isBuyable(itemEnum item) const;
	uint16_t getMoneyAmount() const;
};

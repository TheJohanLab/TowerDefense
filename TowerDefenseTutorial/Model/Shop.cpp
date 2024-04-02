#include "Shop.h"

Shop::Shop(PlayerManager* player)
	:m_Player(player)
{
}

Shop::~Shop()
{
}

void Shop::purchaseItem(itemEnum item)
{
	m_Player->loseGems(m_ItemPrices.at(item));
}

//void Shop::sellItem(itemEnum item)
//{
//	m_Money = std::min(10, m_Money + m_ItemPrices.at(item));
//}
//
//void Shop::addMoney(uint8_t amount)
//{
//	m_Money = std::min(10, m_Money + amount);
//}

bool Shop::isBuyable(itemEnum item) const
{
	if (item == itemEnum::None)
		return false;
	return (m_ItemPrices.at(item) <= m_Player->getCurrentGems());
}

uint16_t Shop::getMoneyAmount() const
{
	return m_Player->getCurrentGems();
}

uint8_t Shop::getItemPrice(itemEnum itemEnum) const
{
	return m_ItemPrices.at(itemEnum);
}

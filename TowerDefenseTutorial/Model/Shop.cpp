#include "Shop.h"

Shop::Shop()
{
}

Shop::~Shop()
{
}

void Shop::purchaseItem(itemEnum item)
{
	m_Money = std::max(0, m_Money - m_ItemPrices.at(item));
}

void Shop::sellItem(itemEnum item)
{
	m_Money = std::min(10, m_Money + m_ItemPrices.at(item));
}

void Shop::addMoney(uint8_t amount)
{
	m_Money = std::min(10, m_Money + amount);
}

bool Shop::isBuyable(itemEnum item) const
{
	if (item == itemEnum::None)
		return false;
	return (m_ItemPrices.at(item) <= m_Money);
}

uint16_t Shop::getMoneyAmount() const
{
	return m_Money;
}

uint8_t Shop::getItemPrice(itemEnum itemEnum) const
{
	return m_ItemPrices.at(itemEnum);
}

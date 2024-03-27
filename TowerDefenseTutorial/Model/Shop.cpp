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
	m_Money = std::min(999, m_Money + m_ItemPrices.at(item) / 2);
}

void Shop::addMoney(uint16_t amount)
{
	m_Money = std::min(999, m_Money + amount);
}

bool Shop::isBuyable(itemEnum item) const
{
	return (m_ItemPrices.at(item) <= m_Money);
}

uint16_t Shop::getMoneyAmount() const
{
	return m_Money;
}

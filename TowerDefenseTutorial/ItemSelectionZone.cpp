#include "ItemSelectionZone.h"

ItemSelectionZone::ItemSelectionZone()
	:m_RectZone({ 0,0,0,0 }), m_Item(itemEnum::None)
{
}

ItemSelectionZone::ItemSelectionZone(int x, int y, int w, int h, itemEnum item)
	:m_RectZone({x,y,w,h}), m_Item(item)
{
}

ItemSelectionZone::ItemSelectionZone(int x, int y, int w, int h)
	: m_RectZone({ x,y,w,h }), m_Item(itemEnum::None)
{
}

ItemSelectionZone::~ItemSelectionZone()
{
}


void ItemSelectionZone::setZone(const SDL_Rect& zone)
{
	m_RectZone = zone;
}

void ItemSelectionZone::setOnClickListener(std::function<void(itemEnum item, int, int, int)> onClickFunction)
{
	m_OnClickFunction = onClickFunction;
}

void ItemSelectionZone::onClick(int mouseButtonStatus, int x, int y)
{
	if (m_OnClickFunction)
		m_OnClickFunction(m_Item,mouseButtonStatus, x, y);
}

bool ItemSelectionZone::isOnZone(int x, int y) const
{

	if (x > m_RectZone.x && x < (m_RectZone.x + m_RectZone.w) && 
		y > m_RectZone.y && y < (m_RectZone.y + m_RectZone.h))
		return true;

	return false;
}

inline const SDL_Rect& ItemSelectionZone::getZone() const
{
	return m_RectZone;
}

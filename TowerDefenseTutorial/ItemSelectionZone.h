#pragma once

#include "SDL2/SDL.h"
#include "Listeners/OnClickListener.h"
//#include "View/UI.h"
#include <functional>
#include "Utils/Utils.h"

class ItemSelectionZone : public OnClickListener
{
private:

	std::function<void(itemEnum, int, int, int)> m_OnClickFunction;
	SDL_Rect m_RectZone;
	itemEnum m_Item;

public:
	ItemSelectionZone();
	ItemSelectionZone(int x, int y, int w, int h, itemEnum item);
	ItemSelectionZone(int x, int y, int w, int h);
	~ItemSelectionZone();

	void setZone(const SDL_Rect& zone);
	void setOnClickListener(std::function<void(itemEnum item, int mouseButtonStatus, int, int)> onClickFunction);
	virtual void onClick(int mouseButtonStatus, int x, int y);

	bool isOnZone(int x, int y) const;

	inline const SDL_Rect& getZone() const;
};


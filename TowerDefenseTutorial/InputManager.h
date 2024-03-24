#pragma once

#include <vector>
#include "SDL2/SDL.h"
#include "ItemSelectionZone.h"

class InputManager
{
private:
	SDL_Event m_Event;

	int m_MouseDownStatus;
	std::vector<ItemSelectionZone*> m_ItemSelectionZones;

public:
	InputManager();
	~InputManager();

	void handleEvents(SDL_Renderer* renderer, bool& running);
	void handleLeftMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void handleRightMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void handleMouseUnClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void addItemSelectionZone(ItemSelectionZone* zone);

};


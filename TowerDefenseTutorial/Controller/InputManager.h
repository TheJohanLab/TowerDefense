#pragma once

#include <vector>
#include "SDL2/SDL.h"
#include "ItemSelectionZone.h"
#include "../View/ItemPlacementPreview.h"

class InputManager
{
private:
	SDL_Event* m_Event;

	int m_MouseDownStatus;
	std::vector<ItemSelectionZone*> m_ItemSelectionZones;
	std::function<void(int, int)> m_OnMouseMoveCallback;
	std::function<void(int)> m_OnKeyPressedCallback;

public:
	InputManager();
	~InputManager();

	void handleEvents(SDL_Renderer* renderer, GameState& gameState);
	void handleLeftMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void handleRightMouseClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void handleMouseUnClick(int mouseButtonStatus, int mouseX, int mouseY) const;
	void handleMouseMotion(int mouseX, int mouseY) const;
	void handleKeyPressed(int key) const;

	void addItemSelectionZone(ItemSelectionZone* zone);
	void setMouseMovementCallback(std::function<void(int, int)> onMouseMoveCallback);
	void setKeyPressedCallback(std::function<void(int)> onKeyPressedCallback);
	
};


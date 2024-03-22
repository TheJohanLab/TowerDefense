#pragma once

#include "Listener.h"
#include "../View/UI.h"

class OnDestroyUnitListener : public Listener<>
{
public:
	void notify() override
	{
		UI* UI = UI::getInstance();
		UI->updateCoins(10);
	}
};
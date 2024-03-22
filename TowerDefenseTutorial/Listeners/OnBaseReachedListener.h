#pragma once

#include "Listener.h"
#include "../View/UI.h"

class OnBaseReachedListener : public Listener<int>
{
public:
	void notify(int damages) override
	{
		UI* UI = UI::getInstance();
		UI->updateHealth(damages);
	}
};
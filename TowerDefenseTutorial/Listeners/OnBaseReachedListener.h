#pragma once

#include "Observer.h"
#include "../View/UI.h"

class OnBaseReachedListener : public Observer<int>
{
public:
	void observe(int damages) override
	{
		UI* UI = UI::getInstance();
		UI->updateHealth(damages);
	}
};
#pragma once

#include "Observer.h"
#include "../View/UI.h"

class OnDestroyUnitListener : public Observer<int>
{
public:
	void observe(int amount) override
	{
		UI* UI = UI::getInstance();
		//UI->getShop()->addMoney(amount);
	}
};
#pragma once

#include "Observer.h"
#include "../View/UI.h"

class OnGameOverListener : public Observer<>
{
public:
	void observe() override
	{
		UI* UI = UI::getInstance();
	}
};
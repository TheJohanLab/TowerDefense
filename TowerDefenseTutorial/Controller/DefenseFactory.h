#pragma once

#include <memory>
#include <functional>
#include "SDL2/SDL.h"
#include "../Model/Defense.h"
#include "../Utils/Vector2D.h"

class DefenseFactory
{
private:


public:
	DefenseFactory() {}
	~DefenseFactory() {}

	template<typename T>
	static std::unique_ptr<Defense> createDefense(SDL_Renderer* renderer, Vector2D pos)
	{
		return std::make_unique<T>(renderer, pos);
	}
};


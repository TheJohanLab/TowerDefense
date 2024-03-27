#pragma once

#include <memory>
#include <functional>
#include "../Unit.h"
#include "../Vector2D.h"

class UnitFactory
{
private:

	static std::map<UnitType, std::function<std::shared_ptr<Unit>(SDL_Renderer* ,Vector2D )>> m_UnitMap;

	static std::shared_ptr<Unit> createMushroom(SDL_Renderer*, Vector2D);

public:
	UnitFactory();
	~UnitFactory();

	static std::shared_ptr<Unit> createUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type);
};


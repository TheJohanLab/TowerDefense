#pragma once

#include <memory>
#include <functional>
#include "../Model/Unit.h"
#include "../Utils/Vector2D.h"

class UnitFactory
{
private:

	static std::map<UnitType, std::function<std::shared_ptr<Unit>(SDL_Renderer* ,Vector2D, std::function<void(uint8_t)>)>> m_UnitMap;


public:
	UnitFactory();
	~UnitFactory();

	static std::shared_ptr<Unit> createUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type, std::function<void(uint8_t)> callback);

private:
	static std::shared_ptr<Unit> createMushroom(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> callback);
	static std::shared_ptr<Unit> createBat(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> callback);
	static std::shared_ptr<Unit> createSkeletton(SDL_Renderer* renderer, Vector2D pos, std::function<void(uint8_t)> callback);


};


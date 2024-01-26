#pragma once
#include <vector>
#include <memory>
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"
class Game;



class Unit
{
public:
	Unit(SDL_Renderer* renderer, Vector2D setPos);
	void update(float dT, Level& level, std::vector<std::shared_ptr<Unit>>& listUnits);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkOverlap(Vector2D posOther, float sizeOther);
	bool getIsAlive() const { return isAlive; }
	Vector2D getPos() const;

private:
	Vector2D pos;
	static const float speed;
	static const float size;

	SDL_Texture* texture = nullptr;

	bool isAlive = true;
};
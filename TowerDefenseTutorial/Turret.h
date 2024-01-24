#pragma once
#include "TextureLoader.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "MathAddon.h"

class Turret
{
public:
	Turret(SDL_Renderer* renderer, Vector2D setPos);
	
	void update(float dT);
	void draw(SDL_Renderer* renderer, int tileSize);
	bool checkIfOnTile(int x, int y) const;

private:
	Vector2D pos;
	float angle;
	static const float speedAngular;

	SDL_Texture
		* textureMain = nullptr,
		* textureShadow = nullptr;

	void drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected, int offset, int tileSize);
};


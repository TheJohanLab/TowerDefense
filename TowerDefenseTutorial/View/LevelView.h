#pragma once

#include <vector>

#include "SDL2/SDL.h"
#include "../Utils/Utils.h"

class LevelView
{

private:
	std::vector < std::vector<Tile>> m_LevelLayersTiles;
	std::vector<Tile> m_BackgroundTiles;
	std::vector<Tile> m_ObstructTiles;
	std::vector<Tile> m_SpawnerAndTargetTiles;

	SDL_Texture * m_BackGroundTileSetTextures = nullptr;
	SDL_Texture *m_ObstructTileSetTextures = nullptr;
	SDL_Texture *m_SpawnerTileSetTextures = nullptr;
	SDL_Texture *m_TargetTileSetTextures = nullptr;

	uint8_t m_TilesCountX;
	uint8_t m_TilesCountY;

public:
	LevelView(SDL_Renderer* renderer, uint8_t tileCountX, uint8_t tileCountY);
	~LevelView();

	void loadStaticTiles(const std::vector<std::vector<Tile>>& layers);
	void draw(SDL_Renderer* renderer) const;
	void drawObstructTiles(SDL_Renderer* renderer) const;

private:

	void drawTiles(SDL_Renderer* renderer, SDL_Texture* texture, uint8_t layer, 
		uint8_t tileSizeX, uint8_t tileSizeY, int offsetX = 0, int offsetY = 0) const;

};


#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include "SDL2/SDL.h"
#include "Unit.h"
#include "Level.h"
#include "Timer.h"
#include "Turret.h"
#include "Projectile.h"
#include "Listeners/OnDestroyUnitListener.h"
#include "InputManager.h"


class Game
{
private:
	enum class PlacementMode {
		wall,
		turret
	} placementModeCurrent;


public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight);
	~Game();


private:

	UI* m_UI = nullptr;
	InputManager* m_InputManager = nullptr;
	Shop* m_Shop = nullptr;

	void processEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY);
	void update(SDL_Renderer* renderer, float dT);
	void updateUnits(float dT);
	void updateProjectiles(float dT);
	void updateSpawnUnitsIfRequired(SDL_Renderer *renderer, float dT);
	void draw(SDL_Renderer* renderer);
	void addUnit(SDL_Renderer* renderer, Vector2D posMouse);
	void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
	bool removeTurretsAtMousePosition(Vector2D posMouse);

	int mouseDownStatus = 0;

	const int tileSize = 32;
	Level level;
	
	itemEnum* m_SelectedItem = nullptr;
	std::vector<std::shared_ptr<Unit>> listUnits;
	std::vector<Turret> listTurrets;
	std::vector<Projectile> listProjectiles;

	SDL_Texture* textureOverlay = nullptr;
	bool overlayVisible = true;

	Timer spawnTimer, roundTimer;
	int spawnUnitCount = 0;

};
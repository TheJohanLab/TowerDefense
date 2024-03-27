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
#include "Utils/Utils.h"
#include "Controller/GameLoop.h"
#include "Controller/LevelManager.h"
#include "Model/LevelData.h"

class Game
{
private:
	UI* m_UI = nullptr;
	InputManager* m_InputManager = nullptr;
	Shop* m_Shop = nullptr;
	ItemPlacementPreview* m_ItemPlacementPreview = nullptr;
	GameStatus* m_GameStatus = nullptr;
	GameLoop* m_GameLoop = nullptr;
	LevelManager* m_LevelManager = nullptr;

	Level m_Level;
	LevelData* m_LevelData = nullptr;

	int mouseDownStatus = 0;

	const int tileSize = 32;


	itemEnum* m_SelectedItem = nullptr;
	std::vector<std::shared_ptr<Unit>> m_ListUnits;
	std::vector<Turret> m_ListTurrets;
	std::vector<Projectile> m_ListProjectiles;

	SDL_Texture* textureOverlay = nullptr;
	bool overlayVisible = true;

	Timer m_WaveTimer { 0.0f };
	Timer m_SpawnTimer { SPAWN_TIMER_MS / 1000.0f };

	uint8_t m_WaveIndex = 0;
	uint8_t m_TotalWaves = 0;
	size_t m_SpawnUnitCount = 0;

public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight);
	~Game();

	void loadNextLevel();
	void clearLevel();
	void startLevel();

	void handleEvents(SDL_Renderer *renderer, GameState& gameState);
	void update(SDL_Renderer* renderer, float dT);
	void draw(SDL_Renderer* renderer);

private:
	void processEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY);
	
	void updateUnits(float dT);
	void updateProjectiles(float dT);
	void updateWaveTimer(SDL_Renderer* renderer, float dT);
	void handleSpawnUnits(SDL_Renderer* renderer, float dT);
	void spawnUnits(SDL_Renderer* renderer, std::vector<UnitCounter>& listEnemies, float dT);
	
	void addUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type);
	void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
	bool removeTurretsAtMousePosition(Vector2D posMouse);

	void handleWaves(SDL_Renderer* renderer, float dT);



};
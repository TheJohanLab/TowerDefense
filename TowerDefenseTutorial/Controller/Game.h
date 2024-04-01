#pragma once
#include <vector>
#include <chrono>
#include <memory>
#include "SDL2/SDL.h"
#include "InputManager.h"
#include "GameLoop.h"
#include "LevelLoaderManager.h"
#include "Level.h"
#include "PlayerManager.h"
#include "KeyboardManager.h"

#include "../Model/Unit.h"
#include "../Model/Timer.h"
#include "../Model/Defense.h"
#include "../Model/Projectile.h"
#include "../Model/LevelData.h"
#include "../Model/Shop.h"

#include "../View/LevelView.h"
#include "../View/GameStateDisplay.h"

#include "../Listeners/OnDestroyUnitListener.h"
#include "../Utils/Utils.h"

class Game
{
private:
	UI* m_UI = nullptr;
	InputManager* m_InputManager = nullptr;
	KeyboardManager* m_KeyBoardManager = nullptr;
	ItemPlacementPreview* m_ItemPlacementPreview = nullptr;
	GameStatus* m_GameStatus = nullptr;
	GameLoop* m_GameLoop = nullptr;
	LevelLoaderManager* m_LevelManager = nullptr;
	PlayerManager* m_PlayerManager = nullptr;
	LevelView* m_LevelView = nullptr;
	GameStateDisplay* m_GameStateDisplay = nullptr;
	Shop* m_Shop = nullptr;

	Level m_Level;
	WaveData m_WaveData;

	int mouseDownStatus = 0;

	const int tileSize = 32;


	itemEnum* m_SelectedItem = nullptr;
	std::vector<std::shared_ptr<Unit>> m_ListUnits;
	std::vector<std::unique_ptr<Defense>> m_ListDefenses;
	//std::vector<Turret> m_ListDefenses;
	std::vector<Projectile> m_ListProjectiles;

	SDL_Texture* textureOverlay = nullptr;
	bool overlayVisible = true;

	Timer m_AssaultTimer{ 0.0f };
	Timer m_SpawnTimer { SPAWN_TIMER_MS / 1000.0f };

	int m_AssaultIndex = 0;
	int m_TotalAssaults = 0;
	size_t m_SpawnUnitCount = 0;

public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight, int playingAreaWidth, int playingAreaHeight);
	~Game();

	void startLevel();
	void startWave();
	void loadNextWave();
	void clearLevel();
	void waitForNextWave();

	void handleEvents(SDL_Renderer *renderer, GameState& gameState);
	void update(SDL_Renderer* renderer, float dT);
	void draw(SDL_Renderer* renderer);

	bool isGameFinished() const;
	void drawVictoryScreen(SDL_Renderer* renderer) const;
	void gameOver(SDL_Renderer* renderer) const;

private:

	void initGame(SDL_Renderer* renderer, int windowWidth, int windowHeight, int playingAreaWidth, int playingAreaHeight);
	void processMouseEvents(SDL_Renderer* renderer, int mouseButtonStatus, int mouseX, int mouseY);
	void processKeyboardEvents(int key);
	
	void updateUnits(float dT);
	void updateDefenses(SDL_Renderer* renderer, float dT);
	void updateProjectiles(float dT);
	void updateAssaultTimer(SDL_Renderer* renderer, float dT);
	void updateGameStateDisplay(float dT);

	void handleSpawnUnits(SDL_Renderer* renderer, float dT);
	void spawnUnits(SDL_Renderer* renderer, std::vector<UnitCounter>& listEnemies, float dT);
	
	void addUnit(SDL_Renderer* renderer, Vector2D pos, UnitType type);
	template<typename T>
	void addDefense(SDL_Renderer* renderer, Vector2D posMouse);
	void addTurret(SDL_Renderer* renderer, Vector2D posMouse);
	bool removeTurretsAtMousePosition(Vector2D posMouse);

	void handleWave(SDL_Renderer* renderer, float dT);

	

};
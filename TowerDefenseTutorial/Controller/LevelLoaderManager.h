#pragma once

#include <vector>
#include <map>
#include "../Model/LevelData.h"

class LevelLoaderManager
{
private:

	std::map<std::string, UnitType> m_UnitTypeMap =
	{
		{"MUSHROOM", UnitType::MUSHROOM }
	};

	std::vector<std::vector<WaveData>> m_LevelDataList;
	uint8_t m_WaveIndex;
	uint8_t m_CurrentLevel;
	bool m_GameFinished;
	const char* filePath;

public:
	LevelLoaderManager(const char* filePath);
	~LevelLoaderManager();
	
	void loadLevelDataFromXML(const char* filePath);

	WaveData loadLevel(uint8_t LevelNumber);

	WaveData loadNextWave();

	bool isLevelFinished() const;

};


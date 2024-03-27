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

	std::vector<LevelData> m_LevelDataList;
	uint16_t m_LevelIndex;
	bool m_GameFinished;
	const char* filePath;

public:
	LevelLoaderManager(const char* filePath);
	~LevelLoaderManager();
	
	void loadLevelDataFromXML(const char* filePath);

	LevelData* loadNextLevel();

	bool isGameFinished() const;

};


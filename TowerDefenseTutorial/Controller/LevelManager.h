#pragma once

#include <vector>
#include "../Model/LevelData.h"

class LevelManager
{
private:
	std::vector<LevelData> m_LevelDataList;
	uint16_t m_LevelIndex;
	bool m_GameFinished;

public:
	LevelManager();
	~LevelManager();
	
	void loadLevelDataFromXML(const char* path);

	LevelData* loadNextLevel();

	bool isGameFinished() const;

};


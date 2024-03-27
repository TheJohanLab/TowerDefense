#include "LevelManager.h"
#include "../Unit.h"

LevelManager::LevelManager()
{
	loadLevelDataFromXML("");
}

LevelManager::~LevelManager()
{
}

void LevelManager::loadLevelDataFromXML(const char* path)
{
	LevelData level1 = LevelData(2, { UnitType::MUSHROOM }, { {{10, UnitType::MUSHROOM}}, {{20, UnitType::MUSHROOM}} }, 10);
	m_LevelDataList.emplace_back(level1);
}

LevelData* LevelManager::loadNextLevel()
{
	if (!m_GameFinished){
		m_GameFinished = m_LevelIndex == m_LevelDataList.size()-1;
		return &m_LevelDataList[m_LevelIndex++];
	}
}

bool LevelManager::isGameFinished() const
{
	return m_GameFinished;
}


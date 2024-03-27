#include "LevelManager.h"
#include "../Model/Unit.h"

LevelManager::LevelManager()
{
	loadLevelDataFromXML("");
}

LevelManager::~LevelManager()
{
}

void LevelManager::loadLevelDataFromXML(const char* path)
{
	LevelData level1 = LevelData(2, { UnitType::MUSHROOM }, 
		{ {{1, UnitType::MUSHROOM}}, {{0, UnitType::MUSHROOM}}, {{0, UnitType::MUSHROOM}} }, 15);
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


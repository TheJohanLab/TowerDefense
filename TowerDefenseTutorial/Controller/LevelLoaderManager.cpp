#include "LevelLoaderManager.h"
#include "../Model/Unit.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

LevelLoaderManager::LevelLoaderManager(const char* filePath)
{
	loadLevelDataFromXML(filePath);
}

LevelLoaderManager::~LevelLoaderManager()
{
}

void LevelLoaderManager::loadLevelDataFromXML(const char* filePath)
{

    rapidxml::file<> xmlFile(filePath);
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    rapidxml::xml_node<>* levelsNode = doc.first_node("levels");
    int levelIndex = 0;
    for (rapidxml::xml_node<>* levelNode = levelsNode->first_node(); levelNode; levelNode = levelNode->next_sibling())
    {
        m_LevelDataList.push_back(std::vector<WaveData>());

        for (rapidxml::xml_node<>* waveNode = levelNode->first_node(); waveNode; waveNode = waveNode->next_sibling()) 
        {
            WaveData wave;

            wave.totalAssaultsNb = std::stoi(waveNode->first_node("totalAssaultsNb")->value());

            rapidxml::xml_node<>* unitsNode = waveNode->first_node("units");
            for (rapidxml::xml_node<>* unitNode = unitsNode->first_node("unit"); unitNode; unitNode = unitNode->next_sibling("unit")) {
                UnitType unitType = m_UnitTypeMap.at(std::string(unitNode->value()));
                // Convertir le nom de l'unité en enum UnitType (implémentation spécifique à votre code)
                wave.listUnits.push_back(unitType);
            }

            rapidxml::xml_node<>* wavesNode = waveNode->first_node("assaults");
            for (rapidxml::xml_node<>* waveNode = wavesNode->first_node(); waveNode; waveNode = waveNode->next_sibling()) {
                std::vector<UnitCounter> unitCounters;
                rapidxml::xml_node<>* unitCountersNode = waveNode->first_node("unitCounters");
                for (rapidxml::xml_node<>* unitCounterNode = unitCountersNode->first_node(); unitCounterNode; unitCounterNode = unitCounterNode->next_sibling()) {
                    UnitCounter unitCounter;
                    unitCounter.unitType = m_UnitTypeMap.at(std::string(unitCounterNode->first_node("type")->value()));
                    unitCounter.count = std::stoi(unitCounterNode->first_node("count")->value());
                    unitCounters.push_back(unitCounter);
                }
                wave.unitsNbPerAssault.push_back(unitCounters);
            }

            rapidxml::xml_node<>* timerNode = waveNode->first_node("timerBetweenassaults");
            for (rapidxml::xml_node<>* waveNode = timerNode->first_node(); waveNode; waveNode = waveNode->next_sibling()) {
                wave.timerBeforeNextAssault.push_back(std::stoi(waveNode->value()));
            }

            m_LevelDataList[levelIndex].push_back(wave);

        }
        levelIndex++;
    }
}

WaveData LevelLoaderManager::loadLevel(uint8_t levelNumber)
{
    m_WaveIndex = 0;
    m_CurrentLevel = levelNumber;
    return loadNextWave();
}

WaveData LevelLoaderManager::loadNextWave()
{

	return m_LevelDataList[m_CurrentLevel][m_WaveIndex++];
	
}

bool LevelLoaderManager::isLevelFinished() const
{
	return  m_WaveIndex == m_LevelDataList[m_CurrentLevel].size();
}






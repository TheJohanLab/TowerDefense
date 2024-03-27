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
    for (rapidxml::xml_node<>* levelNode = levelsNode->first_node(); levelNode; levelNode = levelNode->next_sibling()) {
        LevelData level;

        // Charger totalWavesNb
        level.totalWavesNb = std::stoi(levelNode->first_node("totalWavesNb")->value());

        // Charger listUnits
        rapidxml::xml_node<>* unitsNode = levelNode->first_node("units");
        for (rapidxml::xml_node<>* unitNode = unitsNode->first_node("unit"); unitNode; unitNode = unitNode->next_sibling("unit")) {
            UnitType unitType = m_UnitTypeMap.at(std::string(unitNode->value()));
            // Convertir le nom de l'unité en enum UnitType (implémentation spécifique à votre code)
            level.listUnits.push_back(unitType);
        }

        // Charger unitsNbPerWave
        rapidxml::xml_node<>* wavesNode = levelNode->first_node("waves");
        for (rapidxml::xml_node<>* waveNode = wavesNode->first_node(); waveNode; waveNode = waveNode->next_sibling()) {
            std::vector<UnitCounter> unitCounters;
            rapidxml::xml_node<>* unitCountersNode = waveNode->first_node("unitCounters");
            for (rapidxml::xml_node<>* unitCounterNode = unitCountersNode->first_node(); unitCounterNode; unitCounterNode = unitCounterNode->next_sibling()) {
                UnitCounter unitCounter;
                unitCounter.unitType = m_UnitTypeMap.at(std::string(unitCounterNode->first_node("type")->value()));
                unitCounter.count = std::stoi(unitCounterNode->first_node("count")->value());
                unitCounters.push_back(unitCounter);
            }
            level.unitsNbPerWave.push_back(unitCounters);
        }

        // Charger timerBetweenWaves
        rapidxml::xml_node<>* timerNode = levelNode->first_node("timerBetweenWaves");
        for (rapidxml::xml_node<>* waveNode = timerNode->first_node(); waveNode; waveNode = waveNode->next_sibling()) {
            level.timerBeforeNextWave.push_back(std::stoi(waveNode->value()));
        }

        // Ajouter le niveau au vecteur de niveaux
        m_LevelDataList.push_back(level);
    }
}

LevelData* LevelLoaderManager::loadNextLevel()
{
	if (!m_GameFinished){
		m_GameFinished = m_LevelIndex == m_LevelDataList.size()-1;
		return &m_LevelDataList[m_LevelIndex++];
	}
}

bool LevelLoaderManager::isGameFinished() const
{
	return m_GameFinished;
}






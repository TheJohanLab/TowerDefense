#pragma once

#include <iostream>
#include <vector>
#include "Unit.h"

struct UnitCounter
{
	size_t count;
	UnitType unitType;
};

struct LevelData
{
	LevelData(){}
	LevelData(uint8_t totalWaves, std::vector<UnitType> units, std::vector<std::vector<UnitCounter>> unitsNb, std::vector<uint8_t> timerBeforeNextWave)
		:totalWavesNb(totalWaves), listUnits(units), unitsNbPerWave(unitsNb), timerBeforeNextWave(timerBeforeNextWave)
	{}


	uint8_t	totalWavesNb;
	std::vector<UnitType> listUnits;
	std::vector<std::vector<UnitCounter>> unitsNbPerWave;
	std::vector<uint8_t> timerBeforeNextWave;

};

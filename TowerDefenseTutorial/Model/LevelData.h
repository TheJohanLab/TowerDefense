#pragma once

#include <iostream>
#include <vector>
#include "../Unit.h"

struct UnitCounter
{
	size_t count;
	UnitType unitType;
};

struct LevelData
{
	LevelData(uint8_t totalWaves, std::vector<UnitType> units, std::vector<std::vector<UnitCounter>> unitsNb, uint8_t timer)
		:totalWavesNb(totalWaves), listUnits(units), unitsNbPerWave(unitsNb), timerBetweenWaves(timer)
	{}

	uint8_t	totalWavesNb;
	std::vector<UnitType> listUnits;
	std::vector<std::vector<UnitCounter>> unitsNbPerWave;
	uint8_t timerBetweenWaves;

};

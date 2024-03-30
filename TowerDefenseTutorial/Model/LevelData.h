#pragma once

#include <iostream>
#include <vector>
#include "Unit.h"

struct UnitCounter
{
	size_t count;
	UnitType unitType;
};

struct WaveData
{
	WaveData(){}
	WaveData(uint8_t totalAssaults, std::vector<UnitType> units, std::vector<std::vector<UnitCounter>> unitsNb, std::vector<uint8_t> timerBeforeNextAssault)
		:totalAssaultsNb(totalAssaults), listUnits(units), unitsNbPerAssault(unitsNb), timerBeforeNextAssault(timerBeforeNextAssault)
	{}


	uint8_t	totalAssaultsNb;
	std::vector<UnitType> listUnits;
	std::vector<std::vector<UnitCounter>> unitsNbPerAssault;
	std::vector<uint8_t> timerBeforeNextAssault;

};

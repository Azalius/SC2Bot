#pragma once
#include "botHeader.h"

std::queue<ABILITY_ID> fileAttBat;
std::map<Tag, ABILITY_ID> vcsKiBossent;
std::list<Tag> branleur;
std::list<Unit> faucheur;

using namespace sc2;
	void Bot::OnGameStart()  {
		BO();
	}
    void Bot::OnStep()  {
		for (Unit unite : Observation()->GetUnits(Unit::Alliance::Self)) {
			if (unite.is_selected) {
				std::cout << unite.assigned_harvesters << " " << unite.ideal_harvesters << std::endl;
			}
		}
		batBuildVcs();
		batBuildDo();
		macro();
		micro();
		if (Observation()->GetFoodCap() - Observation()->GetFoodUsed() <= 3 && Observation()->GetFoodUsed() >= 16) {
			Unit buildingPeon = trouveUnPeon(UNIT_TYPEID::TERRAN_SCV);
			Actions()->UnitCommand(buildingPeon, ABILITY_ID::BUILD_SUPPLYDEPOT, trouveOuConstruire(ABILITY_ID::BUILD_SUPPLYDEPOT, buildingPeon));
		}
	}
	 void Bot::OnUnitIdle(const Unit& unit) {
		switch (unit.unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_SCV: {
			if (vcsKiBossent.count(unit.tag) == 0)
				Actions()->UnitCommand(unit, ABILITY_ID::SMART, getNearest(unit.pos, UNIT_TYPEID::NEUTRAL_MINERALFIELD));
		}
		default: {
			break;
		}
		}
	}

	
	
	
	
	
	
	
	
	
	

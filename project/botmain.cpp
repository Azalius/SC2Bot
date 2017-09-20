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
			if (unite.is_selected && unite.orders.size() != 0) {
				std::cout << unite.pos.x << " " << unite.pos.y << std::endl;
			}
		}
		batBuildVcs();
		batBuildDo();
		macro();
		micro();
		reaper();
		peonGaz();
		if (Observation()->GetFoodCap() - Observation()->GetFoodUsed() <= 3 && Observation()->GetFoodUsed() >= 16) {
			Unit buildingPeon = trouveUnPeon(UNIT_TYPEID::TERRAN_SCV);
			Actions()->UnitCommand(buildingPeon, ABILITY_ID::BUILD_SUPPLYDEPOT, trouveOuConstruire(ABILITY_ID::BUILD_SUPPLYDEPOT, buildingPeon));
		}
	}
	 void Bot::OnUnitIdle(const Unit& unit) {
		switch (unit.unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_COMMANDCENTER: {
			if (allRaxBusy() || 1) {
				Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
			}
			break;
		}
		case UNIT_TYPEID::TERRAN_SCV: {
			Actions()->UnitCommand(unit, ABILITY_ID::SMART, getNearest(unit.pos, UNIT_TYPEID::NEUTRAL_MINERALFIELD));
		}
		default: {
			break;
		}
		}
	}

	
	
	
	
	
	
	
	
	
	

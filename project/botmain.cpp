#pragma once
#include "botHeader.h"

using namespace sc2;
	void Bot::OnGameStart()  {
		BO();
	}
    void Bot::OnStep()  {
		for (Unit unite : Observation()->GetUnits(Unit::Alliance::Self)) {
			if (unite.is_selected) {
				fleefrom(unite, getB1().pos);
			}
		}
		batBuildVcs();
		batBuildDo();
		macro();
		micro();
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

	
	
	
	
	
	
	
	
	
	

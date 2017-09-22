#pragma once
#include "botHeader.h"

using namespace sc2;

void Bot::BO() {
	fileAttBat.push(ABILITY_ID::BUILD_SUPPLYDEPOT);
	fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
	fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
	fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
	fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
	fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
	fileAttBat.push(ABILITY_ID::BUILD_FACTORY);
	fileAttBat.push(ABILITY_ID::BUILD_COMMANDCENTER);
	fileAttBat.push(ABILITY_ID::BUILD_STARPORT);
}
void Bot::macro() {
	macroEco();
	macroBat();
	reaper();
	peonGaz();
	macroSupply();
}
void Bot::macroEco() {
	if (makeMorePeon()) {
		for (Unit cc : getAll(UnitGroup::COMMANDCENTER)) { 
			if (cc.orders.size() == 0) {
				Actions()->UnitCommand(cc, ABILITY_ID::TRAIN_SCV);
			}
		}
	}
	for (Unit cc : getAll(UNIT_TYPEID::TERRAN_COMMANDCENTER)) {
		Actions()->UnitCommand(cc, ABILITY_ID::MORPH_ORBITALCOMMAND);     
	}
	for (Unit cc : getAll(UnitGroup::COMMANDCENTER)) {
		if (cc.ideal_harvesters - cc.assigned_harvesters > 0) {
			vaMiner(getPeonFromCc(cc));
		}
	}
}
void Bot::macroSupply() {//TODO
	/*if (Observation()->GetFoodCap() - Observation()->GetFoodUsed() >= 4) {
		Unit peon = trouveUnPeon(UNIT_TYPEID::TERRAN_SCV);
		Actions()->UnitCommand(peon, ABILITY_ID::BUILD_SUPPLYDEPOT, trouveOuConstruire(ABILITY_ID::BUILD_SUPPLYDEPOT, peon));
	}*/
}
void Bot::macroBat() {
	if (Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_FACTORY)).size() != 0 && Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_FACTORY)).front().build_progress == 1.0) {
		Unit facto = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_FACTORY)).front();
		if (facto.add_on_tag == 0) {
			Actions()->UnitCommand(facto, ABILITY_ID::BUILD_TECHLAB);
		}
		else {
			if (Observation()->GetUnit(facto.add_on_tag)->build_progress == 1.0) {
				Actions()->UnitCommand(facto, ABILITY_ID::LIFT);
			}
		}
	}
	if (Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORTFLYING)).size() != 0 && Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORTFLYING)).front().build_progress == 1.0) {
		Unit starpo = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORTFLYING)).front();
		if (Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_TECHLAB)).size() != 0 && Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_TECHLAB)).front().build_progress == 1.0) {
			Unit techLab = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_TECHLAB)).front();
			Actions()->UnitCommand(starpo, ABILITY_ID::LAND, techLab.pos + Point2D(-2.5, 0.5));
		}
	}
	if (Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).size() != 0 && Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front().build_progress == 1.0 &&  Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front().add_on_tag == 0) {
		Actions()->UnitCommand(Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front(), 3679);
	}
	if (Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).size() != 0 && Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front().build_progress == 1.0 &&  Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front().add_on_tag != 0) {
		Unit starpo = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_STARPORT)).front();
		if (starpo.orders.size() == 0) {
			Actions()->UnitCommand(starpo, ABILITY_ID::TRAIN_BANSHEE);
		}
		Actions()->UnitCommand(starpo.add_on_tag, ABILITY_ID::RESEARCH_BANSHEECLOAKINGFIELD);
	}
}

void Bot::peonGaz() {
	skipPourPeon--;
	Units toute = Observation()->GetUnits(Unit::Alliance::Self);
	for (Unit peon : toute) {
		if (peon.unit_type == UNIT_TYPEID::TERRAN_REFINERY && peon.build_progress == 1 && peon.ideal_harvesters - peon.assigned_harvesters > 0 && skipPourPeon < 0) {
			Actions()->UnitCommand(trouveUnPeon(UNIT_TYPEID::TERRAN_SCV), ABILITY_ID::HARVEST_GATHER, peon);
			skipPourPeon = 10;
		}

	}
}
void Bot::reaper() {
	for (Unit rax : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_BARRACKS))) {
		if (rax.orders.size() == 0) {
			Actions()->UnitCommand(rax, ABILITY_ID::TRAIN_REAPER);
		}
	}
}
bool Bot::allRaxBusy() { // not working
	Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_BARRACKS));
	for (Unit racks : toute) {
		if (racks.orders.size() == 0 && racks.build_progress == 1.0) {
			return false;
		}
	}
	return !CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS);
}
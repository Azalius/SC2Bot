#pragma once
#include "botHeader.h"

using namespace sc2;

Unit Bot::trouveUnPeon(UNIT_TYPEID peon = UNIT_TYPEID::TERRAN_SCV) {
	Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(peon));
	for (Unit peon : toute) {
		if (!isPeonBusy(peon.tag)) {
			return peon;
		}
	}
}
void Bot::addGaz(Unit peon) {
	Unit gazToAdd = getNearest(peon.pos, UNIT_TYPEID::NEUTRAL_VESPENEGEYSER);
	Actions()->UnitCommand(peon, ABILITY_ID::BUILD_REFINERY, gazToAdd);
}
void Bot::batBuildDo() {
	for (Tag bran : branleur) {
		vcsKiBossent.erase(bran);
	}
	branleur.clear();
	for (auto& vcs : vcsKiBossent) {
		if (Observation()->GetUnit(vcs.first) != 0 && Observation()->GetUnit(vcs.first)->orders.empty()) {
			if (vcs.second == ABILITY_ID::BUILD_REFINERY) {
				addGaz(getUnit(vcs.first));
			}
			else {
				Actions()->UnitCommand(vcs.first, vcs.second, Observation()->GetUnit(vcs.first)->pos);
			}
		}
		else if (Observation()->GetUnit(vcs.first) != 0 && Observation()->GetUnit(vcs.first)->orders.front().ability_id == vcs.second) {
			branleur.push_back(vcs.first);
			vcsWantBuild--;
		}
	}
}
void Bot::batBuildVcs() {
	if (!fileAttBat.empty() && vcsWantBuild == 0 && Observation()->GetMinerals() > nbMineralsBougerVcs(fileAttBat.front())) {
		vcsWantBuild++;
		Unit vcs = trouveUnPeon();
		Actions()->UnitCommand(vcs, ABILITY_ID::MOVE, trouveOuConstruire(fileAttBat.front(), vcs));
		vcsKiBossent[vcs.tag] = fileAttBat.front();
		fileAttBat.pop_front();
	}
}
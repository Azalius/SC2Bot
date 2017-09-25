#pragma once
#include "botHeader.h"

using namespace sc2;

/*All*/
void Bot::vaMiner(Unit peon) {
	for (Unit cc : getAll(UnitGroup::COMMANDCENTER)) {// TODO IMPROVE
		if (cc.ideal_harvesters - cc.assigned_harvesters < 0) {
			Actions()->UnitCommand(peon, ABILITY_ID::SMART, getNearest(cc.pos, UNIT_TYPEID::NEUTRAL_MINERALFIELD));
			return;
		}
	}
}
void Bot::micro() {
	microReaper();
	microBanshee();
}
int Bot::nbEnnemyNear(Unit unit, float distancemax) {
	int nbEnn = 0;
	for (Unit unite : Observation()->GetUnits(Unit::Alliance::Enemy)) {
		if (!(estBatiment(unite.unit_type) && Distance2D(unite.pos, unit.pos) < distancemax)) {
			nbEnn++;
		}
	}
	return nbEnn;
}
bool Bot::isBeingSurrounded(Unit unit) { //improve
	return !(nbEnnemyNear(unit, 1) < NBUNITTOBESURROUNDED);
}

/*Reaper*/
bool Bot::fautMine(UNIT_TYPEID cible) {
	if (cible == UNIT_TYPEID::ZERG_ZERGLING || cible == UNIT_TYPEID::ZERG_QUEEN || cible == UNIT_TYPEID::ZERG_ROACH || cible == UNIT_TYPEID::ZERG_DRONE) {
		return true;
	}
	return false;
}
bool Bot::estDevant(Unit unit) {
	Point2D ennemy = getNearestEnnemy(locatePack()).pos;
	int nbUnitPlusProche = 0;
	for (Unit reaper : faucheur) {
		if (reaper != unit) {
			if (Distance2D(reaper.pos, ennemy) < Distance2D(unit.pos, ennemy)) {
				nbUnitPlusProche++;
			}
		}
	}
	return nbUnitPlusProche < faucheur.size() / 2;
}
bool Bot::isWithPack(Unit unit) {
	for (Unit reaper : faucheur) {
		if (reaper == unit) {
			return true;
		}
	}
	return false;
}
Point2D Bot::locatePack() {
	Point2D posPack = Point2D(0, 0);
	for (Unit packfo : faucheur) {
		posPack += packfo.pos;
	}
	return posPack / faucheur.size();
}
void Bot::goWithPack(Unit reaper) {
	if (faucheur.size() == 0) {
		faucheur.push_back(reaper);
	}
	Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, locatePack());
	if (Distance2D(reaper.pos, locatePack()) < 5) {
		faucheur.push_back(reaper);
		faucheur.sort();
		faucheur.unique();
	}
}
void Bot::orderPack(ABILITY_ID todo, Point2D where) {
	for (Unit reaper : faucheur) {
		Actions()->UnitCommand(reaper, todo, where);
	}
}
void Bot::updatePack() {
	std::list<Unit> tmpfo;
	for (Unit reaper : faucheur) {
		const Unit *newReaper = Observation()->GetUnit(reaper.tag);
		if (newReaper != 0) {
			tmpfo.push_back(*newReaper);
		}
	}
	faucheur.clear();
	faucheur = tmpfo;
}
Unit Bot::nearestFromEnnemyBase() {
	float dmax = std::numeric_limits<float>::max();
	Unit lePlusPres;
	for (Unit reaper : getAll(UNIT_TYPEID::TERRAN_REAPER)) {
		if (Distance2D(reaper.pos, baseEnnemie()) < dmax) {
			lePlusPres = reaper;
		}
	}
	return lePlusPres;
}
void Bot::microReaper() {
	Point2D lesVcs = (baseEnnemie() + 2 * getNearest(baseEnnemie(), UNIT_TYPEID::NEUTRAL_MINERALFIELD).pos) / 3;
	Unit chef = nearestFromEnnemyBase();
	for (Unit reaper : getAll(UNIT_TYPEID::TERRAN_REAPER)) {
		if (!(isWithPack(reaper))) {
			goWithPack(reaper);
		}
	}
	for (Unit reaper : faucheur) {
		if (faucheur.size() <= REAPERPACKMIN){
			if (reaper.health < REAPERHPFUITE || isBeingSurrounded(reaper)) {
				Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, getB1().pos);
			}
			else {
				attack(reaper);
			}
		}
		else {
			if (estDevant(reaper) && reaper.health < REAPERHPFUITE) {
				Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, getB1().pos);
			}
			else if (attackBuilding(reaper)) {
				Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, lesVcs);
			}
			else {
				attack(reaper);
				if (fautMine(getNearestEnnemy(reaper.pos).unit_type)) {
					Actions()->UnitCommand(reaper, ABILITY_ID::EFFECT_KD8CHARGE, getNearestEnnemy(reaper.pos).pos);
				}
			}
		}
	}
	if (averageHealt(faucheur) < REAPERHPFUITE) {
		orderPack(ABILITY_ID::MOVE, getB1().pos);
	}
	updatePack();
}

/*Banshee*/
bool Bot::estMenace(Unit banshee) {
	return false;
}
void Bot::runFromDetection(Unit banshee) {

}
bool Bot::peutCloak(Unit banshee) {
	return banshee.energy > 10;
}
void Bot::pwnVcs(Unit banshee) {

}
void Bot::microBanshee() {
	for (Unit banshee : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_BANSHEE))) {
		if (banshee.cloak == Unit::CloakState::CloakedDetected && estMenace(banshee)) {
			runFromDetection(banshee);
		}
		else if (banshee.energy > BANSHEEENOUGHENERGY && peutCloak(banshee) && (banshee.cloak == Unit::CloakState::Cloaked || banshee.cloak == Unit::CloakState::CloakedDetected)) {
			pwnVcs(banshee);
		}
		else if (estMenace(banshee) && banshee.cloak == Unit::CloakState::NotCloaked && banshee.energy > 50) {
			Actions()->UnitCommand(banshee, ABILITY_ID::BEHAVIOR_CLOAKON_BANSHEE);
		}
	}
}
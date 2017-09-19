#pragma once
#include "botHeader.h"

using namespace sc2;
/*All*/

/*Reaper*/
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
	Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, locatePack());
	if (Distance2D(reaper.pos, locatePack()) < 5 && Distance2D(getB1().pos, locatePack())> 10) {
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

void Bot::micro() {
	Point2D lesVcs = (baseEnnemie() + 2 * getNearest(baseEnnemie(), UNIT_TYPEID::NEUTRAL_MINERALFIELD).pos) / 3;
	for (Unit reaper : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_REAPER))) {
		if (Distance2D(getB1().pos, reaper.pos) < 15 && faucheur.size() == 0) {
			attack(reaper);
		}
		else if (faucheur.size() == 0 && Distance2D(getB1().pos, reaper.pos)> 25) {
			faucheur.push_back(reaper);
		}
		else if (!isWithPack(reaper)) {
			goWithPack(reaper);
		}
	}
	if (faucheur.size() < PACK_MIN)
	{
		orderPack(ABILITY_ID::ATTACK, locatePack());;
	}
	else
	{
		orderPack(ABILITY_ID::ATTACK, lesVcs);
	}
	for (Unit reaper : faucheur) {
		Units ttlesreaper = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_REAPER));
		if (std::find(ttlesreaper.begin(), ttlesreaper.end(), reaper) == ttlesreaper.end()) {
			faucheur.remove(reaper);
			break;
		}
		if (fautMine(getNearestEnnemy(reaper.pos).unit_type)) {
			Actions()->UnitCommand(reaper, ABILITY_ID::EFFECT_KD8CHARGE, getNearestEnnemy(reaper.pos).tag);
		}
		if (reaper.health < 0.7* averageHealt() && estDevant(reaper) && faucheur.size() >= PACK_MIN - 1) {
			Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, getB1().pos);
		}
		else if (Distance2D(reaper.pos, getNearestEnnemy(reaper.pos).pos) > 4.5 && Distance2D(reaper.pos, getNearestEnnemy(reaper.pos).pos) < 5.5) {
			Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, lesVcs);
		}

	}
	if (faucheur.size() != 0 && averageHealt() / faucheur.front().health_max < 0.3) {
		orderPack(ABILITY_ID::MOVE, getB1().pos);
	}
	updatePack();
}
bool Bot::fautMine(UNIT_TYPEID cible) {
	if (cible == UNIT_TYPEID::ZERG_ZERGLING || cible == UNIT_TYPEID::ZERG_QUEEN || cible == UNIT_TYPEID::ZERG_ROACH || cible == UNIT_TYPEID::ZERG_DRONE) {
		return true;
	}
	return false;
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
		else if (banshee.energy > 20 && peutCloak(banshee) && (banshee.cloak == Unit::CloakState::Cloaked || banshee.cloak == Unit::CloakState::CloakedDetected)) {
			pwnVcs(banshee);
		}
		else if (estMenace(banshee) && banshee.cloak == Unit::CloakState::NotCloaked && banshee.energy > 50) {
			Actions()->UnitCommand(banshee, ABILITY_ID::BEHAVIOR_CLOAKON_BANSHEE);
		}
	}
}
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
void Bot::fleefrom(Unit enfuite, Point2D afuire) {//a verifier
	const float distance = 10.0;
	Point2D whereto;
	float coef = 1.0;
	float a, b; // ax+b equation
	a = (afuire.y - enfuite.pos.y) / (afuire.y - enfuite.pos.y);
	b = afuire.y - a * afuire.x;
	whereto.x = enfuite.pos.x + coef;
	whereto.y = enfuite.pos.y + coef * a;
	Actions()->UnitCommand(enfuite, ABILITY_ID::MOVE, whereto);
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

/*Banshee*/
bool Bot::estMenace(Unit banshee) { //TODO improve close to no energy, detector near...
	bool isVisible;
	bool isDanger;
	isVisible = banshee.cloak == Unit::CloakedDetected || banshee.cloak == Unit::NotCloaked;
	for (Unit ennemy : Observation()->GetUnits(Unit::Alliance::Enemy)) {
		isDanger = (isDanger = false && Distance2D(banshee.pos, ennemy.pos) < attackrange(ennemy) + 1 && AttackFlying(ennemy.unit_type));
	}
	return isVisible && isDanger;
}
void Bot::runFromDetection(Unit banshee) {
	fleefrom(banshee, getNearestEnnemy(banshee.pos).pos);
}
bool Bot::peutCloak(Unit banshee) {
	return banshee.energy > BANSHEEENOUGHENERGY;
}
void Bot::pwnVcs(Unit banshee) {
	Actions()->UnitCommand(banshee, ABILITY_ID::ATTACK_ATTACK, baseEnnemie());
}
void Bot::microBanshee() {
	for (Unit banshee : getAll(UNIT_TYPEID::TERRAN_BANSHEE)) {
		if (banshee.cloak == Unit::CloakState::CloakedDetected && estMenace(banshee)) {
			runFromDetection(banshee);
		}
		else if (peutCloak(banshee) && (banshee.cloak == Unit::CloakState::Cloaked || banshee.cloak == Unit::CloakState::CloakedDetected)) {
			pwnVcs(banshee);
		}
		else if (estMenace(banshee) && banshee.cloak == Unit::CloakState::NotCloaked && banshee.energy > 50) {
			Actions()->UnitCommand(banshee, ABILITY_ID::BEHAVIOR_CLOAKON_BANSHEE);
		}
	}
}
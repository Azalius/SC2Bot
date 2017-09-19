#pragma once
#include "botHeader.h"

using namespace sc2;

int Bot::CountUnitType(UNIT_TYPEID unit_type) {
	return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit_type)).size();
}
Point2D Bot::baseEnnemie() {
	const GameInfo& game_info = Observation()->GetGameInfo();
	return game_info.enemy_start_locations.front();
}
float Bot::averageHealt() { // TODO sur un array de unit
	float health = 0;
	for (Unit reaper : faucheur) {
		health += reaper.health;
	}
	return health / faucheur.size();
}
void Bot::attack(Unit unit) {
		//if (unit.weapon_cooldown == 0.0)
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK, baseEnnemie());
		//else { Actions()->UnitCommand(unit, ABILITY_ID::MOVE, getB1()); }
	}
Unit Bot::getB1() {
	for (Unit base : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_COMMANDCENTER))) {
		return base;
	}
}
bool Bot::estBatiment(UNIT_TYPEID bat) {
	if (bat == UNIT_TYPEID::ZERG_ROACHWARREN || bat == UNIT_TYPEID::ZERG_HATCHERY || bat == UNIT_TYPEID::ZERG_SPAWNINGPOOL) {
		return true;
	}
	if (bat == UNIT_TYPEID::ZERG_LARVA || bat == UNIT_TYPEID::ZERG_EGG || bat == UNIT_TYPEID::ZERG_SPAWNINGPOOL) {
		return true;
	}
	return false;
}
bool Bot::sharePosWithRef(Unit u) {
	Units allRef = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_REFINERY));
	for (Unit ref : allRef) {
		if (ref.pos == u.pos) {
			return true;
		}
	}
	return false;
}
Unit Bot::getNearest(Point2D unite, UNIT_TYPEID type) {
	Unit proche;
	Units units = Observation()->GetUnits();
	float distance = std::numeric_limits<float>::max();
	for (const auto& u : units) {
		if (u.unit_type == type) {
			float d = DistanceSquared2D(unite, u.pos);
			if (d < distance) {
				distance = d;
				proche = u;
			}
		}
	}
	return proche;
}
Unit Bot::getNearestEnnemy(Point2D unite) {
	Unit proche;
	Units units = Observation()->GetUnits();
	float distance = std::numeric_limits<float>::max();
	for (const auto& u : units) {
		if (u.alliance == Unit::Alliance::Enemy) {
			float d = DistanceSquared2D(unite, u.pos);
			if (d < distance) {
				distance = d;
				proche = u;
			}
		}
	}
	return proche;
}
float Bot::attackrange(Unit unit) {
	return 5;
}
bool Bot::isPeonBusy(Tag tag) {
	if (Observation()->GetUnit(tag)->orders.size() == 0) {
		return false;
	}
	std::string cible = Observation()->GetUnit(tag)->orders.front().ability_id.to_string();
	return vcsKiBossent.count(tag) == 0 && (cible != "3666" && cible != "3667");
}
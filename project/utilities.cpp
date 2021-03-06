#pragma once
#include "botHeader.h"

using namespace sc2;

int Bot::CountUnitType(UNIT_TYPEID unit_type) {
	int nbunit = 0;
	for (Unit unit : getAll(unit_type)) {
		if (unit.build_progress == 1.0) {
			nbunit++;
		}
	}
	return nbunit;
}
Point2D Bot::baseEnnemie() {
	const GameInfo& game_info = Observation()->GetGameInfo();
	return game_info.enemy_start_locations.front();
}
float Bot::averageHealt(const std::list<Unit> liste) {
	float health = 0;
	for (Unit reaper : liste) {
		health += reaper.health;
	}
	return health / liste.size();
}
void Bot::attack(Unit unit) { //TODO hit&run
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK, baseEnnemie());
	}
Unit Bot::getB1() {
	for (Unit base : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_COMMANDCENTER))) {
		return base;
	}
}
bool Bot::estBatiment(UNIT_TYPEID bat) {//IMPROVE
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
Unit Bot::getNearestEnnemy(Point2D unite, UNIT_TYPEID type) {
	Unit proche;
	Units units = Observation()->GetUnits();
	float distance = std::numeric_limits<float>::max();
	for (const auto& u : units) {
		if (u.alliance == Unit::Alliance::Enemy && u.unit_type == type) {
			float d = DistanceSquared2D(unite, u.pos);
			if (d < distance) {
				distance = d;
				proche = u;
			}
		}
	}
	return proche;
}
float Bot::attackrange(Unit unit) { // TODO
	return 5;
}
bool Bot::isPeonBusy(Tag tag) {
	if (vcsKiBossent.count(tag) != 0) {
		return true;
	}
	if (Observation()->GetUnit(tag)->orders.size() == 0) {
		return false;
	}
	std::string cible = Observation()->GetUnit(tag)->orders.front().ability_id.to_string();
	return vcsKiBossent.count(tag) == 0 && (cible != NOTIMPORTANTORDERS1 && cible != NOTIMPORTANTORDERS2);
}
Unit Bot::getUnit(Tag tag) {
	const Unit *cepeon;
	cepeon = Observation()->GetUnit(tag);
	return *cepeon;
}
Units Bot::getAll(UNIT_TYPEID unit) {
	return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit));
}
Units Bot::getAll(UnitGroup gr) {
	Units allUnits;
	if (gr == UnitGroup::COMMANDCENTER) {
		for (Unit cc : getAll(UNIT_TYPEID::TERRAN_COMMANDCENTER)) { 
			allUnits.push_back(cc);
		}
		for (Unit cc : getAll(UNIT_TYPEID::TERRAN_ORBITALCOMMAND)) {
			allUnits.push_back(cc);
		}
		for (Unit cc : getAll(UNIT_TYPEID::TERRAN_PLANETARYFORTRESS)) {
			allUnits.push_back(cc);
		}
	}
	return allUnits;
}
bool Bot::attackBuilding(Unit unit) { //TODO
	return false;
}
Unit Bot::getPeonFromCc(Unit cc) { // TODO
	Unit peon;
	float i = 0;
	do {
		peon = trouveUnPeon(UNIT_TYPEID::TERRAN_SCV);
		i += DISTANCEPEONCCREDUCTION;
	} while (Distance2D(cc.pos, peon.pos) > DISTANCEPEONCC - i);
	return peon;
}
bool Bot::AttackFlying(UNIT_TYPEID type) {
	return true;
}
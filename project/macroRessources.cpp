#pragma once
#include "botHeader.h"

using namespace sc2;


int Bot::isBuilding(UNIT_TYPEID unit = UNIT_TYPEID::TERRAN_BARRACKS) {
	int nbBat = 0;
	Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit));
	for (Unit bat : toute) {
		if (bat.build_progress != 1) {
			nbBat++;
		}
	}
	return nbBat;
}
int Bot::SupBuilding() {
	int nbBat = 0;
	for (Unit peon : getAll(UNIT_TYPEID::TERRAN_SCV)) {
		for (UnitOrder ordre : peon.orders) {
			if (ordre.ability_id == ABILITY_ID::BUILD_SUPPLYDEPOT) {
				nbBat++;
			}
		}
	}
	return nbBat + isBuilding(UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
}
Point2D Bot::ouPlacerCC(Unit minerai) {
	Point2D avrMiner = Point2D(0, 0);
	int nbminerals = 0;
	for (Unit mine : Observation()->GetUnits(Unit::Alliance::Neutral, IsUnit(UNIT_TYPEID::NEUTRAL_MINERALFIELD))) {
		if (Distance2D(mine.pos, minerai.pos) < 11) {
			avrMiner += mine.pos;
			nbminerals++;
		}
	}
	avrMiner /= nbminerals;
	float distance = std::numeric_limits<float>::max();
	Point2D laOuTuPeux;
	for (int i = avrMiner.x - LIMITEDISTACEBUILD ; i < avrMiner.x + LIMITEDISTACEBUILD; i++) {
		for (int j = avrMiner.y - LIMITEDISTACEBUILD ; j < avrMiner.y + LIMITEDISTACEBUILD; j++) {
			if (Query()->Placement(ABILITY_ID::BUILD_COMMANDCENTER, Point2D(i, j)) && Query()->PathingDistance(avrMiner, Point2D(i, j)) < distance) {
				laOuTuPeux = Point2D(i, j);
				distance = Query()->PathingDistance(avrMiner, Point2D(i, j));
			}
		}
	}
	return laOuTuPeux;
}
Point2D Bot::trouveOuConstruire(ABILITY_ID bat, Unit vcs) {
	if (bat == ABILITY_ID::BUILD_REFINERY) {
		Unit proche;
		Units units = Observation()->GetUnits();
		float distance = std::numeric_limits<float>::max();
		for (const auto& u : units) {
			if (u.unit_type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER && !sharePosWithRef(u)) {
				float d = DistanceSquared2D(vcs.pos, u.pos);
				if (d < distance) {
					distance = d;
					proche = u;
				}
			}
		}
		return proche.pos;
	}
	else if (bat == ABILITY_ID::BUILD_COMMANDCENTER) {
		Unit pluspres;
		float distance = std::numeric_limits<float>::max();
		for (Unit minerai : Observation()->GetUnits(Unit::Alliance::Neutral, IsUnit(UNIT_TYPEID::NEUTRAL_MINERALFIELD))) {
			if (Distance2D(getB1().pos, minerai.pos) > 10) {
				if (Distance2D(getB1().pos, minerai.pos) < distance) {
					pluspres = minerai;
					distance = Distance2D(getB1().pos, minerai.pos);
				}
			}
		}
		return ouPlacerCC(pluspres);
	}
	else {
		float seuil;
		bool ok = false;
		Point2D laQueJeConstrui;
		float rx, ry;
		int i, j;
		while (!ok) {
			rx = GetRandomScalar();
			ry = GetRandomScalar();
			ok = true;
			laQueJeConstrui = Point2D(vcs.pos.x + rx * 8.0f, vcs.pos.y + ry * 8.0f);
			seuil = Distance2D(getB1().pos, getNearest(laQueJeConstrui, UNIT_TYPEID::NEUTRAL_MINERALFIELD).pos);
			if (Distance2D(getB1().pos, laQueJeConstrui) < seuil && Distance2D(laQueJeConstrui, getNearest(laQueJeConstrui, UNIT_TYPEID::NEUTRAL_MINERALFIELD).pos) < seuil) {
				ok = false;
			}
			if (!Query()->Placement(bat, laQueJeConstrui)) {
				ok = false;
			}
			if (!Query()->Placement(ABILITY_ID::BUILD_SUPPLYDEPOT, laQueJeConstrui + Point2D(2.5, -0.5))) {
				ok = false;
			}
		}
		return laQueJeConstrui;
	}
}
int Bot::nbMineralsBougerVcs(ABILITY_ID bat) {
	if (bat == ABILITY_ID::BUILD_REFINERY) {
		return 50;
	}
	if (bat == ABILITY_ID::BUILD_BARRACKS) {
		return 110;
	}
	if (bat == ABILITY_ID::BUILD_SUPPLYDEPOT) {
		return 80;
	}
	return 100;
}
bool Bot::makeMorePeon() { //TODO
	return getAll(UNIT_TYPEID::TERRAN_BARRACKS).size() == 0 || getAll(UNIT_TYPEID::TERRAN_COMMANDCENTER).size() == 0 || CountUnitType(UNIT_TYPEID::TERRAN_SCV) <= 12;
}
void Bot::addBuilding(ABILITY_ID tobuild) {
	fileAttBat.push_front(tobuild);
}
int Bot::nbBatEnConstr(ABILITY_ID bat) {
	int nbBat = 0;
	if (fileAttBat.front() == bat) {
		nbBat++;
	}
	for (const auto& kv : vcsKiBossent) {
		if (kv.second == bat) {
			nbBat++;
		}
	}
	return nbBat;
}
void Bot::dropMule(Unit cc) { 
	int maxMinerals = INT_MIN;
	Unit mineToDrop;
	for (Unit mine : getAll(UNIT_TYPEID::NEUTRAL_MINERALFIELD)) {
		if (Distance2D(mine.pos, getNearest(mine.pos, UNIT_TYPEID::TERRAN_COMMANDCENTER).pos) < DISTANCEMAXMINERALSCC ){			//TODO for all types of cc
			if (mine.mineral_contents > maxMinerals) {
				maxMinerals = mine.mineral_contents;
				mineToDrop = mine;
			}
		}
	}
	Actions()->UnitCommand(cc, ABILITY_ID::EFFECT_CALLDOWNMULE, mineToDrop);
}
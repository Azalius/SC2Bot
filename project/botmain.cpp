#include "botHeader.h"

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
			addSup();
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
			/*TODO*/
		}
		default: {
			break;
		}
		}
	}

	
	
	
	
	
	
	void Bot::batBuildVcs() {
		if (!fileAttBat.empty() && vcsWantBuild == 0 && Observation()->GetMinerals() > nbMineralsBougerVcs(fileAttBat.front())) {
			vcsWantBuild++;
			Unit vcs = trouveUnPeon();
			Actions()->UnitCommand(vcs, ABILITY_ID::MOVE, trouveOuConstruire(fileAttBat.front(), vcs));
			vcsKiBossent[vcs.tag] = fileAttBat.front();
			fileAttBat.pop();
		}
	}
	void Bot::batBuildDo() {
		for (Tag bran : branleur) {
			vcsKiBossent.erase(bran);
		}
		branleur.clear();
		for (auto& vcs : vcsKiBossent) {
			if (Observation()->GetUnit(vcs.first) != 0 && Observation()->GetUnit(vcs.first)->orders.empty()) {
				if (vcs.second == ABILITY_ID::BUILD_REFINERY) {
					addGaz(vcs.first);
				}
				else {
					Actions()->UnitCommand(vcs.first, vcs.second, Observation()->GetUnit(vcs.first)->pos);
				}
			}
			else if (Observation()->GetUnit(vcs.first) != 0 && Observation()->GetUnit(vcs.first)->orders.front().ability_id == vcs.second) {
				branleur.push_front(vcs.first);
				vcsWantBuild--;
			}
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

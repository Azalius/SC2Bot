#include "botHeader.h"
#include <queue>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <algorithm>

using namespace sc2;

class Bot : public Agent {
public:
	std::queue<ABILITY_ID> fileAttBat;
	std::map<Tag, ABILITY_ID> vcsKiBossent;
	std::list<Tag> branleur;
	std::list<Unit> faucheur;
	virtual void OnGameStart() final {
		BO();
	}
	virtual void OnStep() final {
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
	virtual void OnUnitIdle(const Unit& unit) final {
		switch (unit.unit_type.ToType()) {
		case UNIT_TYPEID::TERRAN_COMMANDCENTER: {
			if (allRaxBusy() || 1) {
				Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
			}
			break;
		}
		case UNIT_TYPEID::TERRAN_SCV: {
			if (vcsKiBossent.count(unit) == 0) {
				uint64_t mineral_target;
				if (!FindNearest(unit.pos, mineral_target, UNIT_TYPEID::NEUTRAL_MINERALFIELD)) {
					break;
				}
				Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
			}
			break;
		}
		default: {
			break;
		}
		}
	}
private:
	int skipPourPeon = 0;
	int vcsWantBuild = 0;
	void BO() {
		fileAttBat.push(ABILITY_ID::BUILD_SUPPLYDEPOT);
		fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
		fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
		fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
		fileAttBat.push(ABILITY_ID::BUILD_BARRACKS);
		fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
		fileAttBat.push(ABILITY_ID::BUILD_FACTORY);
		fileAttBat.push(ABILITY_ID::BUILD_COMMANDCENTER);
		fileAttBat.push(ABILITY_ID::BUILD_STARPORT);
		fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
		fileAttBat.push(ABILITY_ID::BUILD_REFINERY);
	}
	bool allRaxBusy() {
		Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_BARRACKS));
		for (Unit racks : toute) {
			if (racks.orders.size() == 0 && racks.build_progress == 1.0) {
				return false;
			}
		}
		return !CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS);
	}
	void macro() {
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
	Unit trouveUnPeon(UNIT_TYPEID peon = UNIT_TYPEID::TERRAN_SCV) {
		Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(peon));
		for (Unit peon : toute) {
			if (!isPeonBusy(peon.tag)) {
				return peon;
			}
		}
	}
	void peonGaz() {
		skipPourPeon--;
		Units toute = Observation()->GetUnits(Unit::Alliance::Self);
		for (Unit peon : toute) {
			if (peon.unit_type == UNIT_TYPEID::TERRAN_REFINERY && peon.build_progress == 1 && peon.ideal_harvesters - peon.assigned_harvesters > 0 && skipPourPeon < 0) {
				Actions()->UnitCommand(trouveUnPeon(), ABILITY_ID::HARVEST_GATHER, peon);
				skipPourPeon = 10;
			}

		}
	}
	size_t CountUnitType(UNIT_TYPEID unit_type) {
		return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit_type)).size();
	}
	bool TryBuildStructure(ABILITY_ID BatABuild, UNIT_TYPEID unit_type = UNIT_TYPEID::TERRAN_SCV) {
		const ObservationInterface* observation = Observation();
		Unit unit_to_build = trouveUnPeon();
		float rx = GetRandomScalar();
		float ry = GetRandomScalar();
		Actions()->UnitCommand(unit_to_build,
			BatABuild,
			Point2D(unit_to_build.pos.x + rx * 15.0f, unit_to_build.pos.y + ry * 15.0f));
		return true;
	}
	bool addSup(bool force = 0) {
		if (force == 1 || SupBuilding() == 0) {
			return TryBuildStructure(ABILITY_ID::BUILD_SUPPLYDEPOT);
		}
	}
	bool addRax() {
		return TryBuildStructure(ABILITY_ID::BUILD_BARRACKS);
	}
	bool addGaz(Tag tag) {
		uint64_t gaz_target;
		if (!FindNearest(Observation()->GetUnit(tag)->pos, gaz_target, UNIT_TYPEID::NEUTRAL_VESPENEGEYSER)) {
			//std::cout << "no vespen geyser????" << std::endl;
		}
		Actions()->UnitCommand(Observation()->GetUnit(tag)->tag, ABILITY_ID::BUILD_REFINERY, gaz_target);
		return true;
	}
	int isBuilding(UNIT_TYPEID unit = UNIT_TYPEID::TERRAN_BARRACKS) {
		int nbBat = 0;
		Units toute = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit));
		for (Unit bat : toute) {
			if (bat.build_progress != 1) {
				nbBat++;
			}
		}
		return nbBat;
	}
	int SupBuilding() {
		int nbBat = 0;
		Units vcs = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_SCV));
		for (Unit peon : vcs) {
			for (UnitOrder ordre : peon.orders) {
				if (ordre.ability_id == ABILITY_ID::BUILD_SUPPLYDEPOT) {
					nbBat++;
				}
			}
		}
		return nbBat + isBuilding(UNIT_TYPEID::TERRAN_SUPPLYDEPOT);
	}
	bool FindNearest(const Point2D& start, uint64_t& target, UNIT_TYPEID type) {
		Units units = Observation()->GetUnits();
		float distance = std::numeric_limits<float>::max();
		for (const auto& u : units) {
			if (u.unit_type == type) {
				float d = DistanceSquared2D(u.pos, start);
				if (d < distance) {
					distance = d;
					target = u.tag;
				}
			}
		}

		if (distance == std::numeric_limits<float>::max()) {
			return false;
		}

		return true;
	}
	bool estDevant(Unit unit) {
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
	void reaper() {
		for (Unit rax : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_BARRACKS))) {
			if (rax.orders.size() == 0) {
				Actions()->UnitCommand(rax, ABILITY_ID::TRAIN_REAPER);
			}
		}
	}
	bool isWithPack(Unit unit) {
		for (Unit reaper : faucheur) {
			if (reaper == unit) {
				return true;
			}
		}
		return false;
	}
	Point2D locatePack() {
		Point2D posPack = Point2D(0, 0);
		for (Unit packfo : faucheur) {
			posPack += packfo.pos;
		}
		return posPack / faucheur.size();
	}
	void goWithPack(Unit reaper) {
		Actions()->UnitCommand(reaper, ABILITY_ID::MOVE, locatePack());
		if (Distance2D(reaper.pos, locatePack()) < 5 && Distance2D(getB1().pos, locatePack())> 10) {
			faucheur.push_back(reaper);
			faucheur.sort();
			faucheur.unique();
		}
	}
	void orderPack(ABILITY_ID todo, Point2D where) {
		for (Unit reaper : faucheur) {
			Actions()->UnitCommand(reaper, todo, where);
		}
	}
	void updatePack() {
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
	float averageHealt() {
		float health = 0;
		for (Unit reaper : faucheur) {
			health += reaper.health;
		}
		return health / faucheur.size();
	}
	Point2D baseEnnemie() {
		const GameInfo& game_info = Observation()->GetGameInfo();
		return game_info.enemy_start_locations.front();
	}
	void micro() {
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
	bool estMenace(Unit banshee) {

	}
	void runFromDetection(Unit banshee) {

	}
	bool peutCloak(Unit banshee) {
		return banshee.energy > 10;
	}
	void pwnVcs(Unit banshee) {

	}
	void microBanshee() {
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
	bool fautMine(UNIT_TYPEID cible) {
		if (cible == UNIT_TYPEID::ZERG_ZERGLING || cible == UNIT_TYPEID::ZERG_QUEEN || cible == UNIT_TYPEID::ZERG_ROACH || cible == UNIT_TYPEID::ZERG_DRONE) {
			return true;
		}
		return false;
	}
	void attack(Unit unit) {
		//if (unit.weapon_cooldown == 0.0)
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK, baseEnnemie());
		//else { Actions()->UnitCommand(unit, ABILITY_ID::MOVE, getB1()); }
	}
	Unit getB1() {
		for (Unit base : Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_COMMANDCENTER))) {
			return base;
		}
	}
	bool estBatiment(UNIT_TYPEID bat) {
		if (bat == UNIT_TYPEID::ZERG_ROACHWARREN || bat == UNIT_TYPEID::ZERG_HATCHERY || bat == UNIT_TYPEID::ZERG_SPAWNINGPOOL) {
			return true;
		}
		if (bat == UNIT_TYPEID::ZERG_LARVA || bat == UNIT_TYPEID::ZERG_EGG || bat == UNIT_TYPEID::ZERG_SPAWNINGPOOL) {
			return true;
		}
		return false;
	}
	bool sharePosWithRef(Unit u) {
		Units allRef = Observation()->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_REFINERY));
		for (Unit ref : allRef) {
			if (ref.pos == u.pos) {
				return true;
			}
		}
		return false;
	}
	Point2D ouPlacerCC(Unit minerai) {
		Point2D avrMiner = Point2D(0, 0);
		int nbminerals = 0;
		for (Unit mine : Observation()->GetUnits(Unit::Alliance::Neutral, IsUnit(UNIT_TYPEID::NEUTRAL_MINERALFIELD))) {
			if (Distance2D(mine.pos, minerai.pos) < 11) {
				avrMiner += mine.pos;
				nbminerals++;
			}
		}
		avrMiner /= nbminerals;
		int limite = 10;
		float distance = std::numeric_limits<float>::max();
		Point2D laOuTuPeux;
		for (int i = avrMiner.x - limite; i < avrMiner.x + limite; i++) {
			for (int j = avrMiner.y - limite; j < avrMiner.y + limite; j++) {
				if (Query()->Placement(ABILITY_ID::BUILD_COMMANDCENTER, Point2D(i, j)) && Query()->PathingDistance(avrMiner, Point2D(i, j)) < distance) {
					laOuTuPeux = Point2D(i, j);
					distance = Query()->PathingDistance(avrMiner, Point2D(i, j));
					std::cout << "f u " << i << " " << j << std::endl;
				}
			}
		}
		return laOuTuPeux;
	}
	Point2D trouveOuConstruire(ABILITY_ID bat, Unit vcs) {
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
			std::cout << "Tkt je vais faire ca" << std::endl;
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
	void batBuildVcs() {
		if (!fileAttBat.empty() && vcsWantBuild == 0 && Observation()->GetMinerals() > nbMineralsBougerVcs(fileAttBat.front())) {
			vcsWantBuild++;
			Unit vcs = trouveUnPeon();
			Actions()->UnitCommand(vcs, ABILITY_ID::MOVE, trouveOuConstruire(fileAttBat.front(), vcs));
			vcsKiBossent[vcs.tag] = fileAttBat.front();
			fileAttBat.pop();
		}
	}
	void batBuildDo() {
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
	int nbMineralsBougerVcs(ABILITY_ID bat) {
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
	Unit getNearest(Point2D unite, UNIT_TYPEID type) {
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
	Unit getNearestEnnemy(Point2D unite) {
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
	float attackrange(Unit unit) {
		return 5;
	}
	bool isPeonBusy(Tag tag) {
		if (Observation()->GetUnit(tag)->orders.size() == 0) {
			return false;
		}
		std::string cible = Observation()->GetUnit(tag)->orders.front().ability_id.to_string();
		return vcsKiBossent.count(tag) == 0 && (cible != "3666" && cible != "3667");
	}
};


int main(int argc, char* argv[]) {
	std::cout << "<coucou>" << std::endl;
	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);
	coordinator.SetRealtime(true);
	Bot bot;
	coordinator.SetParticipants({
		CreateParticipant(Race::Terran, &bot),
		CreateComputer(Race::Zerg,Hard)
	});
	coordinator.LaunchStarcraft();
	coordinator.StartGame(sc2::kMapBelShirVestigeLE);
	while (coordinator.Update()) {
	}
	return 0;
}
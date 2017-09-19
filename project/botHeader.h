#pragma once
#include <sc2api/sc2_api.h>
#include <iostream>
#include <queue>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <algorithm>

#define PACK_MIN 4

std::queue<ABILITY_ID> fileAttBat;
std::map<Tag, ABILITY_ID> vcsKiBossent;
std::list<Tag> branleur;
std::list<Unit> faucheur;

class Bot : public Agent {
public:
	virtual void OnGameStart() final;
	virtual void OnStep() final;
	virtual void OnUnitIdle(const Unit& unit) final;
private:
	
	int skipPourPeon = 0;
	int vcsWantBuild = 0;
	bool allRaxBusy();
	void BO();
	void macro();
	Unit trouveUnPeon(UNIT_TYPEID peon = UNIT_TYPEID::TERRAN_SCV);
	void peonGaz();
	int CountUnitType(UNIT_TYPEID unit_type);
	bool TryBuildStructure(ABILITY_ID BatABuild, UNIT_TYPEID unit_type = UNIT_TYPEID::TERRAN_SCV);
	int isBuilding(UNIT_TYPEID unit = UNIT_TYPEID::TERRAN_BARRACKS);
	int SupBuilding();
	bool estDevant(Unit unit);
	void reaper();
	bool isWithPack(Unit unit);
	Point2D locatePack();
	void goWithPack(Unit reaper);
	void orderPack(ABILITY_ID todo, Point2D where);
	void updatePack();
	float averageHealt();
	Point2D baseEnnemie();
	void micro();
	bool estMenace(Unit banshee);
	void runFromDetection(Unit banshee);
	bool peutCloak(Unit banshee);
	void pwnVcs(Unit banshee);
	void microBanshee();
	bool fautMine(UNIT_TYPEID cible);
	void attack(Unit unit);
	Unit getB1();
	bool estBatiment(UNIT_TYPEID bat);
	bool sharePosWithRef(Unit u);
	Point2D ouPlacerCC(Unit minerai);
	Point2D trouveOuConstruire(ABILITY_ID bat, Unit vcs);
	void batBuildVcs();
	void batBuildDo();
	int nbMineralsBougerVcs(ABILITY_ID bat);
	Unit getNearest(Point2D unite, UNIT_TYPEID type);
	Unit getNearestEnnemy(Point2D unite);
	float attackrange(Unit unit);
	bool isPeonBusy(Tag tag);
};




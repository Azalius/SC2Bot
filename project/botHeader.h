#pragma once

#include <sc2api/sc2_api.h>
#include <iostream>
#include <vector>
#include <queue>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <algorithm>

#define REAPERHPFUITE 25
#define BANSHEEENOUGHENERGY 20
#define NOTIMPORTANTORDERS1 "3666"
#define NOTIMPORTANTORDERS2 "3667"
#define LIMITEDISTACEBUILD 10
#define REAPERPACKMIN 3
#define NBUNITTOBESURROUNDED 4
#define DISTANCEPEONCC 5
#define FOODDIFFERENCETOADDSUP 3
#define DISTANCEPEONCCREDUCTION 0.05
#define DISTANCEMAXMINERALSCC 8
#define SECPERLOOP 1/24
#define NBSECDETECTIONNEEDED 180
enum UnitGroup { COMMANDCENTER, BATIMENT };

using namespace sc2;

class Bot : public Agent {
public:
	std::list<ABILITY_ID> fileAttBat;
	std::map<Tag, ABILITY_ID> vcsKiBossent;
	std::list<Tag> branleur;
	std::list<Unit> faucheur;
	virtual void OnGameStart() final;
	virtual void OnStep() final;
	virtual void OnUnitIdle(const Unit& unit) final;
private:
	void addGaz(Unit peon);
	int skipPourPeon = 0;
	int vcsWantBuild = 0;
	bool allRaxBusy();
	void BO();
	void macro();
	void macroEco();
	void macroSupply();
	Unit trouveUnPeon(UNIT_TYPEID peon);
	void macroBat();
	void peonGaz();
	int CountUnitType(UNIT_TYPEID unit_type);
	int isBuilding(UNIT_TYPEID unit);
	int SupBuilding();
	bool estDevant(Unit unit);
	void reaper();
	bool isWithPack(Unit unit);
	Point2D locatePack();
	void goWithPack(Unit reaper);
	void orderPack(ABILITY_ID todo, Point2D where);
	void updatePack();
	Unit nearestFromEnnemyBase();
	Point2D baseEnnemie();
	float averageHealt(const std::list<Unit> liste);
	void vaMiner(Unit peon);
	Unit getPeonFromCc(Unit cc);
	bool AttackFlying(UNIT_TYPEID type);
	void micro();
	void fleefrom(Unit enfuite, Point2D afuire);
	int nbEnnemyNear(Unit unit, float distance);
	bool isBeingSurrounded(Unit unit);
	bool estMenace(Unit banshee);
	void runFromDetection(Unit banshee);
	bool peutCloak(Unit banshee);
	void pwnVcs(Unit banshee);
	void microBanshee();
	void microReaper();
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
	bool makeMorePeon();
	void addBuilding(ABILITY_ID tobuild);
	int nbBatEnConstr(ABILITY_ID bat);
	void dropMule(Unit cc);
	Unit getNearest(Point2D unite, UNIT_TYPEID type);
	Unit getNearestEnnemy(Point2D unite);
	Unit getNearestEnnemy(Point2D unite, UNIT_TYPEID type);
	float attackrange(Unit unit);
	bool isPeonBusy(Tag tag);
	Unit getUnit(Tag tag);
	Units getAll(UNIT_TYPEID unit);
	Units getAll(UnitGroup gr);
	bool attackBuilding(Unit unit);
	bool detectionNeeded();
};



#include "botHeader.h"

using namespace sc2;

bool Bot::detectionNeeded() {//TODO
	bool dueToTime = Observation()->GetGameLoop() < SECPERLOOP * NBSECDETECTIONNEEDED;
	return dueToTime;
}
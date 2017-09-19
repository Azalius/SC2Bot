#pragma once
#include "botHeader.h"
//#include "botmain.cpp"
using namespace sc2;

#define TRUESPEED true

int main(int argc, char* argv[]) {
	std::cout << "Initialisation" << std::endl;
	Coordinator coordinator;
	coordinator.LoadSettings(argc, argv);
	coordinator.SetRealtime(TRUESPEED);
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
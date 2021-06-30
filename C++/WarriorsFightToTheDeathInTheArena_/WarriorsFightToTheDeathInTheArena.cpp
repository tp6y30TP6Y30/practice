#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "arena.h"
using namespace std;

void debug(){
#ifdef DEBUG_WARRIOR
	Warrior EvA = generateWarrior();
	EvA.detail();
#elif DEBUG_ARMY
	Army EvA_Offspring = createArmy(10);
	EvA_Offspring.detail();
#elif DEBUG_ARENA
	Army first_army = createArmy(10), second_army = createArmy(10);
	Arena arena = Arena(&first_army, &second_army);
	arena.detail();
#endif
}

int main(){
	srand(time(nullptr));
#ifdef DEBUG
	debug();
#endif
	Army first_army = createArmy(10000), second_army = createArmy(10000);
	Arena arena = Arena(&first_army, &second_army);
	arena.detail();
	while(arena.fightable()){
		arena.fight();
		arena.detail();
	}
	arena.honorTheSurvivors();
}
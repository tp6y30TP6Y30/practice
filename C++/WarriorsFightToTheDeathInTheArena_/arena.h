#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include "army.h"
using namespace std;

class Arena{
	public:
		Arena(Army* a = nullptr, Army* b = nullptr);
		void detail();
		bool fightable();
		void fight();
		void honorTheSurvivors();
	private:
		Army* army1_ptr;
		Army* army2_ptr;
		int fight_count;
};

Arena::Arena(Army* a, Army* b){
	army1_ptr = a;
	army2_ptr = b;
	fight_count = 0;
}

void Arena::detail(){
	printf("After %d round(s) of fight:\n", fight_count);
	cout << "====================\n";
	if(army1_ptr != nullptr and army1_ptr -> size() > 0){
		cout << "Army1" << endl;
		printf("length: %d\n", army1_ptr -> size());
		printf("total HP: %5d\ntotal attack: %5d\ntotal defense: %5d\n", 
			army1_ptr -> total_HP, army1_ptr -> total_attack, army1_ptr -> total_defense);
	}
	else{
		cout << "None" << endl;
	}
	cout << endl;
	if(army2_ptr != nullptr and army2_ptr -> size() > 0){
		cout << "Army2" << endl;
		printf("length: %d\n", army2_ptr -> size());
		printf("total HP: %5d\ntotal attack: %5d\ntotal defense: %5d\n", 
			army2_ptr -> total_HP, army2_ptr -> total_attack, army2_ptr -> total_defense);
	}
	else{
		cout << "None" << endl;
	}
	cout << "Fightable: " << boolalpha << fightable() << endl;
	cout << "====================\n";
	cout << endl;
}

bool Arena::fightable(){
	return army1_ptr -> size() > 0 and army2_ptr -> size() > 0;
}

void Arena::fight(){
	fight_count++;
	assert(fightable());
	auto fightCalculator = [](Warrior& a, Warrior& b){
		a.HP -= int(b.attack * (float(b.attack) / (b.attack + a.defense)));
		b.HP -= int(a.attack * (float(a.attack) / (a.attack + b.defense)));
	};
	int length = min(army1_ptr -> size(), army2_ptr -> size());
	for(int i = 0; i < length; ++i){
		fightCalculator(army1_ptr -> armyList[i], army2_ptr -> armyList[i]);
	}
	army1_ptr -> cleanUp();
	army2_ptr -> cleanUp();
	army1_ptr -> inventory();
	army2_ptr -> inventory();
}

void Arena::honorTheSurvivors(){
	assert(!fightable());
	Army survivors = army1_ptr -> size() > 0 ? *army1_ptr : *army2_ptr;
	survivors.detail();
}
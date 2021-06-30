#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "warrior.h"

class Army{
	public:
		Army(vector <Warrior> warriors);
		void sort();
		void detail();
		int size();
		void inventory();
		void cleanUp();
		friend class Arena;
	private:
		vector <Warrior> armyList;
		int total_HP;
		int total_attack;
		int total_defense;
};

Army createArmy(int amount){
	vector <Warrior> warriors;
	for(int i = 0; i < amount; ++i){
		warriors.push_back(generateWarrior());
	}
	Army army(warriors);
	army.sort();
	army.inventory();
	return army;
}

Army::Army(vector <Warrior> warriors){
	armyList = warriors;
}

void Army::sort(){
	std::sort(armyList.begin(), armyList.end(), [](Warrior a, Warrior b){
		return a.ID < b.ID;
	});
}

void Army::detail(){
	printf("===== Army with %2lu Warriors =====\n", armyList.size());
	printf("total_HP: %d\ntotal_attack: %d\ntotal_defense: %d\n\n", total_HP, total_attack, total_defense);
	for(auto w: armyList){
		w.detail();
		cout << endl;
	}
	cout << "========= Army List End =========" << endl << endl;
}

int Army::size(){
	return armyList.size();
}

void Army::inventory(){
	int HP = 0, attack = 0, defense = 0;
	for_each(armyList.begin(), armyList.end(), [&](Warrior w){
		HP += w.HP;
		attack += w.attack;
		defense += w.defense;
	});
	total_HP = HP, total_attack = attack, total_defense = defense;
}

void Army::cleanUp(){
	auto iter = armyList.begin();
	while(iter != armyList.end()){
		if(iter -> HP <= 0){
			iter = armyList.erase(iter);
		}
		else{
			++iter;
		}
	}
}
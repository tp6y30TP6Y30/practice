#include <iostream>
#include <vector>
#include <ctime>
#include <string>
using namespace std;

#define HP_MIN 100
#define HP_MAX 1000
#define ATTACK_MIN 50
#define ATTACK_MAX 500
#define DEFENSE_MIN 25
#define DEFENSE_MAX 250

class Warrior{
	public:
		Warrior(string id = nullptr, int health = 0, int att = 0, int def = 0);
		void detail();
		friend class Army;
		friend class Arena;
	private:
		string ID;
		int HP;
		int attack;
		int defense;
};

Warrior generateWarrior(){
	auto generateData = [](int value_min, int value_max){
		return value_min + rand() % (value_max - value_min + 1);
	};
	string id = to_string(rand() & int(1e7-1));
	int health = generateData(HP_MIN, HP_MAX);
	int att = generateData(ATTACK_MIN, ATTACK_MAX);
	int def = generateData(DEFENSE_MIN, DEFENSE_MAX);
	Warrior warrior(id, health, att, def);
	return warrior;
}

Warrior::Warrior(string id, int health, int att, int def){
	ID = id;
	HP = health;
	attack = att;
	defense = def;
};

void Warrior::detail(){
	cout << "ID: " << ID << endl;
	cout << "HP: " << HP << endl;
	cout << "Attack: " << attack << endl;
	cout << "Defense: " << defense << endl;
};
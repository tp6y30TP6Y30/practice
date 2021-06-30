#include <iostream>
#include <string>
using namespace std;

class Animal{
	protected:
		// data in protected scope can be kept by an inheritance object
		string s = "test";
	private:
		// data in private scope can NOT be kept by an inheritance object
		string name;
		double height;
		double weight;
		static int numOfAnimals;
	public:
		string getName();
		void setName(string);
		double getHeight();
		void setHeight(double);
		double getWeight();	
		void setWeight(double);
		void toString();
		static int getNumOfAnimals();
		void setAll(string, double, double);
		Animal();
		Animal(string, double, double);
		~Animal();
};

int Animal::numOfAnimals = 0;

Animal::Animal(){
	this -> name = "";
	this -> height = 0;
	this -> weight = 0;
	Animal::numOfAnimals++;
}

Animal::Animal(string name, double height, double weight){
	this -> name = name;
	this -> height = height;
	this -> weight = weight;
	Animal::numOfAnimals++;
}

void Animal::setAll(string name, double height, double weight){
	this -> name = name;
	this -> height = height;
	this -> weight = weight;
}

Animal::~Animal(){
	cout << this -> getName() << " is destroyed." << endl;
	Animal::numOfAnimals--;
}

string Animal::getName(){
	return this -> name;
}

void Animal::setName(string name){
	this -> name = name;
}

double Animal::getHeight(){
	return this -> height;
}

void Animal::setHeight(double height){
	this -> height = height;
}

double Animal::getWeight(){
	return this -> weight;
}

void Animal::setWeight(double weight){
	this -> weight = weight;
}

int Animal::getNumOfAnimals(){
	return Animal::numOfAnimals;
}

void Animal::toString(){
	cout << "Animal " << this -> getName() <<
			" is " << this -> getHeight() <<
			" centimeters tall and " <<
			this -> getWeight() << " kilograms in weight\n";
}

/*

public, protected and private inheritance in C++
public, protected, and private inheritance have the following features:
1. public inheritance makes public members of the base class public in the derived class, and the protected members of the base class remain protected in the derived class.
pulbic: (public, protected) -> (public, protected)
2. protected inheritance makes the public and protected members of the base class protected in the derived class.
protected: (public, protected) -> (protected)
3. private inheritance makes the public and protected members of the base class private in the derived class.
private: (public, protected) -> (private)
Note: private members of the base class are inaccessible to the derived class.

*/

class Dog: public Animal{ 	/* public is needed, denotes that we inheritate Animal to Dog's public scope 
							   if we use private or protected instead of public, than we can't use 
							   dog.setName(string) because it will set setName to private scope */
	private:
		string sound = "Woof";
	public:
		string makeSound();
		void setSound(string);
		Dog(): Animal(){};
		Dog(string, double, double, string);
		void toString();
		void setAll(string, double, double, string);
};

string Dog::makeSound(){
	return this -> sound;
}

void Dog::setSound(string sound){
	this -> sound = sound;
}

Dog::Dog(string name, double height, double weight, string sound){
	// private data in Animal still needs to use Animal's member function to access
	cout << this -> s << endl;
	this -> setName(name);
	this -> setHeight(height);
	this -> setWeight(weight);
	this -> sound = sound;
}

void Dog::setAll(string name, double height, double weight, string sound){
	this -> setName(name);
	this -> setHeight(height);
	this -> setWeight(weight);
	this -> setSound(sound);
}

void Dog::toString(){
	cout << "Animal " << this -> getName() <<
			" is " << this -> getHeight() <<
			" centimeters tall and " <<
			this -> getWeight() << " kilograms in weight and says " <<
			this -> makeSound() << endl;
}

int main(){
	Animal tom;
	tom.toString();
	Animal april("April", 165, 52);
	tom.setAll("Tom", 100, 100);
	april.toString();
	cout << Animal::getNumOfAnimals() << endl;
	Dog jack;
	jack.toString();
	jack.setAll("Test", 111, 111, "Errrr");
	cout << Animal::getNumOfAnimals() << endl; /* Animal::numOfAnimals still increments when we use Dog constructor rather than Animal constructor */
	jack.toString();
	jack.setAll("Jack", 30, 15, "Woooooof");
	jack.toString();
	Dog Laura;
	cout << Animal::getNumOfAnimals() << endl;
	Dog Estherx("Kittie", 164, 50, "Meow");
	cout << Animal::getNumOfAnimals() << endl;
	jack.setName("Jack2");
}
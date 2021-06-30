#include <iostream>
#include <cmath>
#define PI 3.14159
using namespace std;

typedef struct Shape{
	double height, width;
	Shape(double h = 1, double w = 1){
		this -> height = h;
		this -> width = w;
	}
	double area(){
		return height * width;
	}
} shape;

typedef struct Circle: shape{
	Circle(double w){
		this -> width = w;
	}
	double area(){
		return PI * pow(width / 2, 2);
	}
} circle;


int main(){
	shape rectangle(4.0, 5.0);
	cout << rectangle.area() << endl;
	circle round(4.5);
	cout << round.area() << endl;
	cout << round.height << endl;
}
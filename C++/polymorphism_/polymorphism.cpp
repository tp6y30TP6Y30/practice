/* 
Polymorphisom is just a feature in object-oriented programming languages
that allows objects to be similarly, and at the same time they also be 
able to utilize their differences automatically for any methods that been 
marked as virtual.
*/
#include <iostream>
#include <cmath>
#include <string>
#define PI 3.14159
using namespace std;

#ifdef POLYMORPHISOM
class Shape{
	protected:
		double height, width;
	public:
		Shape(){
			this -> height = this -> width = 0.;
		}
		Shape(double side){
			this -> height = this -> width = side;
		}
		Shape(double height, double width){
			this -> height = height;
			this -> width = width;
		}
		virtual double area(){
			return this -> height * this -> width;
		}
};

class Circle: public Shape{
	public:
		Circle(double side) : Shape(side){}
		double area(){
			return PI * pow(this -> width / 2, 2);
		}
};

void printArea(Shape& object){
	printf("Area: %s\n", to_string(object.area()).c_str());
}

int main(){
	Shape rectangle(4.0, 5.0);
	Circle circle(5.0);
	printArea(rectangle);
	printArea(circle);
}

#elif ABSTRACT_CLASS
class Shape{
	public:
		virtual double area() = 0; /* we need this assignment 0 */
};

class Circle: public Shape{
	protected:
		double width;	
	public:
		Circle(double side){
			this -> width = side;
		}
		double area() override{	/* always use override to force compiler to check
								   whether the base function(Shape's area) is 
								   exactly the same with the derived function
								   (Circle's area) */
			return PI * pow(this -> width / 2, 2);
		}
};

class Rectangle: public Shape{
	protected:
		double height, width;
	public:
		Rectangle(double h, double w){
			this -> height = h;
			this -> width = w;
		}
		double area() override /* final */{ /* 
											  final means that the sub class inheritant 
											  from this class can not keep override this
											  virtual function anymore or will get a 
											  runtime error like: 
											  error: declaration of 'area' overrides a 'final'
										  	*/
			return this -> height * this -> width;
		}
};

class Square: public Rectangle{
	public:
		Square(double h, double w) : Rectangle(h, w){}
		double area() override{
			return this -> height * this -> width;
		}
};

void printArea(Shape& object){
	printf("Area: %s\n", to_string(object.area()).c_str());
}

int main(){
	Circle circle(5.0);
	Rectangle rectangle(4.0, 5.0);
	Square square(4.0, 3.0);
	printArea(circle);
	printArea(rectangle);
	printArea(square);
}
#endif
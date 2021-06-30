#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <numeric>
#include <cmath>
#include <sstream>
 
#include <deque>
#include <iterator>
  
// Anything that starts with a # is a preprocessor
// directive and they run before the program compiles
  
// This is a macro constant that will replace 
// PI with 3.14159 in the code before execution
#define PI 3.14159
 
// This is a macro function that will do the same with
// a function
#define AREA_CIRCLE(radius) (PI * (std::pow(radius, 2)))
   
// We use templates to create functions or classes
// that can work with many types
// Templates differ from function overloading in that
// instead of having a function that does similar
// things with different objects a template does the
// same thing with different objects
 
// This says this is a function template that generates
// functions that except 1 parameter
template <typename T>
void Times2(T val){
    std::cout << val << " * 2 = " <<
            val * 2 << "\n";
}
 
// Receive multiple parameters and return a value
template <typename T>
T Add(T val, T val2){
    return val + val2;
}
 
// Work with chars and strings
template <typename T>
T Max(T val, T val2){
    return (val < val2) ? val2 : val;
}
   
// Template classes are classes that can work with 
// different data types
 
// You can define that you may receive parameters
// with different types, but they don't have to
// be different
template <typename T, typename U>
class Person{  
public:
    T height;
    U weight;
    static int numOfPeople;
    Person(T h, U w){
        height = h, weight = w;
        numOfPeople++;
    }   
    void GetData(){
        std::cout << "Height : " <<
            height << 
            " and Weight : " <<
            weight << "\n";
    }
};
 
// You have to initialize static class members
template<typename T, typename U> int Person<T, U>::numOfPeople;
 
int main()
{   
    std::cout << "Circle Area : " <<
            AREA_CIRCLE(5) << "\n";
        // The template function can receive ints or floats
    Times2(5);
    Times2(5.3);
    
    // Multiple parameters and returned value
    std::cout << "5 + 4 = " <<
            Add(5,4) << "\n";
    std::cout << "5.5 + 4.6 = " <<
            Add(5.5,4.6) << "\n";
 
    // Get biggest value
    std::cout << "Max 4 or 8 = " <<
            Max(4, 8) << "\n";
    std::cout << "Max A or B = " <<
            Max('A', 'B') << "\n";
    std::cout << "Max Dog or Cat = " <<
            Max("Dog", "Cat") << "\n";
    
    // When creating the object you must define the 
    // data types used
    Person<double, int> mikeTyson (5.83, 216);
    mikeTyson.GetData();
    
    // You access static values using the object
    // and not the class
    std::cout << "Number of people : " <<
            mikeTyson.numOfPeople << "\n";
    
}
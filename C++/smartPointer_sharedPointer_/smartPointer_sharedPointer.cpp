#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <numeric>
#include <cmath>
#include <sstream>
#include <iterator>
#include <memory>
#include <stdio.h>

#ifdef SHARED_POINTER

class Foo
{
    public:
        void doSomething();
};
 
class Bar
{
private:
    std::shared_ptr<Foo> pFoo;
public:
    Bar()
    {
        pFoo = std::shared_ptr<Foo>(new Foo());
    }
 
    std::shared_ptr<Foo> getFoo()
    {
        return pFoo;
    }
};

void SomeAction()
{
    Bar* pBar = new Bar(); //with the Bar object, a new Foo is created and stored
    //reference counter = 1
 
    std::shared_ptr<Foo> pFoo = pBar->getFoo(); //a copy of the shared pointer is created
    //reference counter = 2
 
    pFoo->doSomething(); 
 
    delete pBar; //with pBar the private pFoo is destroyed
    //reference counter = 1
 
    return; //with the return the local pFoo is destroyed automatically 
    //reference counter = 0
    //internally the std::shared_ptr destroys the reference to the Foo object
}

void SomeOtherAction(std::shared_ptr<Bar> pBar)
{
    std::shared_ptr<Foo> pFoo = pBar->getFoo(); //a copy of the shared pointer is created
    //reference counter = 2
 
    pFoo->doSomething(); 
 
    return; //local pFoo is destroyed
    //reference counter = 1
}

// A Smart pointer is a class that provides the 
// power of pointers, but also handles the reallocation
// of memory when it is no longer required (The pointer
// is automatically destroyed)

#endif
  
int main()
{
    // MALLOC EXAMPLE
    // When you define a primitive type like int or
    // float you define exactly the amount of space
    // to set aside
    
    // If you need to define how much space to set aside
    // you could call malloc() and tell it how much
    // space to set aside and it returns the address to
    // that memory address
    
    int amtToStore;
    std::cout << "How many numbers do you want to store : ";
    std::cin >> amtToStore;
    
    // Create an int pointer and set aside enough space
    int * pNums;
    
    // Cast the pointer and define how much space to set aside
    pNums = (int *) malloc(amtToStore * sizeof(int));
    
    // Check if memory was allocated
    if(pNums != NULL){
        int i = 0;
        
        // Store values
        while(i < amtToStore){
            std::cout << "Enter a Number : ";
            std::cin >> pNums[i];
            i++;
        }
    }
    
    std::cout << "You entered these numbers\n";
    for(int i = 0; i < amtToStore; i++){
        std::cout << pNums[i] << "\n";
    }
    
    // Delete the pointer
    // C: use free with malloc
    free(pNums);
    // C++: use delete with new
    // delete pNums;
    
    // Smart Pointer Solution
    int amtToStore_;
    std::cout << "How many numbers do you want to store : ";
    std::cin >> amtToStore_;
    
    // This memory will be automatically reallocated
    std::unique_ptr<int[]> pNums_(new int[amtToStore_]);
    
    // unique_ptr can only have one owner
    // so this throws an error
    // std::unique_ptr<int[]> pNums_2 = pNums_;
    // I'll cover how to do this with shared_ptr
    // in a later tutorial
    
    if(pNums_ != NULL){
        int i = 0;
        
        // Store values
        while(i < amtToStore_){
            std::cout << "Enter a Number : ";
            std::cin >> pNums_[i];
            i++;
        }
    }
    
    std::cout << "You entered these numbers\n";
    for(int i = 0; i < amtToStore_; i++){
        std::cout << pNums_[i] << "\n";
    }
    
    return 0;
}
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
  
int main()
{        
    // A double ended queue (Deck) is a dynamic array that can 
    // be expanded or contracted on both ends
    std::deque<int> nums = {1,2,3,4};
    nums.push_front(0);
    nums.push_back(5);
    for(int x: nums)
        std::cout << x << "\n";
    
    // You can access index values, but they are costly
    // because values aren't stored contigously, but
    // instead use multiple arrays
    std::cout << nums[0] << "\n";
            // Iterators are used to point at container
    // memory locations
    std::vector<int> nums2 = {1,2,3,4};
    
    // Define an iterator as the same type
    std::vector<int>::iterator itr;
    
    // Refer to the vectors begin and end while
    // incrementing the iterator
    for(itr = nums2.begin(); 
            itr < nums2.end(); 
            itr++){
        
        // Get value at the pointer
        std::cout << *itr << "\n";
    }
    
    // You can also increment a set number of spaces
    // Create an iterator and point it at the beginning
    // of the vector
    std::vector<int>::iterator itr2 = nums2.begin();
    
    // Advance 2 spaces
    advance(itr2, 2);
    std::cout << *itr2 << "\n";
    
    // Next works like advance, but it returns an 
    // iterator
    auto itr3 = next(itr2, 1);
    std::cout << *itr3 << "\n";
    
    // Previous moves a set number of indexes and
    // returns an iterator
    auto itr4 = prev(itr2, 1);
    std::cout << *itr4 << "\n";
    
    // You can also insert at a defined index
    std::vector<int> nums3 = {1,4,5,6};
    std::vector<int> nums4 = {2,3};
    auto itr5 = nums3.begin();
    advance(itr5, 1);
    copy(nums4.begin(), nums4.end(), 
            inserter(nums3, itr5));
    
    for(int &i: nums3)
        std::cout << i << "\n";
              
    return 0;
}
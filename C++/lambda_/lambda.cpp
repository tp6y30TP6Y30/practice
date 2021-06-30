// lambda function should be compiled by using c++11 or after
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

vector <int> generateRandVec(int size, int minNum, int maxNum){
	srand(time(nullptr));
	vector <int> v;
	for(int i = 0; i < size; ++i){
		v.push_back(minNum + rand() % (maxNum - minNum + 1));
	}
	return v;
}

void printVec(const vector <int>& v){
	for_each(v.begin(), v.end(), [](int num){
		cout << num << ' ';	
	});
	cout << endl;
}

int countGreaterThan(const vector <int>& v, int num){
	int count = count_if(v.begin(), v.end(), [&](int n){
		return n > num;
	});
	return count;
}

int findGreaterThan(vector <int>& v, int num, int test_num = 0){
	vector <int>::iterator iter = find_if(v.begin(), v.end(), [num, test_num](int n){		/* place num in [] (capture clause) can let the lambda function 
																				   			   be able to access it without & in [] */
																							// [=] denotes, can access all variable
																							// so we can put many arguements in capture clause [] (e.g., [num, test_num])
		cout << test_num << endl;
		return n > num;
	});
	return iter - v.begin();
}

bool handleIndexOutOfRange(int index, int vectorSize){
	try{
		if(index >= vectorSize){
			throw runtime_error("Index out of range. Index: " + to_string(index) + " >= vectorSize: " + to_string(vectorSize));
		}
	}
	catch(exception &exp){
		cout << "Exception Handled: " << exp.what() << endl;
		return false;
	}
	return true;
}

int makeVectorUnique(vector <int>& v){
	// unique function returns the end iterator of the vector after removing all the duplicate elements in the original vector
	vector <int>::iterator iter = unique(v.begin(), v.end(), [](int a, int b){
		return a == b;
	});
	return iter - v.begin();
}

void copyDependsOnEven(vector <int>& from, vector <int>& destination){
	copy_if(from.begin(), from.end(), back_inserter(destination), [](int num){
		return num % 2 == 0;
	});
}

void accumulate(vector <int>& v, int* sum){
	for_each(v.begin(), v.end(), [&](int num){	/* we need to add & in [] to get accessibility of arguement sum which is not in lambda function's local scope */
		*sum += num;
	});
}

vector <int> generateFibList(int maxIndex){
	vector <int> FibList;
	int i = 0;
	function <int (int)> Fib = [&](int n){
		return n < 2 ? n : Fib(n - 1) + Fib(n - 2);
	};
	while(i <= maxIndex){
		FibList.push_back(Fib(i++));
	}
	return FibList;
}

int main(){
	vector <int> v = generateRandVec(10, 50, 100);
	printVec(v);

	int greaterThanCounts = countGreaterThan(v, 75);
	printf("There are %d numbers in vector which are greater than 100.\n", greaterThanCounts);

	sort(v.begin(), v.end(), [](int a, int b){
		return a < b;
	});
	printVec(v);

	int index = findGreaterThan(v, 75);
	assert(handleIndexOutOfRange(index, v.size()));
	cout << "Index " << index << ": " << v[index] << endl;

	printVec(v);
	int v_original_size = v.size(), v_new_size = makeVectorUnique(v);
	assert(v_original_size != v_new_size);
	v.resize(distance(v.begin(), v.begin() + v_new_size));	/* equals to: v.resize(v_new_size);*/
	printVec(v);

	cout << *v.begin() << endl;								/* dereference on iterator equals to get its value */

	vector <int> v_;
	copyDependsOnEven(v, v_);
	printVec(v_);

	// lambda function
	auto fibonacci = [](int index){
		if(index == 0 || index == 1){
			return index;
		}
		else{
			int a = 0, b = 1;
			for(int i = index; i > 1; --i){
				int temp = a + b;
				a = b;
				b = temp;
			}
			return b;
		}
	};
	cout << "Fibonacci(15): " << fibonacci(15) << endl;

	function <int (int)> Fib = [&Fib](int n){				/* Fib is a function vector which returns int and have to pass an int parameter to run 
															   and we need & or &Fib in [] for Fib function itself to get previous Fib result value */
		return n < 2 ? n : Fib(n - 1) + Fib(n - 2);
	};
	cout << "Fibonacci(15): " << Fib(15) << endl;

	int sum = 0;
	accumulate(v_, &sum);
	cout << sum << endl;

	// transform(first, last, output, unary_operation)
	transform(v_.begin(), v_.end(), v_.begin(), [](int num, int offset = 5000){
		return num + offset;
	});
	printVec(v_);

	// transform(first, last, first_, output, binary_operation)
	transform(v_.begin(), v_.end(), v_.begin(), v_.begin(), [](int a, int b){
		return a + b;
	});
	printVec(v_);

	printVec(generateFibList(10));
}
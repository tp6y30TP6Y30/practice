#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <ctime>
using namespace std;

double multiBy2(double num){
	return num * 2;
}

double multiBy3(double num){
	return num * 3;
}

double functions(function<double(double)> func, double num){
	return func(num);
}

bool isItOdd(int num){
	return num & 1;
}

vector <int> changeListToOdd(vector <int>& listOfNums, function<bool(int)> func){
	vector <int> oddList;
	copy_if(listOfNums.begin(), listOfNums.end(), back_inserter(oddList), [&](int num){
		return func(num);
	});
	return oddList;
}

string randomGenerator(){
	return rand() & 1 ? "Head" : "Tail";
}

vector <string> generateStringVec(vector <string>& list, function <string(void)> func){
	for(int i = 0; i < list.size(); ++i){
		list[i] = func();
	}
	return list;
}

void addIfIsHead(string s, int *count){
	*count += s == "Head";
}

void addIfIsTail(string s, int *count){
	*count += s == "Tail";
}

void counter(vector <function <void(string, int*)>> functions, vector <string> list, int *h_count, int *t_count){
	for_each(list.begin(), list.end(), [&](string s){
		functions[0](s, h_count);
		functions[1](s, t_count);
	});
}

int main(){
	double num5 = 5., num6 = 6.0, result = 0.;
	auto times2 = multiBy2;
	printf("%.1f * 2 = %s\n", num5, to_string(functions(multiBy2, num5)).c_str());
	printf("%.1f * 2 = %s\n", num6, to_string(functions(times2, num6)).c_str());
	vector <function <double(double)>> funcs(2);
	funcs[0] = multiBy2, funcs[1] = multiBy3;
	for(auto func: funcs){
		result += func(num6);
	}
	cout << result << endl;

	vector <int> listOfNums(10);
	iota(listOfNums.begin(), listOfNums.end(), 1);
	vector <int> oddList = changeListToOdd(listOfNums, isItOdd);
	for_each(oddList.begin(), oddList.end(), [](int num){
		cout << num << ' ';
	});
	cout << endl;

	srand(time(NULL));
	vector <string> randomList(100);
	randomList = generateStringVec(randomList, randomGenerator);
	int head_count = 0, tail_count = 0;
	vector <function <void(string, int*)>> count_funcs = {addIfIsHead, addIfIsTail};
	counter(count_funcs, randomList, &head_count, &tail_count);
	cout << "Head: " << head_count << endl;
	cout << "Tail: " << tail_count << endl;
}
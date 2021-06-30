#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

bool handleOffsetError(int offset){
	try{
		if(offset > 26 or offset < 0){
			throw runtime_error("offset not in range [0, 26].");
		}
	}
	catch(exception &exp){
		cout << "Handled Error: " << exp.what() << endl;
		return false;
	}
	return true;
}

string cipher(string str, int offset, bool encrypt){
	string returnString = "";
	assert(handleOffsetError(offset));
	if(encrypt) offset *= -1;
	for(auto& c: str){
		if(isalpha(c)) c += offset + 26 * ((c + offset < 'A' + islower(c) * 32) - (c + offset > 'Z' + islower(c) * 32));
		returnString += c;
	}
	return returnString;
}

vector <int> generateRandomVector(int size, int minNum, int maxNum){
	vector <int> v;
	srand(time(nullptr));
	for(int i = 0; i < size; ++i){
		v.push_back(minNum + rand() % (maxNum - minNum + 1));
	}
	return v;
}

int getMaxNum(vector <int>& v){
	int maxNum = INT_MIN;
	for(auto& num: v) maxNum = max(maxNum, num);
	return maxNum;
}

void printVector(vector <int>& v){
	for(auto& num: v) cout << num << ' ';
	cout << endl;
}

void printVectorWithStyle(vector <int>& v){
	int maxNum = getMaxNum(v);
	string row_dash(6 * v.size(), '-'), col_dash = "|";
	cout << row_dash << endl;
	for(int i = 0; i < v.size(); ++i){
		cout << col_dash;
		printf("%3d  ", i);
		cout << (i == v.size() - 1 ? col_dash + "\n" : "");
	}
	cout << row_dash << endl;
	for(int i = 0; i < v.size(); ++i){
		cout << col_dash;
		printf("%3d  ", v[i]);
		cout << (i == v.size() - 1 ? col_dash + "\n" : "");
	}
	cout << row_dash << endl;
}

int fibonacci(int n){
	if(n == 0 || n == 1) return n;
	else{
		int a = 0, b = 1;
		for(int i = 0; i < n - 1; ++i){
			int temp = a + b;
			a = b;
			b = temp;
		}
		return b;
	}
}

int main(){
	string inputString;
	getline(cin, inputString);
	string encrypted_str = cipher(inputString, 5, true), decrypted_str = cipher(encrypted_str, 5, false);
	cout << "Encrypted string: " << encrypted_str << endl;
	cout << "Decrypted string: " << decrypted_str << endl;
	vector <int> v = generateRandomVector(5, 1, 99);
	printVectorWithStyle(v);
	cout << "Fibonacci(20): " << fibonacci(20) << endl;
}
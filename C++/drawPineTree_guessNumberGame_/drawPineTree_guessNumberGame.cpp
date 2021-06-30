#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;

vector <int> parse(string queryQuestion){
	cout << queryQuestion;
	string querySentence, split_query;
	char split_token = ' ';
	getline(cin, querySentence);
	stringstream parser(querySentence);
	vector <int> treeHeights;
	while(getline(parser, split_query, split_token)){
		treeHeights.push_back(stoi(split_query));
	}
	return treeHeights;
}

void printVec(vector <int>& v){
	for(auto& num: v) cout << num << ' ';
	cout << endl;
}

void handleIllegalHeight(int treeHeight){
	try{
		if(treeHeight <= 0){
			throw "Tree height can't be a non-positive number.";
		}
	}
	catch(const char* exp){
		cout << "Error Message: " << exp << endl;
	}
}

void handleIllegalHeight2(int treeHeight){
	try{
		if(treeHeight <= 0){
			throw runtime_error("Tree height can't be a non-positive number (i.e., " + to_string(treeHeight) + ").\n");
		}
	}
	catch(exception &exp){
		cout << "Handled Exception: " << exp.what();
	}
}

int handleIllegalGuessNum(int guessNum, int limits){
	try{
		if(guessNum <= 0 || guessNum > limits){
			throw runtime_error("The number is not in range [1, "+ to_string(limits) + "] (i.e., " + to_string(guessNum) + ").\n");
		}
	}
	catch(exception &exp){
		cout << "Handled Exception: " << exp.what();
		return -1;
	}
	return 0;
}

int getMaxLeaves(int treeHeight){
	handleIllegalHeight2(treeHeight);
	int maxLeaves = 1 + (treeHeight - 1) * 2;
	return maxLeaves;
}

int max_(vector <int>& v){
	int max_value = -1;
	for(auto& num: v) max_value = max(max_value, num);
	return max_value;
}

void drawTree(int maxLeaves){
	int space = maxLeaves;
	string rowLeaves(space, ' '), stem(space, ' ');
	stem[space / 2] = '#';
	for(int i = space / 2; i >= 0; --i){
		rowLeaves[i] = '#';
		rowLeaves[space - i - 1] = '#';
		cout << rowLeaves << endl;
	}
	cout << stem << endl << endl;
}

void guessNumberGame(int limits = 10){
	srand(time(NULL));
	int secretNum = rand() % limits + 1, guessNum = 0;
	do {
		cout << "Guess a number from 1 to " + to_string(limits) + ": ";
		cin >> guessNum;
		if(handleIllegalGuessNum(guessNum, limits) != 0) continue;
		else{
			if(secretNum > guessNum) cout << "Too small\n";
			else if(secretNum < guessNum) cout << "Too big\n";
		}
	} while(secretNum != guessNum);
	cout << "You guessed it.\n";
}

int main(){
	string querySentence = "Enter pine tree height (e.g., 10 3 7) : ";
	vector <int> treeHeights = parse(querySentence);
#ifdef DEBUG
	printVec(treeHeights);
#endif
	for(auto& treeHeight: treeHeights){
		int maxLeaves = getMaxLeaves(treeHeight);
#ifdef DEBUG
		cout << maxLeaves << endl;
#endif
		drawTree(maxLeaves);
	}
	guessNumberGame();
}
#include <iostream>
#include <random>
#include <vector>
using namespace std;

void printVector(vector <int>& v){
	cout << "[";
	for(auto& num: v) cout << num << ' ';
	cout << "]";
	cout << endl;
}

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(vector <int>& v, int startIndex, int endIndex){
	int pivot = v[endIndex], insertIndex = startIndex - 1;
	for(int i = startIndex; i < endIndex; ++i){
		if(v[i] < pivot){
			insertIndex++;
			swap(&v[i], &v[insertIndex]);
		}
	}
	insertIndex++;
	swap(&v[endIndex], &v[insertIndex]);
	return insertIndex;
}

void quickSort(vector <int>& v, int startIndex, int endIndex){
	if(startIndex < endIndex){
		int midIndex = partition(v, startIndex, endIndex);
		quickSort(v, startIndex, midIndex - 1);
		quickSort(v, midIndex + 1, endIndex);
	}
}

void merge(vector<int>& v, int startIndex, int midIndex, int endIndex){
    vector<int> LeftSub(v.begin() + startIndex, v.begin() + midIndex + 1),
                RightSub(v.begin() + midIndex + 1, v.begin() + endIndex + 1);
    LeftSub.insert(LeftSub.end(), INT_MAX);
    RightSub.insert(RightSub.end(), INT_MAX);
    int idxLeft = 0, idxRight = 0;
    for (int i = startIndex; i <= endIndex; ++i) {
        if (LeftSub[idxLeft] <= RightSub[idxRight]) {
            v[i] = LeftSub[idxLeft];
            idxLeft++;
        }
        else{
            v[i] = RightSub[idxRight];
            idxRight++;
        }
    }
}

void mergeSort(vector<int>& v, int startIndex, int endIndex){
    if (startIndex < endIndex) {                   
        int midIndex = (startIndex + endIndex) / 2;         
        mergeSort(v, startIndex, midIndex);    
        mergeSort(v, midIndex + 1, endIndex);    
        merge(v, startIndex, midIndex, endIndex);
    }
}

vector <int> fillVector(int size){
	vector <int> v;
	for(int i = 1; i <= size; ++i){
		int offset = rand() % (size * 10);
		v.push_back(i + offset);
	}
	return v;
}

int binarySearch(vector <int>& v, int target){
	int startIndex = 0, endIndex = v.size() - 1;
	while(startIndex <= endIndex){
		int midIndex = startIndex + (endIndex - startIndex) / 2;
		if(v[midIndex] == target) return midIndex;
		else if(v[midIndex] > target) endIndex = midIndex - 1;
		else startIndex = midIndex + 1;
	}
	return -1;
}

int main(){
	int size;
	cout << "Enter a vector size:\n";
	cin >> size;
	vector <int> v = fillVector(size);
	// printVector(v);
	int sortWay;
	cout << "Enter sort type (0: quickSort, 1: mergeSort):\n";
	cin >> sortWay;
	void (*sort[2])(vector <int>&, int, int) = {quickSort, mergeSort};
	sort[sortWay](v, 0, v.size() - 1);
	int target;
	cout << "Enter a target number to search:\n";
	cin >> target;
	int foundIndex = binarySearch(v, target);
	cout << (foundIndex == -1 ? "Not found\n": "Found in " + to_string(foundIndex) + " position\n");
}

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

class ListNode {
	public:
		ListNode(int val = 0, ListNode* address = nullptr){
			value = val;
			next = address;
		};
		friend void traverseLinklist(ListNode* head);
	private:
		int value;
		ListNode* next;
};

ListNode* createLinklist(vector <int>& v){
	ListNode* head = nullptr;
	for(int i = v.size() - 1; i >= 0; --i){
		if(head == nullptr){
			head = new ListNode(v[i], nullptr);
		}
		else{
			ListNode* temp = new ListNode(v[i], head);
			head = temp;
		}
	}
	return head;
}

void traverseLinklist(ListNode* head){
	while(head != NULL){
		cout << head -> value << " -> ";
		head = head -> next;
	}
	cout << "NULL" << endl;
}

int main(){
	int size = 10;
	vector <int> v(size);
	iota(v.begin(), v.end(), 1);
	ListNode* head = createLinklist(v);
	traverseLinklist(head);
}


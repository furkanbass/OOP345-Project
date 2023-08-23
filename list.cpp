#include <iostream>

struct Node {
	Node* next{};
	int value{};
};

struct List {
	Node* head{};

	List() {};

	void insert_front(int v) {
			head = new Node{ v,head };
	}
	void insert_back(int v) {
		if (head == nullptr) {
			head = new Node{ v, nullptr };
		}
		else {
			Node* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			//current points to the last element
			current->next = new Node{ v, nullptr };
		}
	}
};
int main() {

	List l;
	
	l.insert_front(1);
	l.insert_front(2);
	l.insert_back(1);

	std::cout << "--- List Contains ---" << std::endl;
	Node* current = l.head;
	while (current != nullptr) {
		std::cout << current->value << std::endl;
		current = current->next;
	}
	std::cout << "Done" << std::endl;
	return 0;
}



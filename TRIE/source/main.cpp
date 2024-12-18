#include "algorithms.h"

int main() {
	cout << "Program: Data structure for Autocomplete system\n";
	cout << "Legend: All letters in structure is lowercase and not contain numbers or symbol!\n";
	cout << "Enter '1' to create TRIE\nEnter '2' to create HashTable\nEnter '0' to use comparison mode\n";
	cout << "Enter command!:";
	int cmd; cin >> cmd;
	if (cmd == 0) {
		Comparisons();
	}
	else if (cmd == 1) {
		TrieTesting();
	}
	else if (cmd == 2) {
		HashTableTesting();
	}
	return 0;
}
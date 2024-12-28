#include "algorithms.h"
#include "libs.h"

int main() {
	cout << "Program: Data structure for Autocomplete system\n";
	cout << "Legend: All letters in structure is lowercase and not contain numbers or symbol!\n";
	cout<< "Enter '0' to enter testing mode\n"
		<< "Enter '1' to create TRIE\n";
		 
	cout << "Enter command!:";

	int cmd; cin >> cmd;
	if (cmd == 0) {
		Comparisons();
	}
	else if (cmd == 1) {
		TrieTesting();
	}
	return 0;
}
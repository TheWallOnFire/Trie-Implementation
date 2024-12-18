#pragma once
#include "libs.h"


// K_NODE implementation
struct K_NODE {
	int key = -1;
	int k = 0; // Size of child arr
	K_NODE** child = { NULL };
	int height = 0;
	bool ending = false; // Check end of word
};


K_NODE* createNode(int k); // k is size of child arr
void deleteNode(K_NODE* node);


// Trie implementation
struct TRIE {
	K_NODE* root = NULL;
	// int range = 26;
	// Avaiable char: abcdefghijklmnopqrstvwxyz
};


void coutWord(TRIE* trie, string prefix, int& n, int& comp);
void coutNode(K_NODE* node, string prefix, int& n, int& comp);
void addWord(TRIE* trie, string word, int& comp);
void addNode(K_NODE* node, char key, int& comp);
void removeWord(TRIE* trie, string word, int& comp);
void removeNode(K_NODE* node, int& comp);


TRIE* createTrie();
void deleteTrie(TRIE* trie);
TRIE* loadTrie(string filename = "words.txt");
void TrieTesting();


// Hash Table implementation
// Define the struct to store the unordered_map
struct HashTable {
	unordered_map<string, vector<string>> table;
};

void addWord(HashTable* hashTable, const string& word, int& comp);

void removeWord(HashTable* hashTable, const string& word, int& comp);

void coutWord(HashTable* hashTable, const string& prefix, int n, int& comp);

bool loadWordsFromFile(const string& filename, HashTable* hashTable);

void HashTableTesting();





// Testing autocomplete
void Comparisons();
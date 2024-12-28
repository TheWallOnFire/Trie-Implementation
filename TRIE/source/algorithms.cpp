#include "algorithms.h"
#include "libs.h"

const string WORD_FILE = "data/words.txt";
const string TEST_INPUT = "data/testcase.txt";
const string TEST_OUTPUT = "data/test_result.csv";
const string WORD_LOG = "data/log.txt";

// Create K_Node
K_NODE* createNode(int k)
{
	// Create a node that has k-child
	K_NODE* node = new K_NODE;

	// Create empty childs
	node->k = k; // Size of child arr
	node->child = new K_NODE * [node->k];
	for (int i = 0; i < k; i++) {
		node->child[i] = NULL;
	}
	return node;
}

// Delete K_Node
void deleteNode(K_NODE* node)
{
	// Delete a node that has k-child
	if (!node) return;
	if (node->child) {
		// Has childs
		for (int i = 0; i < node->k; i++) {
			// Recursion
			deleteNode(node->child[i]);
		}
		delete[] node->child;
	}
	delete node;
}


// Print word from prefix
void searchPrefix(TRIE* trie, string prefix, int& n, int& comp)
{
	if (!trie || !isAllLowercase(prefix)) {
		cout << "Error!\n";
		return;
	}

	// Find prefix in trie
	K_NODE* curr = trie->root;
	if (!curr) return;

	string word = {};
	for (int i = 0; i < prefix.size(); i++, comp++) {
		char ch = prefix[i];
		if (++comp && curr->child[(int)(ch - 'a')]) {
			curr = curr->child[(int)(ch - 'a')];
		}
		else return; // Can't find
	}

	// Cout n words that start with given prefix
	searchNode(curr, prefix, n, comp);
}
void searchNode(K_NODE* node, string prefix, int& n, int& comp)
{
	// Checking condition
	if (n < 1 || !node) return;

	// Cout word
	if (++comp && node->ending) {
		cout << prefix << endl;
		n--;
	}

	// Recursion for each child
	for (int i = 0; i < 26 && n > 0; i++, ++comp) {
		if (node->child[i]) {
			string word = prefix;
			word += (char)(i + 'a');
			searchNode(node->child[i], word, n, comp);
		}
	}
}

// Add word to Trie
void addWord(TRIE* trie, string word, int& comp)
{
	if (!trie || !isAllLowercase(word)) {
		cout << word << " can't be added into TRIE\n";
		return;
	}

	// Add new word to trie;
	K_NODE* curr = trie->root;
	for (char ch : word) {
		comp++;
		addNode(curr, ch, comp);
		curr = curr->child[(int)(ch - 'a')];
	}

	// Assign last char is end of word
	if (curr->ending == true) {
		cout << word << " is already in TRIE!\n";
	}
	else {
		curr->ending = true;
	}
}
void addNode(K_NODE* node, char key, int& comp)
{
	// Create node for key
	if (!node) {
		cout << "Empty node!\n";
		return;
	}
	int index = (int)(key - 'a');
	if (++comp && !node->child[index]) {
		node->child[index] = createNode(26);
		if (!node->child[index])
			cout << "Empty child!" << key << endl;
		else
			node->child[index]->key = (int)key;
	}
}

// Remove word from Trie
void removeWord(TRIE* trie, string word, int& comp)
{
	if (!trie || !isAllLowercase(word)) {
		cout << "Can't delete word\n";
		return;
	}

	K_NODE* curr = trie->root;
	if (!curr) return;
	K_NODE* temp = NULL; // Nearest multibranch Node from curr
	char c = 'a';

	for (char ch : word) {
		comp++;
		if (++comp && !curr->child[(int)(ch - 'a')]) {
			cout << "Can't find word/n";
			return;
		}
		else {
			// count number of child
			int childs = 0;
			for (int i = 0; i < 26; i++, ++comp) {
				if (++comp && !curr->child[i]) childs++;
			}
			// Check curr is unremovable 
			if (++comp && childs > 1) {
				temp = curr; c = ch;
			}
			curr = curr->child[(int)(ch - 'a')];
		}
	}

	// curr is last char in word
	// check curr is leaf
	for (int i = 0; i < 26; i++, ++comp) {
		if (!curr->child[i]) {
			curr->ending = false;
			return;
		}
	}

	// Delete below del
	if (++comp && temp) {
		removeNode(temp->child[(int)(c - 'a')], comp);
	}
}
void removeNode(K_NODE* node, int& comp)
{
	// Recursion and loop all root
	if (!node) return;
	for (int i = 0; i < 26; i++, ++comp) {
		if (++comp && node->child[i]) removeNode(node->child[i], comp);
	}
	delete node;
}

// Create Trie
TRIE* createTrie()
{
	TRIE* trie = new TRIE;
	trie->root = createNode(26);
	trie->root->key = 0;
	return trie;
}

// Delete Trie
void deleteTrie(TRIE* trie)
{
	if (!trie) return;
	deleteNode(trie->root);
	delete trie; trie = NULL;
}

// Load Trie from file
TRIE* loadTrie(string filename)
{
	ifstream input;
	input.open(filename);
	if (!input.is_open()) return NULL;
	cout << "Loading data from file!\n";
	TRIE* trie = createTrie();
	if (!trie) return trie;
	string line;
	int size = 0;
	while (getline(input, line)) {
		stringstream ss(line);
		string word;
		while (getline(ss, word)) {
			int comp = 0;
			if (word.size() < 3 || !isAllLowercase(word)) continue;
			addWord(trie, word, comp);
			size++;
		}
	}

	cout << "Trie created!\n";
	cout << "Size: " << size << endl;
	return trie;
}

// Test Trie
void TrieTesting() {
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start; 
	auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	float time = (float)(timer.count()) / 1'000'000.0;

	cout << "Load trie from file? (YES/NO)";
	string input; cin >> input;
	TRIE* trie = nullptr;
	if (input == "YES") {
		// Load data and measure time
		start = std::chrono::high_resolution_clock::now();
		trie = loadTrie(WORD_FILE);
		end = std::chrono::high_resolution_clock::now();
		timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		time = (float)(timer.count()) / 1'000'000.0;
		if (!trie) {
			cout << "Can't create trie from " << WORD_FILE << endl;
			return;
		}
		else cout << "Trie created from " << WORD_FILE << endl;
		// Cout and save result
		cout << "Time for creating trie from " << WORD_FILE << ":" << time << endl;

	}
	else {
		trie = new TRIE;
		trie->root = createNode('a');
	}
	clearContentOfFile(WORD_LOG);
	
	// Time stop
	while (true) {
		int cmd = 0; int comp = 0;

		cout << "\n########LIST OF COMMANDS!###########\n"
			<< "0->End Program\n"
			<< "1->Add new word\n"
			<< "2->Search words\n"
			<< "3->Remove word\n";
		cout << "Enter command: "; cin >> cmd;

		switch (cmd) {
		case 0: {
			// Exit
			cout << "Stop the program!\n";
			return;
		}
		case 1: {
			// Add
			cout << "Add word:";
			string word; cin >> word;

			// Measure time
			appendToFile(WORD_LOG, "Add: " + word);
			start = std::chrono::high_resolution_clock::now();
			addWord(trie, word, comp);
			end = std::chrono::high_resolution_clock::now();
			timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			break;
		}
		case 2:
		{
			// Cout
			cout << "Enter prefix: ";
			string word; cin >> word;
			cout << "Number of words:";
			int n = 0; cin >> n;
			if (n < 0) n = 0;

			// Measure time
			appendToFile(WORD_LOG, "Prefix: " + word);
			appendToFile(WORD_LOG, "Number: " + to_string(n));
			start = std::chrono::high_resolution_clock::now();
			searchPrefix(trie, word, n, comp);
			end = std::chrono::high_resolution_clock::now();
			timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			if (n > 0) cout << "Can't find more words!\n";
			break;
		}
		case 3:
		{
			// Remove
			cout << "Remove word:";
			string word; cin >> word;

			// Measure time
			appendToFile(WORD_LOG, "Remove: " + word);
			start = std::chrono::high_resolution_clock::now();
			removeWord(trie, word, comp);
			end = std::chrono::high_resolution_clock::now();
			timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			break;
		}
		default: {
			// Unknown
			cout << "Unknown command!\n";
			continue;
		}
		}
		// Debug time
		cout << "Time running: " << time << endl;
		cout << "Comparison  : " << comp << endl;
		const string log = to_string(time) + "(s)-----" + to_string(comp) + "(time)";
		appendToFile(WORD_LOG, log);
		cout << "Store into " << WORD_LOG << endl;
	}
	deleteTrie(trie); trie = NULL;
}





// Function to add a word to the hash table
void addWord(HashTable* hashTable, const string& word, int& comp) {
	for (size_t i = 1; i <= word.length(); ++i) {
		string prefix = word.substr(0, i);
		auto& wordList = hashTable->table[prefix];

		// Check if the word is already in the prefix list
		bool found = false;
		for (const auto& w : wordList) {
			++comp; // Increment comparison counter
			if (w == word) {
				found = true;
				break;
			}
		}
		if (!found) {
			wordList.push_back(word);
		}
	}
}

// Function to remove a word from the hash table
void removeWord(HashTable* hashTable, const string& word, int& comp) {
	for (size_t i = 1; i <= word.length(); ++i) {
		string prefix = word.substr(0, i);
		auto& wordList = hashTable->table[prefix];

		// Find and remove the word from the prefix list
		auto it = find(wordList.begin(), wordList.end(), word);
		if (it != wordList.end()) {
			wordList.erase(it);
		}
	}
}

// Function to find words starting from a given prefix
void searchPrefix(HashTable* hashTable, const string& prefix, int n, int& comp) {
	// Check if the prefix exists in the hash table
	if (hashTable->table.find(prefix) != hashTable->table.end()) {
		auto& wordList = hashTable->table[prefix];
		int count = 0;

		// Print up to n words from the prefix list
		for (size_t i = 0; i < wordList.size() && count < n; ++i) {
			++comp;  // Count each comparison
			cout << wordList[i] << endl;
			++count;
		}

		// If less than n words are found, notify the user
		if (count < n) {
			cout << "Only " << count << " words found for the prefix \"" << prefix << "\"." << endl;
		}
	}
	else {
		cout << "No words found with the prefix \"" << prefix << "\"." << endl;
	}
}

HashTable* createHashTable()
{
	HashTable* hash = new HashTable;
	return hash;
}

// Function to load words from a file into the hash table
bool loadWordsFromFile(const string& filename, HashTable* hashTable) {
	ifstream input;
	input.open(filename);
	if (!input.is_open()) {
		cout << "Failed to open file: " << filename << endl;
		return false;
	}

	cout << "Loading data from file!\n";
	string line;
	int size = 0;

	while (getline(input, line)) {
		stringstream ss(line);
		string word;
		while (getline(ss, word, ' ')) {
			int comp = 0;
			if (word.size() < 3 || !isAllLowercase(word)) continue;
			addWord(hashTable, word, comp);
			size++;
		}
	}

	cout << "Hash table created!\n";
	cout << "Size: " << size << endl;
	return true;
}

// Function to simulate user commands on the hash table
void HashTableTesting() {
	HashTable* hashTable = new HashTable;

	// create dataset for testing
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	float time = 0;

	// Trie
	start = std::chrono::high_resolution_clock::now();
	if (!loadWordsFromFile(WORD_FILE, hashTable)) {
		return;
	}
	end = std::chrono::high_resolution_clock::now();
	timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	time = (float)(timer.count()) / 1'000'000.0;

	// Cout and save result
	cout << left << setw(15) << "Create trie time:" << setw(10) << time << endl;

	int comp = 0;

	while (true) {
		int cmd = 0;
		cout << "1->Add new word\n2->Find word\n3->Remove word\n";
		cout << "Enter command: "; cin >> cmd;

		float time = 0;

		switch (cmd) {
		case 0: {
			// Exit
			cout << "Comparisons: " << comp << endl;
			cout << "Exit!\n";
			return;
		}
		case 1: {
			// Add word
			cout << "Add word: ";
			string word; cin >> word;

			// Record start time
			auto start = std::chrono::high_resolution_clock::now();

			// Call the function
			addWord(hashTable, word, comp);

			// Record end time
			auto end = std::chrono::high_resolution_clock::now();

			// Calculate elapsed time
			auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			break;
		}
		case 2: {
			// Find word by prefix
			cout << "Enter prefix to find: ";
			string prefix; cin >> prefix;
			cout << "Number of words to display: ";
			int n = 0; cin >> n;

			// Record start time
			auto start = std::chrono::high_resolution_clock::now();

			// Call the function
			searchPrefix(hashTable, prefix, n, comp);

			// Record end time
			auto end = std::chrono::high_resolution_clock::now();

			// Calculate elapsed time
			auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			break;
		}
		case 3: {
			// Remove word (similar to add but remove functionality can be added here)
			cout << "Remove word: ";
			string word; cin >> word;

			// Record start time
			auto start = std::chrono::high_resolution_clock::now();

			// Call the function (removeWord would be similar to addWord)
			// removeWord(hashTable, word, comp);

			// Record end time
			auto end = std::chrono::high_resolution_clock::now();

			// Calculate elapsed time
			auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			break;
		}
		default: {
			cout << "Unknown command!\n";
			continue;
		}
		}

		cout << "Time running: " << time << " seconds\n";
		cout << "Comparisons: " << comp << endl;
	}
}






// Testing and compare 2 structure
void Comparisons()
{
	// save result to file test_result.csv
	ofstream output; output.open(TEST_OUTPUT);
	if (!output.is_open()) {
		cout << "failed to open " << TEST_OUTPUT << endl;
		return;
	}

	// Load words
	output << "Load word,time\n";

	// Trie
	auto start = std::chrono::high_resolution_clock::now();
	TRIE* trie = loadTrie(WORD_FILE);
	auto end = std::chrono::high_resolution_clock::now();
	auto timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	float time = (float)(timer.count()) / 1'000'000.0;

	// Cout and save result
	cout << left << setw(15) << "Create trie time:" << setw(10) << time << endl;
	output << "Trie," << time << endl;

	// Hash Table
	HashTable* hashtable = createHashTable();
	start = std::chrono::high_resolution_clock::now();
	loadWordsFromFile(WORD_FILE, hashtable);
	end = std::chrono::high_resolution_clock::now();
	timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	time = (float)(timer.count()) / 1'000'000.0;

	// Cout and save result
	cout << left << setw(15) << "Create hash table time:" << setw(10) << time << endl;
	output << "Hash Table," << time << endl;

	// Load data
	vector<string> listOfWords = loadWordsFromFile(TEST_INPUT);
	int numberOfWords[] = { 1,5,10,15 };
	size_t n = sizeof(numberOfWords) / sizeof(numberOfWords[0]);
	output << "\nFind words from prefix\n";

	// Loop 
	for (int i = 0; i < n; i++) {
		// Cout number of word
		output << "Number of words, " << numberOfWords[i] << ",";

		// Cout list of word
		for (string word : listOfWords) {
			output << word << ",,";
		}
		output << endl;

		output << "TRIE" << ",,";

		// Test trie - Find word
		for (string word : listOfWords) {
			int comp = 0; int count = numberOfWords[i];

			// Trie
			start = std::chrono::high_resolution_clock::now();
			searchPrefix(trie, word, count, comp);
			end = std::chrono::high_resolution_clock::now();
			timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			// Cout and save result
			cout << left << setw(15) <<	 "Running time:" << setw(10) << time << "|"
				<< left << setw(10) << "Comparisons: " << setw(10) << comp << endl;
			output << time << "," << comp << ",";
		}

		// Test trie - Find word
		output << endl << "Hash Table,,";
		for (string word : listOfWords) {
			int comp = 0;  int count = numberOfWords[i];

			// Trie
			start = std::chrono::high_resolution_clock::now();
			searchPrefix(hashtable, word, count, comp);
			end = std::chrono::high_resolution_clock::now();
			timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			time = (float)(timer.count()) / 1'000'000.0;

			// Cout and save result
			cout << left << setw(15) << "Running time:" << setw(10) << time << "|"
				<< left << setw(10) << "Comparisons: " << setw(10) << comp << endl;
			output << time << "," << comp << ",";
		}
		output << endl;
	}

	// Deallocate
	output << "\nDeallocate,time" << endl;
	// Trie
	start = std::chrono::high_resolution_clock::now();
	deleteTrie(trie);
	end = std::chrono::high_resolution_clock::now();
	timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	time = (float)(timer.count()) / 1'000'000.0;

	// Cout and save result
	cout << left << setw(15) << "Delete trie time:" << setw(10) << time << endl;
	output << "Trie," << time << endl;

	// Hash Table
	start = std::chrono::high_resolution_clock::now();
	delete hashtable;
	end = std::chrono::high_resolution_clock::now();
	timer = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	time = (float)(timer.count()) / 1'000'000.0;

	// Cout and save result
	cout << left << setw(15) << "Delete hash table time:" << setw(10) << time << endl;
	output << "Hash table," << time << endl;
}

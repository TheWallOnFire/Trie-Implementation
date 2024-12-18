#include "libs.h"
#include <cctype>

bool isAllLowercase(const std::string& str) {
    for (char c : str) {
        if (!std::islower(c)) {
            return false; // If any character is not lowercase, return false
        }
    }
    return true; // All characters are lowercase
}

// Function to load words from a file into a vector
vector<string> loadWordsFromFile(const string& filename) {
    ifstream input(filename);
    vector<string> words;

    if (!input.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return words;  // Return empty vector if file cannot be opened
    }

    string word;
    while (input >> word) {
        words.push_back(word);  // Add each word to the vector
    }

    input.close();  // Close the file after reading

    return words;
}

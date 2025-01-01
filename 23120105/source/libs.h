#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <conio.h>
#include <stdlib.h> 
#include <chrono>
#include <iomanip>
#include <ctime>
using namespace std;

bool isAllLowercase(const std::string& str);
vector<string> loadWordsFromFile(const string& filename);
void appendToFile(const string& filename, const string& word);
void clearContentOfFile(const string& filename);
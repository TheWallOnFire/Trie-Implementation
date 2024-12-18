#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <iomanip>
using namespace std;

bool isAllLowercase(const std::string& str);
vector<string> loadWordsFromFile(const string& filename);

#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <map>


using namespace std;

// CONSTANTS
const float FRIENDLY = 0.8;
const float GRUFF = 0.2;
const float MODERATE = 0.5;

const int L1_TURNS = 3;
const int L2_TURNS = 3;
const int L3_TURNS = 3;

const int NEGO1 = 1;
const int NEGO2 = 2;
const int NEGO3 = 3;

// Inventory
map<string, int> economy;

// Key pointers used across functions
Encounter* currNego = nullptr;
Encounter* level1 = nullptr;
Encounter* level2 = nullptr;
Encounter* level3 = nullptr;

PlayerCharacter* player = nullptr;

// Empty vectors for possible user inputs
vector<string> genActions;
vector<string> negoActions;

// General globals
string uInput = "";
bool negoPossible = false;
bool inNego = false;
bool bidAccepted = false;
bool anyKeyOK = true;


#endif

#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <map>


using namespace std;

// CONSTANTS
const float FRIENDLY = 0.8, GRUFF = 0.2, MODERATE = 0.5;
const int L1_TURNS = 3, L2_TURNS = 3, L3_TURNS = 3, L4_TURNS = 3;
const float L1_KEY = 10.0, L2_KEY = 20.0, L3_KEY = 30.0, L4_KEY = 40.0;


// Inventory
map<string, float> economy;

// Key pointers used across functions
Encounter*       level1 = nullptr;
Encounter*       level2 = nullptr;
Encounter*       level3 = nullptr;
Encounter*       level4 = nullptr;

PlayerCharacter* player = nullptr;

Negotiator*      opponent1 = nullptr;
Negotiator*      opponent2 = nullptr;
Negotiator*      opponent3 = nullptr;
Negotiator*      opponent4 = nullptr;

// General globals
string uInput = "", PROMPT_DIVIDER = "", INVALID_INPUT = "";

bool doSkip = false;
bool wonEncounter = false;
int currLevel = 1;

#endif

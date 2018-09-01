#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <map>

using namespace std;

// CONSTANTS
const int WIDTH = 80;
const float FRIENDLY = 0.8;
const float GRUFF    = 0.2;
const float MODERATE = 0.5;

const int L0_TURNS = 3;
const int L1_TURNS = 3;
const int L2_TURNS = 3;
const int L3_TURNS = 3;
const int L4_TURNS = 3;

const float L0_KEY = 40.0;
const float L1_KEY = 20.0;
const float L2_KEY = 30.0;
const float L3_KEY = 40.0;
const float L4_KEY = L0_KEY;


// Inventory
map<string, float> economy;

// Key pointers used across functions
Encounter*       level0 = nullptr;
Encounter*       level1 = nullptr;
Encounter*       level2 = nullptr;
Encounter*       level3 = nullptr;
Encounter*       level4 = nullptr;

PlayerCharacter* player = nullptr;

Negotiator*      opponent0 = nullptr;
Negotiator*      opponent1 = nullptr;
Negotiator*      opponent2 = nullptr;
Negotiator*      opponent3 = nullptr;
Negotiator*      opponent4 = nullptr;

// General globals
string uInput = "", PROMPT_DIVIDER = "", INVALID_INPUT = "";
string title0 = "", title1 = "", title2 = "",
       title3 = "", title4 = "", titleEnd = "";

bool veryBeginning = true;
bool doSkip = false;
bool wonEncounter = false;
int currLevel = 0;

#endif


/*
METADATA
Name: Sagar Doshi
Date: 24 July 2018
Title: Negotiations Video Game
Course: MSc Computing Science
University: Imperial College London
*/

#include "../inc/Negotiator.h"
#include "../inc/Encounter.h"
#include "../inc/PlayerCharacter.h"
#include "../inc/Globals.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


// Adds total items in economy... unfortunately duplicated in Negotiator.cpp
void createEconomy() {
    // 0) Tutorial Lepha
    economy["black trousers"] = 1.0;
    economy["black tunic"] = 1.0;

    // 1) Mosta the Stork/Pepita the Pigeon -- spirits of inquisitiveness
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["packet of sunflower seeds"] = 5.0;

    // 2) Toto the #2 Rabbit of Centzon Totochtin -- spirit of drunkenness
    economy["pulque flask"] = 5.0;
    economy["paint canister"] = 2.0;
    economy["morning headache tonic"] = 10.0;
    economy["long ear warmers"] = 5.0;


    // 3) Burro the night river beaver -- spirit of industriousness
    economy["wood polish bottle"] = 6.0;
    economy["waterproof wax jar"] = 8.0;

    // 4) lepha -- spirit of order

}

// Defines PROMPT_DIVIDER and INVALID_INPUT
void createGlobalStrings() {
    PROMPT_DIVIDER =  "----------------------------------------";
    PROMPT_DIVIDER += "----------------------------------------\n";

    INVALID_INPUT = "\n" + PROMPT_DIVIDER + "\nInvalid input. Try again.\n";
}

// Creates player and all enemies
void createCharacters() {
    player = new PlayerCharacter("You", MODERATE);

    opponent0 = new Negotiator("Lepha", MODERATE);
    opponent1 = new Negotiator("Mosta and Pepita", FRIENDLY);
    opponent2 = new Negotiator("Toto", MODERATE);
    opponent3 = new Negotiator("Burro", GRUFF);
    opponent4 = opponent0;

}

// Converts a string fully to lowercase (in place)
void lower(string &anyString) {
    int i = 0;
    while (anyString[i]) {
        anyString[i] = tolower(anyString[i]);
        i++;
    }
}

// Removes whitespaces in string (in place)
void removeWS(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

// Packages lower() & removeWS() with a getline call, using global uInput
void getCleanLine(istream &is, string &str) {
    getline(cin, str);
    lower(str);
    removeWS(str);
}

// Pauses text for user to read and get a breather
void checkpoint() {
    string anyInputText  = "\n" + PROMPT_DIVIDER + "[Press return to continue ";
           anyInputText += "or type \"skip\" to jump to the next section.] ";

    cout << anyInputText;
    getCleanLine(cin, uInput);

    if (uInput == "skip" ) doSkip = true; // Set global flag for future

    cout << PROMPT_DIVIDER << "\n";
}

// Starts game and asks if player wants to jump to encounter
void startScreen() {
    string title  = "\n\n========================================";
           title +=     "========================================\n";
           title +=     "                             ";
           title +=     "A DEAL WITH THE SPIRITS";
           title +=     "                            \n";
           title +=     "========================================";
           title +=     "========================================\n\n";

    cout << title;

    checkpoint();
}

// Gets simple input to lowercase; callable with specific prompt/target
void setUInput(string prompt = "", string targetInput = "") {

    uInput = ""; // Clear uInput
    string specificInputText = "\n" + PROMPT_DIVIDER + ">>>> ";

    cout << prompt << specificInputText;
    getCleanLine(cin, uInput);

    lower(targetInput);
    removeWS(targetInput);

    // Only worry about invalidity if targetInput not empty
    if (targetInput != "") {
        // Then if wrong input, note invalidity, and re-print prompt
        while (uInput != targetInput) {
            cout << INVALID_INPUT << prompt << specificInputText;
            getCleanLine(cin, uInput);
        }
    }
    cout << PROMPT_DIVIDER << "\n";
}

// Require a specific input from user, as coded by text file
void handleSpecificInput(string line, string &output) {
    cout << output; // Flush output thus far

    const int ASTERISKS_NEEDED = 3;
    int newLength = line.length() - ASTERISKS_NEEDED - ASTERISKS_NEEDED;

    string targetInput = line.substr(ASTERISKS_NEEDED, newLength);
    string prompt = "\nType \"" + targetInput + "\"\n";
    setUInput(prompt, targetInput);

    // Once inventory typed, display it
    if (targetInput == "inventory") player->printInventory();

    output = ""; // Reset output
}

// Ensures that line is of format ***SOME_KEYWORD***
bool isAsteriskEntry(string line) {
    // Must have three asterisks at beginning AND end of prompt

    const int ASTERISKS_NEEDED = 3;
    if (line.length() <= (ASTERISKS_NEEDED + ASTERISKS_NEEDED)) return false;

    for (int i = 0; i < ASTERISKS_NEEDED; i++) {
        if (line[i] != '*') return false; // Check for asterisks at beginning
        if (line[line.length() - i - 1] != '*') return false; // and at end
    }

    return true;
}

// Prints txt script and replaces key symbols with frequent functions
void loadScript(string filename) {
    if (doSkip) return; // Skip all text until encounter

    string filepath = "txt/" + filename + ".txt";
    string line = "", output = "";

	ifstream in;
	in.open(filepath);

	while (getline(in, line)) {
        if (line == "***") {
            cout << output;
            checkpoint();
            output = "";

            // Handle "skip" typed within a script file
            if (doSkip) {
                in.close();
                return;
            }
        } else if (isAsteriskEntry(line)) handleSpecificInput(line, output);
        else output += "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
}

// Can be called any time -- ends game
void quitGame() {
    loadScript("Gen/QuitGame");
    exit(0);
}



// After intros, these kick off each individual level
void startCurrentLevel() {
    string outText  = to_string(currLevel) + "/Level";
           outText += to_string(currLevel) + "Start";

    doSkip = false; // Allow scripts at beginning of level
    loadScript(outText);

    // Extra instruction at the start
    if (currLevel == 0) loadScript("Gen/NegotiationStrategy");
}

void levelEnd(bool wonEncounter) {
    string outText  = to_string(currLevel) + "/Level" + to_string(currLevel);
    // Load the correct script
    (wonEncounter ? (outText += "Win") : (outText += "Lose"));

    doSkip = false; // Allow scripts after encounter
    loadScript(outText); // Text for victory or loss to be run first

    // If lost and not in tutorial
    if (!wonEncounter && (currLevel > 0)) quitGame();

    currLevel++;
    return;
}

// Runs text before level, runs encounter, and runs text after level
void runNextLevel(Encounter* levelPointer) {
    startCurrentLevel(); // Intro text for level
    levelPointer->runEncounter(economy, wonEncounter);
    levelEnd(wonEncounter); // Ending text for level (plus ending if lost)
}

// Loads intro scripts in turn
void runIntro() {
    player->fillInventory(); // Give player an inventory before the intro
    loadScript("0/Intro"); // Early exposition until discovery by Lepha

    player->initInventory(); // Clear inventory before tutorial level
    level0 = new Encounter(player, opponent0, L0_TURNS, L0_KEY);
    runNextLevel(level0);

    player->fillInventory(); // Refill inventory before Level 1
}

void playGame() {

    startScreen(); // Ask to skip intro

    runIntro(); // Runs intro

    // Create levels immediately before running for freshest inventory mapping
    level1 = new Encounter(player, opponent1, L1_TURNS, L1_KEY);
    runNextLevel(level1);

    level2 = new Encounter(player, opponent2, L2_TURNS, L2_KEY);
    runNextLevel(level2);

    level3 = new Encounter(player, opponent3, L3_TURNS, L3_KEY);
    runNextLevel(level3);

    level4 = new Encounter(player, opponent4, L4_TURNS, L4_KEY);
    runNextLevel(level4);

}

void fillDictsAndCharacters() {
    createEconomy();
    createGlobalStrings();
    createCharacters(); // including player BEFORE creating levels
}

void releaseMemory() {
    // Delete levels
    delete level0;
    delete level1;
    delete level2;
    delete level3;
    delete level4;

    // Delete characters
    delete opponent0; // No need to delete opponent4, as it's a duplicate
    delete opponent1;
    delete opponent2;
    delete opponent3;

    delete player;
}

int main() {

    fillDictsAndCharacters();

    playGame();

    releaseMemory();

    return 0;
}

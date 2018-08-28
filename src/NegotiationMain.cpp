
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

    // If we imagine that opponents are now:
    // 1) Mosta the Maribou Stork & Pepita the Pigeon -- spirits of play
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["sunflower seeds"] = 10.0;

    // 2) centzon totochtin -- spirit of drunkenness
    // 3) night river beaver -- spirit of industriousness
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

    opponent1 = new Negotiator("Mosta and Pepita", FRIENDLY);
    opponent2 = new Negotiator("Toto", FRIENDLY);
    opponent3 = new Negotiator("Burro", MODERATE);
    opponent4 = new Negotiator("LEPHA", GRUFF);

}


// Pauses text for user to read and get a breather
void checkpoint() {
    string anyInputText  = "\n" + PROMPT_DIVIDER + "[Press return to continue ";
           anyInputText += "or type \"skip\" to jump to the next encounter.] ";

    cout << anyInputText;
    getline(cin, uInput);

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

// Converts a string fully to lowercase (in place)
string lower(string anyString) {
    int i = 0;
    while (anyString[i]) {
        anyString[i] = tolower(anyString[i]);
        i++;
    }

    return anyString;
}

// Gets simple input to lowercase; callable with specific prompt/target
void setUInput(string prompt = "", string targetInput = "") {

    uInput = ""; // Clear uInput
    string specificInputText = "\n" + PROMPT_DIVIDER + ">>>> ";

    cout << prompt << specificInputText;
    getline(cin, uInput);

    // Only worry about invalidity if targetInput not empty
    if (targetInput != "") {
        // Then if wrong input, note invalidity, and re-print prompt
        while (uInput != lower(targetInput)) {
            cout << INVALID_INPUT << prompt << specificInputText;
            getline(cin, uInput);
        }
    }
    cout << PROMPT_DIVIDER << "\n";

    lower(uInput);

}

// Checks if user input is a particular string
void specificInputCheck(string targetInput) {
    string prompt = "Type \"" + targetInput + "\"\n";

    setUInput(prompt, targetInput);

    // Once inventory typed, display it
    if (targetInput == "inventory") player->printInventory();
}

// Prints txt script and replaces key symbols with frequent functions
void loadScript(string filename) {
    if (doSkip) return; // Skip all text until encounter

    string filepath = "txt/" + filename + ".txt";

    string output = "";
    string line = "";

	ifstream in;
	in.open(filepath);

	while (getline(in, line)) {
        if (line == "***") {
            cout << output;
            checkpoint();

            // Handle "skip" typed within a script file
            if (doSkip) {
                in.close();
                return;
            }

            output = "";
        } else if (line == "***inventory***") {
            cout << output;
            specificInputCheck("inventory");
            output = "";
        } else if (line == "***stop***") {
            cout << output;
            specificInputCheck("stop");
            output = "";
        } else output += "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
}

// Can be called any time -- ends game
void quitGame() {
    loadScript("Gen/QuitGame");
    exit(0);
}


// Loads intro scripts in turn
void runIntro() {
    loadScript("0/Intro1"); // Early exposition until discovery by Lepha
    loadScript("0/Intro2"); // Background on you, worldbuilding, your motivation
}

// After intros, these kick off each individual level
void levelStart(int ) {
    string outText  = to_string(currLevel) + "/Level";
           outText += to_string(currLevel) + "Start";

    doSkip = false; // Allow scripts at beginning of level
    loadScript(outText);

    // Extra instruction at the start
    if (currLevel == 1) loadScript("Gen/NegotiationStrategy");
}

void levelEnd(bool wonEncounter) {
    string outText  = to_string(currLevel) + "/Level" + to_string(currLevel);
    // Load the correct script
    (wonEncounter ? (outText += "Win") : (outText += "Lose"));

    doSkip = false; // Allow scripts after encounter
    loadScript(outText); // Text for victory or loss to be run first

    if (!wonEncounter) quitGame();

    currLevel++;
    return;
}

// Runs text before level, runs encounter, and runs text after level
void runNextLevel(Encounter* levelPointer) {
    levelStart(currLevel); // Intro text for level
    levelPointer->runEncounter(economy, wonEncounter);
    levelEnd(wonEncounter); // Ending text for level (plus ending if lost)
}

void playGame() {

    startScreen(); // Ask to skip intro

    player->fillInventory(); // Give player an inventory before the intro

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
    delete level1;
    delete level2;
    delete level3;
    delete level4;

    // Delete characters
    delete opponent1;
    delete opponent2;
    delete opponent3;
    delete opponent4;
    delete player;
}

int main() {

    fillDictsAndCharacters();

    playGame();

    releaseMemory();

    return 0;
}

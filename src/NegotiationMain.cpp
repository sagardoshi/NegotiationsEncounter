
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

// Only allowable actions are help, quit, inventory, turns, & propose
void createActions() {
    // Acceptable actions more generally
    genActions.push_back("help");
    genActions.push_back("quit");
    genActions.push_back("inventory");

    // Negotiation actions
    negoActions.push_back("turns");
    negoActions.push_back("propose");
}

// Adds total items in economy... unfortunately duplicated in Negotiator.cpp
void createEconomy() {

    // If we imagine that opponents are now:
    // 0) Mosta the Maribou Stork & Pepita the Pigeon -- spirits of play
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["sunflower seeds"] = 10.0;
    economy["bird spirit key"] = 10.0;

    // 1) centzon totochtin -- spirit of debauchery
    // 2) night river beaver -- spirit of industriousness
    // 3) lepha -- spirit of order

}

// Defines PROMPT_DIVIDER and INVALID_INPUT
void createGlobalStrings() {
    PROMPT_DIVIDER =  "----------------------------------------";
    PROMPT_DIVIDER += "----------------------------------------\n";

    INVALID_INPUT = "\n" + PROMPT_DIVIDER + "\nInvalid input. Try again.\n";
}

// Creates player, Mosta/Pepita
void createCharacters() {
    player = new PlayerCharacter("You", MODERATE);

    birdSpirits = new Negotiator("Mosta and Pepita", FRIENDLY);
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
    // uInput = "";

    string title  = "\n\n========================================";
           title +=     "========================================\n";
           title +=     "                            ";
           title +=     "DEALING WITH THE SPIRITS";
           title +=     "                            \n";
           title +=     "========================================";
           title +=     "========================================\n\n";

    cout << title;

    checkpoint();

    // string skipIntroText =  "\n" + PROMPT_DIVIDER;
    //        skipIntroText += "Type \"start\" to start from the beginning.\n";
    //        skipIntroText += "Type \"skip\" to skip to the first encounter.\n";
    //        skipIntroText += ">>>> ";
    //
    //
    // // Print title and prompt to skip
    // cout << title << skipIntroText;
    // getline(cin, uInput);
    //
    // // Keep reprinting prompt until input valid
    // while (uInput != "start" && uInput != "skip") {
    //     cout << INVALID_INPUT << skipIntroText;
    //     getline(cin, uInput);
    // }
    // cout << PROMPT_DIVIDER << "\n";
    //
    // if (uInput == "skip") doNotSkip = false;
    //
    // uInput = ""; // Clear uInput
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

// Gets simple, unverified user input and converts to lowercase
// Can be called with particular prompt/target
void setUInput (string prompt = "", string targetInput = "") {

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

            if (doSkip) { // If user typed "skip", end this script now
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

// Prints help menu, unfortunately duplicated in PlayerCharacter.cpp
void printHelp() {
    string helpBorder = "***** HELP *****\n";

    string extraHelp  = "Add your inventory items by number to your offer\n\n";
           extraHelp += "propose: type this to send your current offer\n";
           extraHelp += "turns: see how many turns you have left\n";

    string helpText   = "inventory: double-check what you currently hold\n";
           helpText  += "help: see this menu\n";
           helpText  += "quit: exit the game\n\n";

    (inNego ? helpText = helpBorder + extraHelp + helpText + helpBorder :
              helpText = helpBorder +             helpText + helpBorder);

    cout << helpText;

}

// Can be called anytime -- ends game
void quitGame() {
    loadScript("Gen/QuitGame");
    exit(0);
}

// Rejects input not in list of acceptable verbs
bool isValidInput() {
    if (find(genActions.begin(), genActions.end(), uInput)
                                      != genActions.end()) {
        return true;
    }

    if (inNego) {
        if (find(negoActions.begin(), negoActions.end(), uInput)
                                            != genActions.end()) {
            return true;
        }
    }
    return false;
}

// Fully abstracted function to call to get user input
void getUserInput() {
    if (!startingNego) setUInput();

    while(true) {
        if (uInput == "quit") quitGame();
        else if (uInput == "help") printHelp();
        else if (uInput == "inventory") player->printInventory();

        if (inNego) {
            currNego->buildValidOffer(economy, bidAccepted);
            if (bidAccepted) return;
        }
        setUInput();
    }
}

// Loads intro scripts in turn
void runIntro() {
    loadScript("0/Intro1"); // Early exposition until discovery by Lepha
    loadScript("0/Intro2"); // Background on you, worldbuilding, your motivation
}

// After intros, these kick off each individual level
void levelStart(int level) {
    if (level == 1) {
        loadScript("1/Level1Start"); // Tutorial opponents
        loadScript("Gen/NegotiationStrategy");
    }
    if (level == 2) loadScript("2/Level2Start");
    if (level == 3) loadScript("3/Level3Start");
}

// After levels, loads the ending scripts
void endStart() { currNego = nullptr; } // Placeholder

// Handles the bool and pointer switches for each level
void runNextLevel(Encounter* levelPointer) {
    levelStart(currLevel); // Intro text for level

    currNego = levelPointer;
    inNego = true;
    bidAccepted = false;

    currNego->beginEncounter();
    startingNego = true;
    getUserInput();

    inNego = false;
    doSkip = false; // Re-engage scripts after an encounter
    currLevel++;
}

void playGame() {

    startScreen(); // Ask to skip intro

    player->fillInventory(); // Give player an inventory before the intro

    runIntro(); // Runs intro

    // Progress to Level 1
    level1 = new Encounter(player, birdSpirits, NEGO1, L1_TURNS);
    runNextLevel(level1);

    // Progress to Level 2
    // level2 = new Encounter(player, rhubarb, NEGO2, L2_TURNS);
    // runNextLevel(level2);

    // Progress to Level 3
    // level3 = new Encounter(player, chamoy, NEGO3, L3_TURNS);
    // runNextLevel(level3);

    // Run ending
    endStart();
}

void fillDictsAndCharacters() {
    createActions();
    createEconomy();
    createGlobalStrings();
    createCharacters(); // including player
}

void releaseMemory() {
    delete level1;
    // delete level2;
    // delete level3;
    delete currNego;


    delete birdSpirits;
    delete player;
}

int main() {

    fillDictsAndCharacters();

    playGame();

    releaseMemory();

    return 0;
}


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

void printCurrentLevelTitle() {
    // Usually to print title at top of new screen
    switch (currLevel) {
        case 0: cout << title0; break;
        case 1: cout << title1; break;
        case 2: cout << title2; break;
        case 3: cout << title3; break;
        case 4: cout << title4; break;
    }
}

// Somewhat pathetic helper function
void clearScreen() {
    const int BIG_NUMBER = 100;
    string clear = "";
    for (int i = 0; i < BIG_NUMBER; i++) {
        clear += "\n";
    }

    cout << clear;
    if (!veryBeginning) printCurrentLevelTitle();
    else veryBeginning = false;
}


// Adds a character 80 times, then a new line to a specified string
void addCharXTimes(char input, int x, string &toChain) {
    toChain += string(x, input) + "\n";
}

// Center text, assuming 80 char wide
void centerText(string title, string &toAdd) {
    int len = title.length();
    int gap = WIDTH - len;

    if (len > WIDTH) return; // Not applicable in this case

    if (gap % 2 == 0) { // If even
        toAdd += string(gap/2, ' ') + title + string(gap/2, ' ') + "\n";
    } else { // If odd, add a space after title, and use decremented gap
        gap--;
        toAdd += string(gap/2, ' ') + title + string(gap/2, ' ') + " \n";
    }
}

// Packages previous two helper functions assuming full screen width
void createTitle(char input, int x, string text,
                 string &toAdd, bool print = true) {
    addCharXTimes(input, WIDTH, toAdd);
    centerText(text, toAdd);
    addCharXTimes(input, WIDTH, toAdd);
    toAdd += "\n";

    if (print) cout << toAdd;
}

// Defines PROMPT_DIVIDER and INVALID_INPUT
void createGlobalStrings() {
    addCharXTimes('-', WIDTH, PROMPT_DIVIDER);

    INVALID_INPUT = "\nInvalid input. Try again.\n";

    bool doPrint = false;
    string text0 = "Prologue: Caught";
    string text1 = "Level 1 / 4: Mosta and Pepita";
    string text2 = "Level 2 / 4: Toto";
    string text3 = "Level 3 / 4: Burro";
    string text4 = "Level 4 / 4: Lepha";

    createTitle('=', WIDTH, text0, title0, doPrint);
    createTitle('=', WIDTH, text1, title1, doPrint);
    createTitle('=', WIDTH, text2, title2, doPrint);
    createTitle('=', WIDTH, text3, title3, doPrint);
    createTitle('=', WIDTH, text4, title4, doPrint);
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
    cout << ">>>> ";
    getline(cin, str);
    lower(str);
    removeWS(str);
}

// Pauses text for user to read and get a breather
void checkpoint() {
    string toAdd = "\n";
    string anyInputText  = "[Press return to continue ";
           anyInputText += "or type \"skip\" to jump to the next section.] ";

    createTitle('-', WIDTH, anyInputText, toAdd);
    getCleanLine(cin, uInput);

    if (uInput == "skip" ) doSkip = true; // Set global flag for future

    clearScreen();
}

// Starts game and asks if player wants to jump to encounter
void startScreen() {
    clearScreen();

    string text = "A DEAL WITH THE SPIRITS";
    string fullTitle = "\n\n";
    createTitle('|', WIDTH, text, fullTitle);

    checkpoint();
}

// Gets simple input to lowercase; callable with specific prompt/target
void setUInput(string prompt = "", string targetInput = "") {

    // Simplify targetInput
    lower(targetInput);
    removeWS(targetInput);

    string output = "";
    createTitle('-', WIDTH, prompt, output);
    getCleanLine(cin, uInput);

    // Only worry about invalidity if targetInput not empty
    if (targetInput != "") {
        // Then if wrong input, note invalidity, and re-print prompt
        while (uInput != targetInput) {
            clearScreen();
            cout << INVALID_INPUT << endl;
            output = ""; // Reset output
            createTitle('-', WIDTH, prompt, output);
            getCleanLine(cin, uInput);

        }
    }
    clearScreen();
}

// Require a specific input from user, as coded by text file
void handleSpecificInput(string line, string &output) {
    cout << output << endl; // Flush output thus far

    const int ASTERISKS_NEEDED = 3;
    int newLength = line.length() - ASTERISKS_NEEDED - ASTERISKS_NEEDED;

    string targetInput = line.substr(ASTERISKS_NEEDED, newLength);
    string prompt = "Type \"" + targetInput + "\"";

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
    clearScreen();
    doSkip = false; // In order to print this final section
    loadScript("QuitGame");
    exit(0);
}

// After intros, these kick off each individual level
void startCurrentLevel() {
    string outText  = to_string(currLevel) + "/Level";
           outText += to_string(currLevel) + "Start";

    // doSkip = false; // Allow scripts at beginning of level
    loadScript(outText);
}

void levelEnd(bool wonEncounter) {
    string outText  = to_string(currLevel) + "/Level" + to_string(currLevel);
    // Load the correct script
    (wonEncounter ? (outText += "Win") : (outText += "Lose"));

    doSkip = false; // Allow scripts after encounter
    loadScript(outText); // Text for victory or loss to be run first

    // If lost and not in tutorial
    if (!wonEncounter && (currLevel > 0)) exit(0);

    currLevel++;
    return;
}

// Runs text before level, runs encounter, and runs text after level
void runNextLevel(Encounter* levelPointer) {
    startCurrentLevel(); // Intro text for level

    bool didNotQuit = levelPointer->runEncounter(wonEncounter);
    if (!didNotQuit) quitGame();

    levelEnd(wonEncounter); // Ending text for level (plus ending if lost)
    wonEncounter = false; // Reset, even though it will be filled later
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

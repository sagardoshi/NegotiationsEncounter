
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
#include <csignal>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;



////////////////////////////////////////////////////////////////////////////////
// Beautify screen printouts for command line play
////////////////////////////////////////////////////////////////////////////////

// Typically to ensure that each dialogue nugget has level title at top
void printCurrentLevelTitle() {
    switch (currLevel) {
        case 0:  cout << title0;   break;
        case 1:  cout << title1;   break;
        case 2:  cout << title2;   break;
        case 3:  cout << title3;   break;
        case 4:  cout << title4;   break;
        case 5:  cout << titleEnd; break;
        default: cout << "";
    }
}

// Pathetic, but functional screen clearer -- big improvement to readability
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

// Helper to print add a character 80 times to a string (usually for titles)
void addCharXTimes(char input, int x, string &toChain) {
    toChain += string(x, input) + "\n";
}

// Assuming 80 char wide screen, centers a string automatically
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

// Packages previous two helper functions assuming 80 char screen width
void createTitle(char input, int x, string text,
                 string &toAdd, bool print = true) {
    addCharXTimes(input, WIDTH, toAdd);
    centerText(text, toAdd);
    addCharXTimes(input, WIDTH, toAdd);
    toAdd += "\n";

    if (print) cout << toAdd;
}


////////////////////////////////////////////////////////////////////////////////
// Handle game quits, whether natural or partway
////////////////////////////////////////////////////////////////////////////////


// Does so for levels, opponents, and player
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

// Re-set CTRL+D at end of game
void resetCtrlD() {
    system("stty eof '^d'"); // re-enable CTRL+D for user's sessions
}

// Helper to immediately end game if quit typed
void quitGame(bool finishedGame = false) {
    clearScreen();
    releaseMemory();
    resetCtrlD();

    if (!finishedGame) {
        string gaveUp  = "You can't handle this anymore. This house, these ";
               gaveUp += "spirits, they are driving you\n";
               gaveUp += "mad! You've gotten this far, and that feels like an ";
               gaveUp += "achievement in itself.\n\n";
               gaveUp += "Perhaps it's the only achievement left to you. Yes, ";
               gaveUp += "perhaps it is.\n\n";
               gaveUp += "You lay back, allow the failure to fill you up, and ";
               gaveUp += "resign yourself to the coming punishment.\n\n";
               gaveUp += "You have quit the game. Please come back and visit ";
               gaveUp += "the spirits again whenever\n";
               gaveUp += "you can! They'll be waiting...\n\n";

        cout << gaveUp;
        exit(0);
    }
}

// Sends to quitGame to handle memory release
void signalHandler(int signum) { quitGame(); }


////////////////////////////////////////////////////////////////////////////////
// Handle user inputs and manage story mode
////////////////////////////////////////////////////////////////////////////////


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
void getCleanUInput() {
    cout << ">>>> ";
    getline(cin, uInput);

    lower(uInput);
    removeWS(uInput);

    if (uInput == "quit") quitGame(); // Anytime there's a new entry, allow quit
}

// Gets simple input to lowercase; callable with specific prompt/target
void setUInput(string prompt = "", string targetInput = "") {

    // Simplify targetInput
    lower(targetInput);
    removeWS(targetInput);

    string output = "";
    createTitle('-', WIDTH, prompt, output);
    getCleanUInput();

    // Only worry about invalidity if targetInput not empty
    if (targetInput != "") {
        // Then if wrong input, note invalidity, and re-print prompt
        while (uInput != targetInput) {
            clearScreen();
            cout << INVALID_INPUT << endl;
            output = ""; // Reset output
            createTitle('-', WIDTH, prompt, output);
            getCleanUInput();
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

// Pauses text for user to read and get a breather
void checkpoint() {
    string toAdd = "\n";
    string anyInputText  = "[Press return to continue ";
           anyInputText += "or type \"skip\" to jump to the next section.] ";

    createTitle('-', WIDTH, anyInputText, toAdd);
    getCleanUInput();

    if (uInput == "skip" ) doSkip = true; // Set global flag for future

    clearScreen();
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


////////////////////////////////////////////////////////////////////////////////
// Actually run the levels and manage encounter mode
////////////////////////////////////////////////////////////////////////////////

// Helper to convert currLevel to an actual level pointer
Encounter* getCurrLevelPointer() {
    switch (currLevel) {
        case 0:  return level0;
        case 1:  return level1;
        case 2:  return level1;
        case 3:  return level1;
        case 4:  return level1;
        case 5:  return level1;
        default: return level0;
    }
}

// Starts game and asks if player wants to jump to encounter
void startScreen() {
    clearScreen();

    string text = "DEALING WITH THE SPIRITS";
    string fullTitle = "\n\n";
    createTitle('|', WIDTH, text, fullTitle);

    checkpoint();
}

// Loads final ending score + script, equivalent for loss or victory
void runEnding() {
    // Should give score at end of most recent level
    float finalScoreFloat = getCurrLevelPointer()->getFinalInvValue();

    // Convert score to string for later use
    string finalScore = player->toPreciseString(finalScoreFloat);

    // Modify to output sentence
    string scoreText = "You started with £100 in loot. You finished with £";
           scoreText += finalScore + ".\n\n";

    doSkip = false; // In order to print the final goodbye
    currLevel = 5; // To get the THE END title
    clearScreen(); // To switch to the THE END screen

    // Printouts
    cout << scoreText;
    loadScript("Gen/Ending"); // Generic text for victory or defeat
    cout << endl << endl;

}

// After intros, these kick off each individual level
void startCurrentLevel() {
    string outText  = to_string(currLevel) + "/Level";
           outText += to_string(currLevel) + "Start";

    // doSkip = false; // Allow scripts at beginning of level
    loadScript(outText);
}

// Called at the end of EACH level, with title change and skips in place
void endCurrentLevel(bool wonEncounter) {
    string outText  = to_string(currLevel) + "/Level" + to_string(currLevel);
    // Load the correct script
    (wonEncounter ? (outText += "Win") : (outText += "Lose"));

    doSkip = false; // Allow scripts after encounter
    loadScript(outText); // Text for victory or loss to be run first

    // If lost encounter and not in tutorial, end game
    if (!wonEncounter && (currLevel > 0)) runEnding();

    return;
}

// Runs text before level, runs encounter, and runs text after level
void runNextLevel(Encounter* levelPointer) {
    startCurrentLevel(); // Intro text for level

    bool didNotQuit = levelPointer->runEncounter(wonEncounter);
    if (!didNotQuit) quitGame();

    endCurrentLevel(wonEncounter); // Ending text for level (+ game over ending)
    wonEncounter = false; // Reset, even though it will be filled later
    currLevel++; // Advance level after complete
}

// Intro and prologue are unique (re:inventory), so handled separately
void runIntro() {
    player->fillInventory(); // Give player an inventory before the intro
    loadScript("Gen/Intro"); // Early exposition until discovery by Lepha

    player->initInventory(); // Clear inventory before tutorial level
    level0 = new Encounter(player, opponent0, currLevel, L0_TURNS, L0_KEY);
    runNextLevel(level0);

    player->fillInventory(); // Refill inventory before Level 1
}


// 9/9) Core game function
void playGame(bool &finished) {

    startScreen(); // Ask to skip intro

    runIntro(); // Runs intro

    // Create levels immediately before running for freshest inventory mapping
    // and accurate current level
    level1 = new Encounter(player, opponent1, currLevel, L1_TURNS, L1_KEY);
    runNextLevel(level1);

    level2 = new Encounter(player, opponent2, currLevel, L2_TURNS, L2_KEY);
    runNextLevel(level2);

    level3 = new Encounter(player, opponent3, currLevel, L3_TURNS, L3_KEY);
    runNextLevel(level3);

    level4 = new Encounter(player, opponent4, currLevel, L4_TURNS, L4_KEY);
    runNextLevel(level4);

    runEnding();

    finished = true;
}


////////////////////////////////////////////////////////////////////////////////
// Handle global variables and memory, and main function
////////////////////////////////////////////////////////////////////////////////

// Defines PROMPT_DIVIDER and INVALID_INPUT
void createGlobalStrings() {
    addCharXTimes('-', WIDTH, PROMPT_DIVIDER);

    INVALID_INPUT = "\nInvalid input. Try again.\n";

    bool doPrint = false;
    string text0 =   "Prologue: Caught";
    string text1 =   "Level 1 / 4: Mosta and Pepita";
    string text2 =   "Level 2 / 4: Toto";
    string text3 =   "Level 3 / 4: Burro";
    string text4 =   "Level 4 / 4: Lepha";
    string textEnd = "The End";

    createTitle('=', WIDTH, text0, title0, doPrint);
    createTitle('=', WIDTH, text1, title1, doPrint);
    createTitle('=', WIDTH, text2, title2, doPrint);
    createTitle('=', WIDTH, text3, title3, doPrint);
    createTitle('=', WIDTH, text4, title4, doPrint);
    createTitle('=', WIDTH, textEnd, titleEnd, doPrint);
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

// Register signals and signal handler
void registerSignals() {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
}

// Re-direct CTRL+D during game
void changeCtrlD() {
    system("stty eof undef"); // disable CTRL+D
    // system("stty intr '^d'"); // redirect to SIGINT
}

// Abstracts above three functions
void setUpGame() {
    changeCtrlD();
    registerSignals();
    createGlobalStrings();
    createCharacters(); // including player BEFORE creating levels
}

// Main function handles memory allocation/deallocation and launches playGame()
int main() {
    bool finishedGame = false;

    setUpGame();
    playGame(finishedGame);
    quitGame(finishedGame);

    return 0;
}

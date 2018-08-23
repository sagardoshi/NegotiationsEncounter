// Ref: http://patorjk.com/software/taag/#p=testall&f=Fun%20Face&t=O

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

void createActions() {
    // Acceptable actions more generally
    genActions.push_back("help");
    genActions.push_back("quit");
    genActions.push_back("inventory");

    // Negotiation actions
    negoActions.push_back("turns");
    negoActions.push_back("propose");
}

void createEconomy() {

    // If we imagine that opponents are now:
    // 0) Mosta the Maribou Stork & Pepita the Pigeon -- spirits of play
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["sunflower seeds"] = 10.0;
    economy["bird spirit key"] = 10.0;


    // 1) centzon totochtin -- spirit of debauchery
    // 2) night river beaver -- spirit of industriousness
    // 3) mama elephant -- spirit of order

}

void createGlobalStrings() {
    PROMPT_DIVIDER =  "----------------------------------------";
    PROMPT_DIVIDER += "----------------------------------------\n";

    INVALID_INPUT = "\n" + PROMPT_DIVIDER + "\nInvalid input. Try again.\n";
}

void createCharacters() {
    player = new PlayerCharacter("You", MODERATE);

    birdSpirits = new Negotiator("Mosta and Pepita", FRIENDLY);
}

void startScreen() {
    uInput = "";

    string title  = "\n\n========================================";
           title +=     "========================================\n";
           title +=     "                            ";
           title +=     "DEALING WITH THE SPIRITS";
           title +=     "                            \n";
           title +=     "========================================";
           title +=     "========================================\n\n";

    string skipIntroText =  "\n" + PROMPT_DIVIDER;
           skipIntroText += "Type \"start\" to start from the beginning.\n";
           skipIntroText += "Type \"skip\" to skip to the first encounter.\n";
           skipIntroText += ">>>> ";


    // Print title and prompt to skip
    cout << title << skipIntroText;
    getline(cin, uInput);

    // Keep reprinting prompt until input valid
    while (uInput != "start" && uInput != "skip") {
        cout << INVALID_INPUT << skipIntroText;
        getline(cin, uInput);
    }
    cout << PROMPT_DIVIDER << "\n";

    if (uInput == "skip") printIntroText = false;

    uInput = ""; // Clear uInput
}

string lower(string anyString) {
    // Convert any capitals to lowercase in place
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

// Pauses text for user to read and get a breather
void checkpoint() {
    string anyInputText = "\n" + PROMPT_DIVIDER + "[Press return to continue.]";

    cout << anyInputText;
    getline(cin, uInput);
    cout << PROMPT_DIVIDER << "\n";
}

void specificInputCheck(string targetInput) {
    string prompt = "Type \"" + targetInput + "\"\n";

    setUInput(prompt, targetInput);

    // Once inventory typed, display it
    if (targetInput == "inventory") player->printInventory();
}

// Print text script saved in txt directory by name
// Also replaces key symbols with frequent functions
void loadScript(string filename) {
    string filepath = "txt/" + filename + ".txt";

    string output = "";
    string line = "";

	ifstream in;
	in.open(filepath);

	while (getline(in, line)) {
        if (line == "***") {
            cout << output;
            checkpoint();
            output = "";
        } else if (line == "***inventory***") {
            cout << output;
            specificInputCheck("inventory");
            output = "";
        } else if (line == "***stop***") {
            cout << output;
            specificInputCheck("stop");
            output = "";
        } else output = output + "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
}

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

void quitGame() {
    loadScript("QuitGame");
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

void runIntro() {
    loadScript("Intro1"); // Early exposition until discovery by Lepha
    loadScript("Intro2"); // Background on you, worldbuilding, your motivation
    loadScript("Intro3"); // Tutorial opponents, leading to level kick-off
}

void levelStart(int level) {
    if (level == 1) {
        loadScript("Level1Start");
        loadScript("NegotiationStrategy");
    }
    if (level == 2) loadScript("Level2Start");
    if (level == 3) loadScript("Level3Start");
}

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
    currLevel++;
}

void playGame() {

    startScreen(); // Ask to skip intro

    player->fillInventory(); // Give player an inventory before the intro

    if (printIntroText) runIntro();



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



//
// void tutorialChoices(string whichChoice) {
//
//     if (whichChoice == "firstTutorialQuestion") {
//         cout << "[1: \"Uh, I'm not really sure...\"]\n";
//         cout << "[2: \"To meet you!\"]\n";
//         cout << "[3: \"It's a long story involving extreme sports and ";
//         cout << "catapults...\"]\n\n";
//
//         setUInput();
//         while (uInput != "1" && uInput != "2" && uInput != "3") {
//             cout << "[Invalid input. Try again.]\n\n";
//             cout << "[1: \"Uh, I'm not really sure...\"]\n";
//             cout << "[2: \"To meet you!\"]\n";
//             cout << "[3: \"It's a long story involving extreme sports and ";
//             cout << "catapults...\"]\n\n";
//
//             setUInput();
//         }
//     }
//
//     if (whichChoice == "secondTutorialQuestion") {
//         cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
//         cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";
//
//         setUInput();
//         while (uInput != "1" && uInput != "2") {
//             cout << "[Invalid input. Try again.]\n\n";
//             cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
//             cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";
//
//             setUInput();
//         }
//     }
//
// }

// // void introStart() {
//
//     loadScript("Intro1");
//
//     tutorialChoices("firstTutorialQuestion");
//     uInput = ""; // Reset uInput
//
//     loadScript("Intro2");
//
//     tutorialChoices("secondTutorialQuestion");
//
//     if (uInput == "1") {
//         cout << "Chamoy rubs her palms together in anticipation.\n\n";
//         cout << "\"Excellent! Just what I was hoping to hear! Let's see ";
//         cout << "if you can be the first human to make it through ";
//         cout << "all of us.\"\n";
//     }
//     if (uInput == "2") {
//         cout << "Chamoy rubs her chin thoughtfully.\n\n";
//
//         cout << "\"Okay, if you say so!\" she says, shrugging her shoulders. ";
//         cout << "\"We have another way of making sure you learn your lesson.\"\n\n";
//
//         checkpoint();
//
//         cout << "She reaches one powerful arm back, and swings it rapidly ";
//         cout << "towards your face. The last thing you see is her sorrowful ";
//         cout << "expression. You feel a sharp pain, and then nothing. ";
//         cout << "The blackness gulps you down.\n\n";
//
//         cout << "You have been killed by Chamoy Tripgor.\n\n";
//
//         cout << "Perhaps you might do better in another life if you don't ";
//         cout << "antagonise the very strong matriarch whose home you ";
//         cout << "invaded.\n\n";
//
//         cout << "Goodbye, and happy haggling!\n\n\n\n";
//         exit(0);
//     }
//
//     uInput = ""; // Reset uInput
//
//     loadScript("Intro3");
//
//     player->fillInventory();
//
//     loadScript("Intro4");
// }

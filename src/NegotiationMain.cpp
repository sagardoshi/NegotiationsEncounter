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
    // name: [default price, how many owned]
    economy["pomegranate"] = 10.0;
    economy["knuckle pads"] = 15.0;
    economy["silverback perfume"] = 10.0;
    economy["ginger cookie"] = 5.0;
    economy["coins"] = 1.0;
    economy["basket"] = 2.0;
    economy["porridge's key"] = 30.0;
    economy["rhubarb's key"] = 40.0;
    economy["chamoy's key"] = 50.0;



}


void checkpoint(bool allKeys = true) {
    uInput = "";
    cout << "\n\n---------------------------\n";
    if (allKeys) cout << "[Press return to continue.]";
    else cout << ">>>> ";
    getline(cin, uInput);
    cout << "---------------------------\n";
}


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
        } else output = output + "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
}



void printHelp() {
    cout << "******************* HELP *******************\n";
    if (inNego) {
        cout << "Add your inventory items by number to your offer\n";
        cout << "propose: type this to send your current offer\n\n";
        cout << "turns: see how many turns you have left\n";
    }

    cout << "inventory: double-check what you currently hold\n";
    cout << "help: see this menu\n";
    cout << "quit: exit the game\n\n";
    cout << "********************************************\n";
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

// Gets simple, unverified user input and converts to lowercase
string getStandardisedInput() {
    // Get standard input (including whitespaces)
    bool anyKeyAcceptable = false;
    checkpoint(anyKeyAcceptable);

    // Convert any capitals to lowercase in place
    int i = 0;
    while (uInput[i]) {
        uInput[i] = tolower(uInput[i]);
        i++;
    }

    return uInput;
}


// Fully abstracted function to call to get user input
void getUserInput() {
    if (!startingNego) string uInput = getStandardisedInput();

    while(true) {
        if (uInput == "quit") quitGame();
        else if (uInput == "help") printHelp();
        else if (uInput == "inventory") player->printInventory();

        if (inNego) {
            currNego->buildValidOffer(economy, bidAccepted);
            if (bidAccepted) return;
        }
        uInput = getStandardisedInput();
    }
}


void tutorialChoices(string whichChoice) {

    if (whichChoice == "firstTutorialQuestion") {
        cout << "[1: \"Uh, I'm not really sure...\"]\n";
        cout << "[2: \"To meet you!\"]\n";
        cout << "[3: \"It's a long story involving extreme sports and ";
        cout << "catapults...\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2" && uInput != "3") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[1: \"Uh, I'm not really sure...\"]\n";
            cout << "[2: \"To meet you!\"]\n";
            cout << "[3: \"It's a long story involving extreme sports and ";
            cout << "catapults...\"]\n\n";

            getStandardisedInput();
        }
    }

    if (whichChoice == "secondTutorialQuestion") {
        cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
        cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
            cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";

            getStandardisedInput();
        }
    }

}

void introStart() {

    loadScript("Intro1");

    tutorialChoices("firstTutorialQuestion");
    uInput = ""; // Reset uInput

    loadScript("Intro2");

    tutorialChoices("secondTutorialQuestion");

    if (uInput == "1") {
        cout << "Chamoy rubs her palms together in anticipation.\n\n";
        cout << "\"Excellent! Just what I was hoping to hear! Let's see ";
        cout << "if you can be the first human to make it through ";
        cout << "all of us.\"\n";
    }
    if (uInput == "2") {
        cout << "Chamoy rubs her chin thoughtfully.\n\n";

        cout << "\"Okay, if you say so!\" she says, shrugging her shoulders. ";
        cout << "\"We have another way of making sure you learn your lesson.\"\n\n";

        checkpoint();

        cout << "She reaches one powerful arm back, and swings it rapidly ";
        cout << "towards your face. The last thing you see is her sorrowful ";
        cout << "expression. You feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Chamoy Tripgor.\n\n";

        cout << "Perhaps you might do better in another life if you don't ";
        cout << "antagonise the very strong matriarch whose home you ";
        cout << "invaded.\n\n";

        cout << "Goodbye, and happy haggling!\n\n\n\n";
        exit(0);
    }

    uInput = ""; // Reset uInput

    loadScript("Intro3");

    player->fillInventory();

    loadScript("Intro4");
}

void levelStart(int level) {
    if (level == 1) {
        loadScript("Level1Start");
        loadScript("HowToNegotiate");
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

    // Run introduction
    introStart();

    // Progress to Level 1
    level1 = new Encounter(player, porridge, NEGO1, L1_TURNS);
    runNextLevel(level1);

    // Progress to Level 2
    level2 = new Encounter(player, rhubarb, NEGO2, L2_TURNS);
    runNextLevel(level2);

    // Progress to Level 3
    level3 = new Encounter(player, chamoy, NEGO3, L3_TURNS);
    runNextLevel(level3);

    // Run ending
    endStart();
}

void fillDictsAndCharacters() {
    createActions();
    createEconomy();

    player = new PlayerCharacter("You", MODERATE);
    porridge = new Negotiator("Porridge", FRIENDLY);
    rhubarb = new Negotiator("Rhubarb", GRUFF);
    chamoy = new Negotiator("Chamoy", GRUFF);
}

void releaseMemory() {
    delete level1;
    delete level2;
    delete level3;
    delete currNego;

    delete porridge;
    delete rhubarb;
    delete chamoy;
    delete player;
}

int main() {

    fillDictsAndCharacters();

    playGame();

    releaseMemory();

    return 0;
}

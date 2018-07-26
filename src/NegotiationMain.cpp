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

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <random>

using namespace std;

// A trivial 0-1 random generator engine from time-based seed
float getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator)/100.0;
}

// Prints instructions
void printHelp() {
    cout << "********************************************\n";
    cout << "******************* MENU *******************\n";
    cout << "********************************************\n\n";
    cout << "[Type 'view' and hit return to read what you ";
    cout << "can see in front of you.]" << endl;

    cout << "[Type 'open' to try to open a door in front ";
    cout << "of you.]" << endl;

    cout << "[Type 'up', 'down', 'left', or 'right' to move ";
    cout << "forwards, backwards, left, or right.]" << endl;

    cout << "[Type 'quit' to quit the game.]" << endl;

    cout << "[Type 'help' to see these instructions.]" << endl;

    cout << "[Type 'play' to return back to the game.]" << endl;
}

// Holds a vector of acceptable verb commands
vector<string> commands() {
    vector<string> acceptableCommands;

    acceptableCommands.push_back("view");
    acceptableCommands.push_back("open");
    acceptableCommands.push_back("up");
    acceptableCommands.push_back("down");
    acceptableCommands.push_back("left");
    acceptableCommands.push_back("right");
    acceptableCommands.push_back("quit");
    acceptableCommands.push_back("help");
    acceptableCommands.push_back("play");

    return acceptableCommands;
}

// Prints ASCII art
void printGorilla() {
    cout << "          .-.      " << endl;
    cout << "        c(O_O)c    " << endl;
    cout << "       ,'.---.`,   " << endl;
    cout << "      / /|_|_|\\ \\ " << endl;
    cout << "      | \\_____/ |  " << endl;
    cout << "      '. `---' .`  " << endl;
    cout << "        `-...-'    " << endl;
}

// Rejects input not in list of acceptable verbs
bool isValidInput(string uInput) {
    vector<string> verbs = commands();

    for (int i = 0; i < verbs.size(); i++) {
        if (uInput == verbs[i]) return true;
    }

    return false;
}

// Gets simple, unverified user input and converts to lowercase
string getLowerCaseInput() {
    string uInput = "";
    cout << ">> ";
    cin >> uInput;
    ::transform(uInput.begin(), uInput.end(), uInput.begin(), ::tolower);
    return uInput;
}

// Fully abstracted function to call to get user input
string getUserInput() {
    string uInput = getLowerCaseInput();

    while(!isValidInput(uInput)) {
        cout << "Invalid input. Try again. Type 'help' for instructions.\n\n";
        uInput = getLowerCaseInput();
    }

    if (uInput == "help") printHelp();
    while (uInput != "play") uInput = getUserInput();

    return uInput;
}

void interact() {
    string uInput = "";

    printGorilla();
    cout << "--Welcome to the world of negotiations! My name is Illa. ";
    cout << "Ook ook.--\n\n";
    cout << "--You may not return to your normal life until you gain the keys ";
    cout << "you need to escape.--\n\n";

    cout << "--How to escape, you ask? Oooooook.--\n\n";

    cout << "--Simple! See that door in front of you? All you have to do is ";
    cout << "go and open it. Go on. Try! OOOOOOOOOK OOOOK.--\n\n";

    cout << "(p.s. if you're stuck, type 'menu')\n\n";

    uInput = getUserInput();

    cout << "[you typed: " << uInput << "]\n\n";;



    cout << "Follow our favourite pasttime! ";
    cout << "Haggle with us! If you can pass the three ";
    cout << "challenges, you'll get the keys to take you ";
    cout << "back home. Don't forget! ";
    cout << "Three keys... (ook)" << endl;
}

int main() {

    // getRandWeight();

    interact();

    cout << "\n===============================" << endl;
    cout << "Testing the Negotiations Engine" << endl;
    cout << "===============================" << endl;
    cout << endl;

    const float FRIENDLY = 0.8;
    const float GRUFF = 0.2;
    const float MODERATE = 0.5;

    Encounter* currentLevel = nullptr;

    Negotiator* friendlyNegotiator = new Negotiator(FRIENDLY);
    Negotiator* gruffNegotiator = new Negotiator(GRUFF);

    PlayerCharacter player(MODERATE);

    ////////// Level 1 //////////
    // Encounter, made up of two issues
    Encounter level1(friendlyNegotiator);
    // Issues
    Issue<float> keyPrice("Key Price", 10.0);
    Issue<bool> friendlifyPotion("Get Friendlify Potion", false);
    // Add issues to encounter
    level1.loadFloatIssue(keyPrice);
    level1.loadBoolIssue(friendlifyPotion);


    level1.printOfferOnTable();


    player.enterNegotiation();

    cout << "===============================" << endl;
    cout << "Closing the Negotiations Engine" << endl;
    cout << "===============================" << endl;
    cout << endl;

    delete friendlyNegotiator;
    delete gruffNegotiator;

    return 0;

}

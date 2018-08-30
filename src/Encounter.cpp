#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Encounter.h"
#include "../inc/Offer.h"

using namespace std;


Encounter::Encounter(PlayerCharacter* pc, Negotiator* opp, int t, float kV) :
                     turns(t), keyValue(kV), player(pc), opponent(opp) {

        offer = new Offer("", 0.0); // Table has no name, no personality
        setInventoryForEncounter(); // Order mapping + saving initial value
}

Encounter::~Encounter()             { delete offer;        }
void Encounter::printOfferOnTable() { offer->printOffer(); }
float Encounter::getFinalInvValue() { return endInvValue;  }

// Get opponent name in all caps
string Encounter::getCapsName() {
    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);
    return opp;
}


// Adds a character 80 times, then a new line to a specified string
void Encounter::addCharXTimes(char input, int x, string &toChain) {
    toChain += string(x, input) + "\n";
}

// Center text, assuming 80 char wide
void Encounter::centerText(string title, string &toAdd) {
    const int WIDTH = 80;
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
void Encounter::createTitle(char input, int x, string text, string &toAdd) {
    const int WIDTH = 80;
    addCharXTimes(input, WIDTH, toAdd);
    centerText(text, toAdd);
    addCharXTimes(input, WIDTH, toAdd);
    toAdd += "\n";

    cout << toAdd;
}

// Converts a string fully to lowercase (in place)
void Encounter::lower(string &anyString) {
    int i = 0;
    while (anyString[i]) {
        anyString[i] = tolower(anyString[i]);
        i++;
    }
}

// Removes whitespaces in string (in place)
void Encounter::removeWS(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
}


// Somewhat pathetic helper function
void Encounter::clearScreen() {
    const int BIG_NUMBER = 100;
    string clear = "";
    for (int i = 0; i < BIG_NUMBER; i++) {
        clear += "\n";
    }
    cout << clear;
}

// Prints turns left
void Encounter::printTurns() {
    string turnsText  = "***** TURNS *****\n";
           turnsText += "You have " + to_string(turns) + " turns left before ";
           turnsText += "the patience of ";
           turnsText += opponent->getName() + " runs out.\n";
           turnsText += "***** TURNS *****\n";

    cout << turnsText << endl;
}

// New inventory order map for each encounter
void Encounter::setInventoryForEncounter() {
    player->invMap.clear(); // Re-map numbering for next encounter
    player->mapPlayerInventory(); // Give each inventory item a number
    opponent->resetGenerosity(); // Ensure no offer at start of encounter

    // Calculate open market value of inventory at start of encounter
    startInvValue = player->getInvValue();
    offerInvValue = 0.0; // init to zero
    endInvValue = 0.0; // init to zero
}

// Verifies that string is a number by iterating through each char of string
bool Encounter::isNum(string &input) {
    string::const_iterator it = input.begin();

    // Advance iterator as long as every character in string is a digit
    while (it != input.end() && isdigit(*it)) ++it;

    // Return true if not empty AND iterator has reached the end
    return ( !input.empty() && (it == input.end()) );
}

// Takes int input and resaves it as appropriate string mapping
void Encounter::remapKeyword(string &keyword) {
    // Keyword should come in as number
    int keyNumber = stoi(keyword);

    // Re-save it as the appropriate string
    map<string, int>::iterator it;
    string currentItem = "";
    int currentOrder;

    for (it = player->invMap.begin(); it != player->invMap.end(); it++) {
        currentItem = it->first;
        currentOrder = it->second;

        // If the invMap leads to the player's entry, save that string mapping
        if (currentOrder == keyNumber) {
            keyword = currentItem;
            return;
        }
    }
}


// Gets simple, unverified user input and converts to lowercase
void Encounter::saveStandardisedInput(string &keyword) {
    const int WIDTH = 80;
    string output = "\n\n";
    string reminder = "Type \"help\" for the menu";

    createTitle('-', WIDTH, reminder, output);

    cout << ">>>> ";
    getline(cin, keyword);
    lower(keyword);
    removeWS(keyword);

    // Non-action int input implies an inv item... replace int with string
    if (isNum(keyword)) remapKeyword(keyword);
}

// Runs full encounter and only returns false if quit was selected
bool Encounter::runEncounter(bool &didWin) {

    string title = "YOU ARE NEGOTIATING WITH " + getCapsName();
    string prop;
    bool forProposal = true;
    bool firstTime = true;

    while (true) {
        if (firstTime) {
            clearScreen(); // To remove incorrect title
            printTitle(title); // first need an immediate print
        }
        offer->printOffer(); // Prints every time after title
        player->printInventory(forProposal, firstTime);
        firstTime = false;

        prop = "";
        saveStandardisedInput(prop);

        clearScreen(); // Critical in this position

        if (!firstTime) printTitle(title); // AFTER the clearScreen for later

        if (prop == "quit") return false;
        else if (prop == "strategy") player->printStrategy();
        else if (prop == "help") player->printHelp();
        else if (prop == "turns") printTurns();
        else if (prop == "") continue; // repeat on empty input
        else if (prop == "cancel") player->takeBackOffer(offer);
        else if (prop == "super") { // Superuser access, not shown in menu
            didWin = true;
            handleEnd(didWin);
            return true;
        }
        else if (prop == "forfeit") {
            didWin = false; // don't check for encounter end... assume loss
            handleEnd(didWin);
            return true;
        }
        else if (prop == "propose" || prop == "proposal") {

            // Save current value, in case it's encounter-ending
            offerInvValue = offer->getInvValue();

            // Check if nego done, and break if so
            if (encounterIsOver(didWin)) {
                handleEnd(didWin);
                return true;
            }
        }

        // Must be a legit item, though the player might not have it
        else if (player->inventory.count(prop)) {
            if (player->inventory[prop]) {
                cout << "You place your " << prop << " on the table.";
                cout << "\n\n";
                player->placeInvObjOnTable(prop, offer);
            } else cout << "You have no " << prop << " to give.\n\n";
        }
        else cout << "You have no such thing. Try again.\n\n";
    }
}

// Prints centered, capitalised title (with opponent name))
void Encounter::printTitle(string textToSay, bool isEnd) {
    string title = "\n";

    addCharXTimes('=', 80, title);
    centerText(textToSay, title);
    addCharXTimes('=', 80, title);
    title += "\n\n";

    cout << title;
}

// Set flags if encounter over; move to next proposal if not yet over
bool Encounter::encounterIsOver(bool &didWin) {
    // Fill win with true or false, depending on acceptance
    didWin = opponent->reactToOffer(offer, keyValue);

    // In case over, remember inventory
    endInvValue = player->getInvValue();

    // If won or if didn't win and no turns left
    if (didWin || (!didWin && turns <= 1)) return true;
    else { // but turns are left, must return items to inventory and continue
        turns--; // Use one turn
        player->takeBackOffer(offer);
        opponent->rejectTerms(turns);
        return false;
    }
}

// Print end title, score, and load correct scripts
void Encounter::handleEnd(bool didWin) {
    clearScreen(); // To remove the "YOU ARE NEGOTIATING VS." title

    string textToSay = "";
    bool isEnd = true;
    if (didWin) textToSay = "YOU WON AGAINST " + getCapsName();
    else textToSay = "YOU LOST AGAINST " + getCapsName();

    printTitle(textToSay, isEnd);
    if (didWin) {
        player->score(startInvValue, offerInvValue, endInvValue);
        opponent->acceptTerms();
    } else player->takeBackOffer(offer); // Returns stuff to you if loss ≠ end
}

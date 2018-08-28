#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Encounter.h"
#include "../inc/Issue.h"
#include "../inc/Offer.h"

using namespace std;


Encounter::Encounter(PlayerCharacter* pc, Negotiator* opp, int t, float kV) :
                     turns(t), keyValue(kV), player(pc), opponent(opp) {
        offer = new Offer(); // Create empty offer
        setInventoryForEncounter(); // Order mapping + saving initial value
}

Encounter::~Encounter()             { delete offer;        }
void Encounter::printOfferOnTable() { offer->printOffer(); }

// Prints turns left
void Encounter::printTurns() {
    string turnsText  = "***** TURNS *****\n";
           turnsText += "You have " + to_string(turns) + " turns left before ";
           turnsText += " the patience of ";
           turnsText += opponent->getName() + " runs out.\n";
           turnsText += "***** TURNS *****\n";

    cout << turnsText << endl;
}

// New inventory order map for each encounter
void Encounter::setInventoryForEncounter() {
    player->invMap.clear(); // Re-map numbering for next encounter
    player->mapPlayerInventory(); // Give each inventory item a number

    // Calculate open market value of inventory at start of encounter
    startInvValue = player->getInvValue(&player->inventory);
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
    // Get standard input (including whitespaces)
    cout << "\n\n--------------------------\n";
    cout << ">>>> ";
    getline(cin, keyword);
    cout << "--------------------------\n\n\n\n";

    // Convert any capitals to lowercase in place
    int i = 0;
    while (keyword[i]) {
        keyword[i] = tolower(keyword[i]);
        i++;
    }

    // Non-action int input implies an inv item... replace int with string
    if (isNum(keyword)) remapKeyword(keyword);
}

void Encounter::runEncounter(map<string, float> econ, bool &didWin) {

    beginEncounter();

    string prop;
    bool forProposal = true;

    while (true) {
        prop = "";
        player->printInventory(forProposal);
        saveStandardisedInput(prop);

        if (prop == "quit") exit(0);
        else if (prop == "inventory") player->printInventory();
        else if (prop == "help") player->printHelp();
        else if (prop == "turns") printTurns();
        else if (prop == "propose") {

            // Print offer and save its value
            offer->printOffer();
            offerInvValue = player->getInvValue(&offer->inventory);

            // Check if nego done, and break if so
            if (encounterIsOver(didWin)) {
                handleEnd(didWin);
                return;
            }
        }

        // Must be a legit item, though the player might not have it
        else if (player->inventory.count(prop)) {
            if (player->inventory[prop]) {
                cout << "You place your " << prop << " on the table.";
                cout << "\n\n";
                player->placeInvObjOnTable(prop, offer);
            } else cout << "You have no " << prop << " to give.\n\n";
            offer->printOffer();
        }
        else cout << "You have no such thing. Try again.\n\n";
    }
}


void Encounter::beginEncounter() {

    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);

    cout << "==========================================================\n";
    cout << "YOU ENTERED A NEGOTIATION WITH " << opp << endl;
    cout << "==========================================================\n\n";

    player->printHelp();
}

void Encounter::printEndEncounter(bool didWin) {

    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);

    cout << "==========================================================\n";
    cout << "YOU " << (didWin ? "WON" : "LOST") << " A ";
    cout << "NEGOTIATION WITH " << opp << endl;
    cout << "==========================================================\n\n";
}

// Set flags if encounter over; move to next proposal if not yet over
bool Encounter::encounterIsOver(bool &didWin) {
    // Fill win with true or false, depending on acceptance
    didWin = opponent->reactToOffer(offer, keyValue);

    // In case over, remember inventory
    endInvValue = player->getInvValue(&player->inventory);

    // If won or if didn't win and no turns left
    if (didWin || (!didWin && turns <= 1)) return true;
    else { // but turns are left, must return items to inventory and continue
        turns--; // Use one turn
        player->clearTable(offer);
        opponent->rejectTerms(turns);
        return false;
    }
}

// Print end title, score, and load correct scripts
void Encounter::handleEnd(bool didWin) {
    printEndEncounter(didWin);
    player->score(startInvValue, offerInvValue, endInvValue);

    if (didWin) opponent->acceptTerms();
    else {
        player->clearTable(offer); // Returns stuff to you if loss ≠ end
        opponent->walkAway();
    }
}

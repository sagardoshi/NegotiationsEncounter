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


Encounter::Encounter(PlayerCharacter* pc, Negotiator* opp, int l, int t) :
                     level(l), turns(t), player(pc), opponent(opp) {

        offer = new Offer(); // Create empty offer
        mapPlayerInventory(); // Give each inventory item a number
        // Calculate open market value of inventory at start of encounter
        startInvValue = player->getInvValue(&player->inventory);
}

Encounter::~Encounter()             { delete offer;                       }
int Encounter::getLevel()           { return level;                       }
void Encounter::printTurns()        { cout << turns << " turns left\n\n"; }
void Encounter::printOfferOnTable() { offer->printOffer();                }

// Goes through every item player has in an encounter, and assigns a number
void Encounter::mapPlayerInventory() {
    map<string, int>::iterator it;
    string item = "";
    int itemOrder = 1;
    int amount = 0;


    for (it = player->inventory.begin(); it != player->inventory.end(); it++) {
        item = it->first;
        amount = it->second;

        if (amount > 0) {
            player->invMap[item] = itemOrder;
            itemOrder++;
        }
    }
}

// Gets simple, unverified user input and converts to lowercase
string Encounter::saveStandardisedInput(string keyword) {
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

    // TODO: FIX THIS TO BE GENERAL
    if (keyword == "1") keyword = "burn relief ointment";
    if (keyword == "2") keyword = "carved walking cane";
    if (keyword == "3") keyword = "sunflower seeds packet";

    // if (keyword == "1") keyword = "pomegranate";
    // if (keyword == "2") keyword = "knuckle pads";
    // if (keyword == "3") keyword = "silverback perfume";
    // if (keyword == "4") keyword = "ginger cookie";
    // if (keyword == "5") keyword = "coins";
    // if (keyword == "6") keyword = "basket";

    return keyword;
}


void Encounter::buildValidOffer(map<string, float> econ, bool &didWin) {

    string prop;
    bool forProposal = true;
    bool negoOver = false;

    while (true) {
        prop = "";
        player->printInventory(forProposal);
        prop = saveStandardisedInput(prop);

        if (prop == "quit") exit(0);
        else if (prop == "inventory") player->printInventory();
        else if (prop == "help") player->printHelp();
        else if (prop == "turns") printTurns();
        else if (prop == "propose") {

            // Print offer and save its value
            offer->printOffer();
            offerInvValue = player->getInvValue(&offer->offerInv);

            // Check if nego done, and break if so
            checkEndEncounter(didWin, negoOver);
            if (negoOver) return;
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
    cout << "YOU " << (didWin ? "PASSED" : "LOST") << " A ";
    cout << "NEGOTIATION WITH " << opp << endl;
    cout << "==========================================================\n\n";
}

void Encounter::checkEndEncounter(bool &didWin, bool &negoOver) {
    // Fill win with true or false, depending on acceptance
    didWin = opponent->reactToOffer(offer);

    // In case over, remember inventory
    endInvValue = player->getInvValue(&player->inventory);

    // If won
    if (didWin) {
        negoOver = true;
        printEndEncounter(didWin);
        player->score(startInvValue, offerInvValue, endInvValue);
        opponent->acceptTerms();
    }
    // If offer rejected
    else {
        // but no turns left
        if (!turns) {
            negoOver = true;
            printEndEncounter(didWin);
            player->score(startInvValue, offerInvValue, endInvValue);
            opponent->walkAway();
        }
        // but turns are left, must return items to inventory first
        else {
            player->clearTable(offer);
            turns--; // Use one turn
            opponent->rejectTerms(turns);

        }
    }
}

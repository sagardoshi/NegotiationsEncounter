#include <iostream>
#include <string>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a),
                                                      levelReached(0) {

    inventory["pomegranate"] = 0;
    inventory["knuckle pads"] = 0;
    inventory["silverback perfume"] = 0;
    inventory["ginger cookie"] = 0;
    inventory["coins"] = 0;
    inventory["basket"] = 0;
}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

void PlayerCharacter::fillInventory() {
    inventory["pomegranate"] = 1;
    inventory["knuckle pads"] = 1;
    inventory["silverback perfume"] = 1;
    inventory["ginger cookie"] = 4;
    inventory["coins"] = 30;
    inventory["basket"] = 1;
}

void PlayerCharacter::addToInventory(string itemName, int quantity) {
    inventory[itemName] += quantity;
}

void PlayerCharacter::removeFromInventory(string itemName) {
    inventory[itemName] = 0;
}

void PlayerCharacter::placeInvObjOnTable(string itemName, Offer* onTable) { inventory[itemName]--;
    onTable->addObjToTable(itemName);
}

void PlayerCharacter::clearTable(Offer* table) {
    map<string, int>::iterator it;
    for (it = table->inventory.begin(); it != table->inventory.end(); it++) {
        if (it->second > 0) {
            addToInventory(it->first, it->second);
            table->inventory[it->first] = 0;
        }
    }
}

void PlayerCharacter::printHelp() {
    cout << "******************* GENERAL HELP *******************\n";
    cout << "[help: see this menu]\n";
    cout << "[quit: exit the game]\n";
    cout << "[inventory: see what you currently hold]\n";
    cout << "[negotiate: to start a level (when prompted)]\n";
    cout << "****************************************************\n\n";

    cout << "**************** DURING NEGOTIATION ****************\n";
    cout << "[turns: see how many turns you have left]\n";
    cout << "[issues: see what you're negotiating over]\n";
    cout << "[propose offer: use a turn to build your own offer]\n";
    cout << "****************************************************\n\n";
}

void PlayerCharacter::printInventory(bool forProposal) {

    if (!inventory["pomegranate"] &&
        !inventory["knuckle pads"] &&
        !inventory["silverback perfume"] &&
        !inventory["ginger cookie"] &&
        !inventory["coins"] &&
        !inventory["basket"]) {
        cout << "***************** INVENTORY ****************\n";
        cout << "You have nothing in your inventory.\n\n";
        cout << "********************************************\n\n";
        return;
    }

    if (forProposal) {
        cout << "***************** PROPOSE OFFER ****************\n";
        cout << "[To build an offer, add your items by number, one by one.]\n";
        cout << "[Type \"done\" when you don't wish to add any more.]\n\n";
    } else cout << "***************** INVENTORY ****************\n";
    if (inventory["pomegranate"]) {
        cout << (forProposal ? "[1: " : "[") << "Pomegranate, ";
        cout << inventory["pomegranate"] << " owned]" << endl;
    }
    if (inventory["knuckle pads"]) {
        cout << (forProposal ? "[2: " : "[") << "Knuckle Pads, ";
        cout << inventory["knuckle pads"] << " owned]" << endl;
    }
    if (inventory["silverback perfume"]) {
        cout << (forProposal ? "[3: " : "[") << "Silverback Perfume, ";
        cout << inventory["silverback perfume"] << " owned]" << endl;
    }
    if (inventory["ginger cookie"]) {
        cout << (forProposal ? "[4: " : "[") << "Ginger Cookie";
        cout << (inventory["ginger cookie"] > 1 ? "s" : "");
        cout << ", " << inventory["ginger cookie"] << " owned]" << endl;

    }
    if (inventory["coins"]) {
        cout << (forProposal ? "[5: " : "[") << "Coin";
        cout << (inventory["coins"] > 1 ? "s" : "");
        cout << ", " << inventory["coins"] << " owned]" << endl;
    }
    if (inventory["basket"]) {
        cout << (forProposal ? "[6: " : "[") << "Basket, ";
        cout << inventory["basket"] << " owned]" << endl;
    }
    cout << "************************************************\n\n";


}

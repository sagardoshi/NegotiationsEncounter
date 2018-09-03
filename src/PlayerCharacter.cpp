#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a) {
    initInventory();
}

void PlayerCharacter::initInventory() {
    inventory["burn relief ointment"] = 0;
    inventory["carved walking cane"] = 0;
    inventory["sunflower seeds packet"] = 0;
    inventory["pulque flask"] = 0;
    inventory["paint canister"] = 0;
    inventory["morning headache tonic"] = 0;
    inventory["long earmuffs"] = 0;
    inventory["wood varnish bottle"] = 0;
    inventory["waterproof wax jar"] = 0;
    inventory["loose leaf sencha tea"] = 0;
    inventory["vinegar disinfectant"] = 0;
    inventory["personal black trousers"] = 1;
    inventory["personal black tunic"] = 1;
}

// How many of each item you carry
void PlayerCharacter::fillInventory() {
    inventory["burn relief ointment"] = 1;
    inventory["carved walking cane"] = 1;
    inventory["sunflower seeds packet"] = 1;
    inventory["pulque flask"] = 2;
    inventory["paint canister"] = 3;
    inventory["morning headache tonic"] = 1;
    inventory["long earmuffs"] = 1;
    inventory["wood varnish bottle"] = 2;
    inventory["waterproof wax jar"] = 1;
    inventory["loose leaf sencha tea"] = 2;
    inventory["vinegar disinfectant"] = 4;
    inventory["personal black trousers"] = 1; // these were already added
    inventory["personal black tunic"] = 1; // but repeating to avoid mistakes
}


bool PlayerCharacter::knowsOfItem(string item) {
    return ((economy.count(item) > 0) ? true : false); // If item exists
}

bool PlayerCharacter::hasItem(string item) {
    return ((inventory[item] > 0) ? true : false); // Player has at least one
}

void PlayerCharacter::placeItemOnTable(string itemName, Negotiator* offer) {
    inventory[itemName]--; // Take item out of bag
    offer->inventory[itemName]++; // Place it on table
}

void PlayerCharacter::takeBackOffer(Negotiator* offer) {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;

    for (it = offer->inventory.begin(); it != offer->inventory.end(); it++) {
        item = it->first;
        amount = it->second;
        if (amount > 0) {
            inventory[item] += amount;
            offer->inventory[item] = 0;
        }
    }
}

#include "../inc/PlayerCharacter.h"


PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a) {
    initInventory();
}

////////////////////////////////////////////////////////////////////////////////
// Inventory Methods
////////////////////////////////////////////////////////////////////////////////

void PlayerCharacter::fillPreferences() {
    prefs["personal black trousers"] = 1.0;
    prefs["personal black tunic"] = 1.0;
    prefs["burn relief ointment"] = 1.0;
    prefs["carved walking cane"] = 1.0;
    prefs["sunflower seeds packet"] = 1.0;
    prefs["pulque flask"] = 1.0;
    prefs["paint canister"] = 1.0;
    prefs["morning headache tonic"] = 1.0;
    prefs["long earmuffs"] = 1.0;
    prefs["wood varnish bottle"] = 1.0;
    prefs["waterproof wax jar"] = 1.0;
    prefs["loose leaf sencha tea"] = 1.0;
    prefs["vinegar disinfectant"] = 1.0;
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

// How many of each item player gets at start
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


////////////////////////////////////////////////////////////////////////////////
// Trading between inventory and table
////////////////////////////////////////////////////////////////////////////////

void PlayerCharacter::placeItemOnTable(string itemName, Inventory* table) {
    inventory[itemName]--;        // Take item out of bag
    table->inventory[itemName]++; // Place it on table
}

void PlayerCharacter::takeBackOffer(Inventory* table) {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;

    for (it = table->inventory.begin(); it != table->inventory.end(); it++) {
        item = it->first;
        amount = it->second;
        if (amount > 0) {
            table->inventory[item] = 0; // Take item(s) off table
            inventory[item] += amount;  // Put item(s) back into bag

        }
    }
}

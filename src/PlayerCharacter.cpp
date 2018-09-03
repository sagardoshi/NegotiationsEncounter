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


float PlayerCharacter::getInvValue() {
    map<string, int>::iterator it;
    string itemName = "";
    float baseValue = 0.0, totalValue = 0.0;
    int quantity = 0;

    // Multiply each inventory item's base value by amount held and sum up
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        quantity = it->second;
        baseValue = economy[itemName];
        totalValue += (baseValue * quantity);
    }
    return totalValue;
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

void PlayerCharacter::printInventory(map<string, int>* mapPtr) {

    // Print appropriate header
    string header = "----- YOUR LOOT -----\n";
    string footer = header;

    cout << header;

    // Local vars for iterator
    map<string, int>::iterator it;

    const int MAX_ITEM_LEN = 23;
    string itemName = "";
    string itemText = "";

    int numItemsHeld = 0;
    int amount = 0;
    string amountText = "";

    int itemOrder = 0;
    string itemOrderText = "";

    int itemValue = 0;
    string itemValueText = "";


    // Depending on whether within proposal, print list of items one by one
    // Skip items with <= 0 inventory
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;
        amountText = to_string(amount);
        itemOrder = ((mapPtr == NULL) ? 0 : (*mapPtr)[itemName]);
        itemOrderText = to_string(itemOrder);
        itemValue = economy[itemName];
        itemValueText = to_string(itemValue);


        if (amount > 0) {
            numItemsHeld += amount;

            // First add itemOrder, if appropriate, keeping aligned
            if (itemOrder) {
                if (itemOrder < 10) itemText += " " + itemOrderText + ": ";
                else itemText += itemOrderText + ": ";
            }

            // Then add itemName with extra spaces at end for alignment
            itemText += itemName;
            int gap = MAX_ITEM_LEN - itemName.length();
            if (gap > 0) {
                for (int i = 0; i < gap; i++) itemText += " ";
            }

            itemText += " ... ";

            // Next add amount, with extra spaces for alignment
            if (amount < 10) itemText += " " + amountText + " owned";
            else itemText += amountText + " owned";

            itemText += " ... ";

            // Finally add itemValue, again with spaces for alignment
            if (itemValue < 10) itemText += " £" + itemValueText;
            else itemText += "£" + itemValueText;
            itemText += " base market value per unit";

            itemText += "\n";

            // At last, flush out itemText, empty it, and go on to next
            cout << itemText;
            itemText = "";
        }

    }

    string total = "";
    // Print totals or empty notice
    if (numItemsHeld) {
        total += "\n Loot Market Value: ";
        total += "£" + toPreciseString(getInvValue()) + "\n";
    } else footer = "[Empty]\n" + footer;

    cout << total << footer;
}

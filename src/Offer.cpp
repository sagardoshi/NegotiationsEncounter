#include "../inc/Offer.h"
#include "../inc/Issue.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer() {
    inventory["black trousers"] = 0;
    inventory["black tunic"] = 0;
    inventory["burn relief ointment"] = 0;
    inventory["carved walking cane"] = 0;
    inventory["packet of sunflower seeds"] = 0;
    inventory["pulque bottle"] = 0;
    inventory["paint jar"] = 0;
    inventory["morning headache tonic"] = 0;
    inventory["long ear warmers"] = 0;
}

void Offer::addObjToTable(string itemName) { inventory[itemName]++; }

int Offer::inventoryCount() {
    map<string, int>::iterator it;
    int totalItems = 0;
    for (it = inventory.begin(); it != inventory.end(); it++) {
        totalItems += it->second;
    }
    return totalItems;
}


void Offer::printOffer() {
    string borderText = "***** THE TABLE *****\n";
    cout << borderText;

    // Local vars for iterator
    map<string, int>::iterator it;
    int numItemsHeld = 0;
    string itemName = "";
    string listItemText = "";
    int amount = 0;

    // For all items held with >0 quantity, print amount and name on table
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;

        if (amount > 0) {
            numItemsHeld += amount;

            listItemText += to_string(amount) + " " + itemName;
            listItemText += (amount > 1 ? "s\n" : "\n");

            cout << listItemText; // Flush out item and go on to the next one
        }
        listItemText = "";

    }

    // Prepend footer if necessary with zero inventory notice
    if (!numItemsHeld) {
        borderText = "You have placed nothing on the table.\n" + borderText;
    }
    cout << borderText << endl;

}

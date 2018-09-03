#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer(string n, float a) : Negotiator(n, a) {}

int Offer::inventoryCount() {
    map<string, int>::iterator it;
    int totalItems = 0;
    for (it = inventory.begin(); it != inventory.end(); it++) {
        totalItems += it->second;
    }
    return totalItems;
}

void Offer::printOffer() {

    string borderText = "----- THE TABLE -----\n";
    cout << borderText;


    // Local vars for iterator
    map<string, int>::iterator it;
    const int MAX_ITEM_LEN = 23;
    int numItemsHeld = 0;
    string itemName = "";
    string itemText = "";
    int amount = 0;
    int itemValue = 0;

    // For all items held with >0 quantity, print amount and name on table
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;
        itemValue = economy[itemName];

        if (amount > 0) {
            numItemsHeld += amount;

            // First print name
            itemText += "    " + itemName;
            int gap = MAX_ITEM_LEN - itemName.length();
            if (gap > 0) {
                for (int i = 0; i < gap; i++) itemText += " ";
            }

            itemText += " ... ";

            // Then quantity on the table
            itemText += " " + to_string(amount) + " given";

            itemText += " ... ";

            // Then market value per item
            itemText += (itemValue < 10 ? " £" : "£");
            itemText += to_string(itemValue) + " base market value per unit";

            itemText += "\n";

            cout << itemText; // Flush out item and go on to the next one
        }
        itemText = "";

    }


    string total = "";
    // Print totals or empty notice
    if (numItemsHeld) {
        total += "\nOffer Market Value: ";
        total += "£" + toPreciseString(getInvValue()) + "\n";
    } else borderText = "[Empty]\n" + borderText;

    cout << total << borderText << endl;

}

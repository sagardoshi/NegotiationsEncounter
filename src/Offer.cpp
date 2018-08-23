#include "../inc/Offer.h"
#include "../inc/Issue.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer() {
    offerInv["burn relief ointment"] = 0;
    offerInv["carved walking cane"] = 0;
    offerInv["sunflower seeds packet"] = 0;
    offerInv["bird spirit key"] = 0;
}

void Offer::addObjToTable(string itemName) { offerInv[itemName]++; }

int Offer::inventoryCount() {
    map<string, int>::iterator it;
    int totalItems = 0;
    for (it = offerInv.begin(); it != offerInv.end(); it++) {
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
    for (it = offerInv.begin(); it != offerInv.end(); it++) {
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

//
// void Offer::printOffer() {
//
//     if (inventoryCount() == 0) cout << "There is nothing on the table.\n\n";
//     else {
//         cout << "***************** ON THE TABLE *****************\n";
//         if (offerInv["pomegranate"]) {
//             cout << "-- " << offerInv["pomegranate"] << " pomegranate\n";
//         }
//         if (offerInv["knuckle pads"]) {
//             cout << "-- " << offerInv["knuckle pads"] << " set of knuckle pads\n";
//         }
//         if (offerInv["silverback perfume"]) {
//             cout << "-- " << offerInv["silverback perfume"] << " bottle of silverback perfume\n";
//         }
//         if (offerInv["ginger cookie"]) {
//             cout << "-- " << offerInv["ginger cookie"] << " ginger cookie";
//             cout << (offerInv["ginger cookie"] > 1 ? "s" : "") << endl;
//         }
//         if (offerInv["coins"]) {
//             cout << "-- " << offerInv["coins"] << " coin";
//             cout << (offerInv["coins"] == 1 ? "" : "s") << endl;
//         }
//         if (offerInv["basket"]) {
//             cout << "-- and 1 basket to put it all in." << endl;
//         }
//         cout << "************************************************\n\n";
//     }
// }

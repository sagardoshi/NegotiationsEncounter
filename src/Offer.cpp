#include "../inc/Offer.h"
#include "../inc/Issue.h"

#include <iostream>
#include <string>
#include <map>
// #include <numeric>
// #include <utility>

using namespace std;

// Init required issue numbers to 0
Offer::Offer() {
    inventory["pomegranate"] = 0;
    inventory["knuckle pads"] = 0;
    inventory["silverback perfume"] = 0;
    inventory["ginger cookie"] = 0;
    inventory["coins"] = 0;
    inventory["basket"] = 0;
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

    if (inventoryCount() == 0) cout << "There is nothing on the table.\n\n";
    else {
        cout << "***************** ON THE TABLE *****************\n";
        if (inventory["pomegranate"]) {
            cout << "-- " << inventory["pomegranate"] << " pomegranate\n";
        }
        if (inventory["knuckle pads"]) {
            cout << "-- " << inventory["knuckle pads"] << " set of knuckle pads\n";
        }
        if (inventory["silverback perfume"]) {
            cout << "-- " << inventory["silverback perfume"] << " bottle of silverback perfume\n";
        }
        if (inventory["ginger cookie"]) {
            cout << "-- " << inventory["ginger cookie"] << " ginger cookie";
            cout << (inventory["ginger cookie"] > 1 ? "s" : "") << endl;
        }
        if (inventory["coins"]) {
            cout << "-- " << inventory["coins"] << " coin";
            cout << (inventory["coins"] == 1 ? "" : "s") << endl;
        }
        if (inventory["basket"]) {
            cout << "-- and 1 basket to put it all in." << endl;
        }
        cout << "************************************************\n\n";
    }
}

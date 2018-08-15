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
    offerInv["pomegranate"] = 0;
    offerInv["knuckle pads"] = 0;
    offerInv["silverback perfume"] = 0;
    offerInv["ginger cookie"] = 0;
    offerInv["coins"] = 0;
    offerInv["basket"] = 0;
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

    if (inventoryCount() == 0) cout << "There is nothing on the table.\n\n";
    else {
        cout << "***************** ON THE TABLE *****************\n";
        if (offerInv["pomegranate"]) {
            cout << "-- " << offerInv["pomegranate"] << " pomegranate\n";
        }
        if (offerInv["knuckle pads"]) {
            cout << "-- " << offerInv["knuckle pads"] << " set of knuckle pads\n";
        }
        if (offerInv["silverback perfume"]) {
            cout << "-- " << offerInv["silverback perfume"] << " bottle of silverback perfume\n";
        }
        if (offerInv["ginger cookie"]) {
            cout << "-- " << offerInv["ginger cookie"] << " ginger cookie";
            cout << (offerInv["ginger cookie"] > 1 ? "s" : "") << endl;
        }
        if (offerInv["coins"]) {
            cout << "-- " << offerInv["coins"] << " coin";
            cout << (offerInv["coins"] == 1 ? "" : "s") << endl;
        }
        if (offerInv["basket"]) {
            cout << "-- and 1 basket to put it all in." << endl;
        }
        cout << "************************************************\n\n";
    }
}

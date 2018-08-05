#include "../inc/Offer.h"
#include "../inc/Issue.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer() : requiredFloatIssues(0), requiredIntIssues(0) {
    inventory["pomegranate"] = 0;
    inventory["knuckle pads"] = 0;
    inventory["silverback perfume"] = 0;
    inventory["ginger cookie"] = 0;
    inventory["coins"] = 0;
    inventory["basket"] = 0;
}


int Offer::countIssuesInOffer() {
    return (floatIssues.size() + intIssues.size());
}

void Offer::addObjToTable(string itemName) {
    inventory[itemName]++;
}



void Offer::printOffer() {
    cout << "On the table, you are offering: \n";
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
        cout << "-- and 1 basket to put it all in.\n";
    }

    cout << "\n\n";
}




// Bump required issue number for each new issue
void Offer::loadFloatIssue(Issue<float> issueToAdd) {
    floatIssues.push_back(issueToAdd);
    requiredFloatIssues++;
}

// Bump required issue number for each new issue
void Offer::loadIntIssue(Issue<int> issueToAdd) {
    intIssues.push_back(issueToAdd);
    requiredIntIssues++;
}

int Offer::getReqdFloatIssueCount() { return requiredFloatIssues; }
int Offer::getReqdIntIssueCount()   { return requiredIntIssues;   }

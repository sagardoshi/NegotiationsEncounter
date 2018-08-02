#include "../inc/Offer.h"
#include "../inc/Issue.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer() : requiredFloatIssues(0), requiredIntIssues(0) {
    inventory["pomegranate"] = make_pair(10, 0);
    inventory["knuckle pads"] = make_pair(15, 0);
    inventory["silverback perfume"] = make_pair(10, 0);
    inventory["ginger cookie"] = make_pair(1, 0);
    inventory["coin purse"] = make_pair(5, 0);
    inventory["basket"] = make_pair(2, 0);
}


int Offer::countIssuesInOffer() {
    return (floatIssues.size() + intIssues.size());
}

void Offer::addObjToTable(string itemName) {
    inventory[itemName].second++;
}


void Offer::printOffer() {
    cout << "On the table, you are offering: \n";
    if (inventory["pomegranate"].second) {
        cout << "--" << inventory["pomegranate"].second << " pomegranate\n";
    }
    if (inventory["knuckle pads"].second) {
        cout << "--" << inventory["knuckle pads"].second << " set of knuckle pads\n";
    }
    if (inventory["silverback perfume"].second) {
        cout << "--" << inventory["silverback perfume"].second << " bottle of silverback perfume\n";
    }
    if (inventory["ginger cookie"].second) {
        cout << "--" << inventory["ginger cookie"].second << " ginger cookie";
        cout << (inventory["ginger cookie"].second > 1 ? "s" : "") << endl;
    }
    if (inventory["coin purse"].second) {
        cout << "--1 coin purse with " << inventory["coin purse"].second;
        cout << " coin" << (inventory["coin purse"].second == 1 ? "" : "s");
        cout << endl;
    }
    if (inventory["basket"].second) {
        cout << "--and 1 basket to put it all in.\n";
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

#include <iostream>
#include <string>

#include "../inc/Encounter.h"
#include "../inc/Negotiator.h"
#include "../inc/Issue.h"
#include "../inc/Offer.h"

using namespace std;


Encounter::Encounter(Negotiator* opp) :
                     opponent(opp) {
    offerOnTheTable = new Offer(); // Create empty offer
}

Encounter::~Encounter() {
    delete offerOnTheTable;
}


void Encounter::loadFloatIssue(Issue<float> issueToAdd) {
    floatIssues.push_back(issueToAdd);
    offerOnTheTable->loadFloatIssue(issueToAdd);
}

void Encounter::loadBoolIssue(Issue<bool> issueToAdd) {
    boolIssues.push_back(issueToAdd);
    offerOnTheTable->loadBoolIssue(issueToAdd);
}
void Encounter::loadIntIssue(Issue<int> issueToAdd) {
    intIssues.push_back(issueToAdd);
    offerOnTheTable->loadIntIssue(issueToAdd);
}


int Encounter::countIssuesInEncounter() {
    return (floatIssues.size() + boolIssues.size() + intIssues.size());
}

void Encounter::printOfferOnTable() {
    offerOnTheTable->printOffer();
}


bool Encounter::validateOffer() {
    bool isValid = true;

    if (!offerOnTheTable ||
        offerOnTheTable->countIssuesInOffer() != this->countIssuesInEncounter()) isValid = false;

    if (!offerOnTheTable) {
        cout << "There is no offer on the table." << endl;
    }
    else {
        cout << "There is an offer on the table, but I don't have ";
        cout << "validation software set up yet." << endl;
    }

    return isValid;
}

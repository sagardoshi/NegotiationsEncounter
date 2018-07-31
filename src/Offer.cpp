#include <iostream>
#include <string>

#include "../inc/Offer.h"
#include "../inc/Issue.h"

using namespace std;

// Init required issue numbers to 0
Offer::Offer() : requiredFloatIssues(0), requiredIntIssues(0) {}


int Offer::countIssuesInOffer() {
    return (floatIssues.size() + intIssues.size());
}


void Offer::printOffer() {
    for (int i = 0; i < floatIssues.size(); i++) floatIssues[i].printIssue();
    for (int i = 0; i < intIssues.size(); i++) intIssues[i].printIssue();
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

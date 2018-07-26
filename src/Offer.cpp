#include <iostream>
#include <string>

#include "../inc/Offer.h"
#include "../inc/Issue.h"

using namespace std;

Offer::Offer() {}

int Offer::countIssuesInOffer() {
    return (floatIssues.size() + boolIssues.size() + intIssues.size());
}


void Offer::printOffer() {
    for (int i = 0; i < this->floatIssues.size(); i++) {
        this->floatIssues[i].printIssue();
    }

    for (int i = 0; i < this->boolIssues.size(); i++) {
        this->boolIssues[i].printIssue();
    }

    for (int i = 0; i < this->intIssues.size(); i++) {
        this->intIssues[i].printIssue();
    }


}


void Offer::loadFloatIssue(Issue<float> issueToAdd) {
    floatIssues.push_back(issueToAdd);
}

void Offer::loadBoolIssue(Issue<bool> issueToAdd) {
    boolIssues.push_back(issueToAdd);
}
void Offer::loadIntIssue(Issue<int> issueToAdd) {
    intIssues.push_back(issueToAdd);
}

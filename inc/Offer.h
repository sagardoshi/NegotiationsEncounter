#ifndef OFFER_H
#define OFFER_H

#include "Issue.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

class Offer {
private:
    int requiredFloatIssues;
    int requiredIntIssues;

    vector< Issue<float> > floatIssues;
    vector< Issue<int> > intIssues;

public:
    Offer();

    map<string, int> inventory;


    void addObjToTable(string);

    int inventoryCount();
    int issueCount();
    void printOffer();

    int getReqdFloatIssueCount();
    int getReqdIntIssueCount();

    void loadFloatIssue(Issue<float>);
    void loadIntIssue(Issue<int>);
};

#endif

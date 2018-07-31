#ifndef OFFER_H
#define OFFER_H

#include "Issue.h"

#include <vector>
#include <string>

using namespace std;

class Offer {
private:
    int requiredFloatIssues;
    int requiredIntIssues;

    vector< Issue<float> > floatIssues;
    vector< Issue<int> > intIssues;

public:
    Offer();

    int countIssuesInOffer();
    void printOffer();

    int getReqdFloatIssueCount();
    int getReqdIntIssueCount();

    void loadFloatIssue(Issue<float>);
    void loadIntIssue(Issue<int>);
};

#endif

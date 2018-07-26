#ifndef OFFER_H
#define OFFER_H

#include "Issue.h"

#include <vector>
#include <string>

using namespace std;

class Offer {
private:
    vector< Issue<float> > floatIssues;
    vector< Issue<bool> > boolIssues;
    vector< Issue<int> > intIssues;

public:
    Offer();

    int countIssuesInOffer();
    void printOffer();


    void loadFloatIssue(Issue<float>);
    void loadBoolIssue(Issue<bool>);
    void loadIntIssue(Issue<int>);
};

#endif

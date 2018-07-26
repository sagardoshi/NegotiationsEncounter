#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "Offer.h"
#include "Negotiator.h"
#include "Issue.h"

#include <string>
#include <vector>


class Encounter {

private:
    std::string topic;
    // int level;
    Negotiator* opponent;
    Offer* offerOnTheTable;

    std::vector< Issue<float> > floatIssues;
    std::vector< Issue<bool> > boolIssues;
    std::vector< Issue<int> > intIssues;

public:
    Encounter(Negotiator*);
    ~Encounter();

    // Only to be used at initial loading time
        void loadFloatIssue(Issue<float>);
        void loadBoolIssue(Issue<bool>);
        void loadIntIssue(Issue<int>);

    bool validateOffer();
    int countIssuesInEncounter();
    void printOfferOnTable();

};

#endif

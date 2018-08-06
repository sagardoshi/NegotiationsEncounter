#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "Offer.h"
#include "PlayerCharacter.h"
#include "Negotiator.h"
#include "Issue.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class Encounter {

private:
    string topic;
    int level;
    int turns;

    PlayerCharacter* player;
    Negotiator* opponent;
    Offer* offer;

    vector< Issue<float> > floatIssues;
    vector< Issue<int> > intIssues;

public:
    Encounter(PlayerCharacter*, Negotiator*, int, int);
    ~Encounter();

    // Only to be used at initial loading time
    void loadFloatIssue(Issue<float>);
    void loadIntIssue(Issue<int>);


    int getLevel();
    void useOneTurn();

    string saveStandardisedInput(string keyword);

    void buildValidOffer(map<string, int>);
    int issueCount();

    void printTurns();
    void printOfferOnTable();
    void printIssues();

    void beginEncounter();

};

#endif

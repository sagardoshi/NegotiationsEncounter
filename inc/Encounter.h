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
    bool isPCTurn;

    PlayerCharacter* player;
    Negotiator* opponent;
    Offer* offerOnTheTable;

    vector< Issue<float> > floatIssues;
    vector< Issue<int> > intIssues;

public:
    Encounter(PlayerCharacter*, Negotiator*, int, int);
    ~Encounter();

    int getLevel();
    void useOneTurn();

    string getStandardisedInput(string keyword);

    // Only to be used at initial loading time
    void loadFloatIssue(Issue<float>);
    void loadIntIssue(Issue<int>);

    void buildValidOffer(map<string, int>);
    int countIssuesInEncounter();

    void printTurnsLeft();
    void printEncounterIssues();
    void printOfferOnTable();

    void beginEncounter(float);

};

#endif

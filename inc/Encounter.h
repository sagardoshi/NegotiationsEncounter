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

    float startInvValue;
    float offerInvValue;
    float endInvValue;


public:
    Encounter(PlayerCharacter*, Negotiator*, int, int);
    ~Encounter();

    int getLevel();

    void saveStandardisedInput(string&);

    void mapPlayerInventory();
    bool isNum(string&);
    void remapKeyword(string&);

    void buildValidOffer(map<string, float>, bool&);

    void printTurns();
    void printOfferOnTable();

    void beginEncounter();
    void printEndEncounter(bool);
    void checkEndEncounter(bool&, bool&);

};

#endif

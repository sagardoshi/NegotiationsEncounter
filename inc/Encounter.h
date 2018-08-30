#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "Offer.h"
#include "PlayerCharacter.h"
#include "Negotiator.h"

#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Encounter {

private:
    string topic;
    int level;
    int turns;
    float keyValue;

    PlayerCharacter* player;
    Negotiator* opponent;
    Offer* offer;

    float startInvValue;
    float offerInvValue;
    float endInvValue;

    void addCharXTimes(char, int, string&);
    void centerText(string, string&);
    void lower(string&);
    void removeWS(string&);
    void createTitle(char, int, string, string&);


public:
    Encounter(PlayerCharacter*, Negotiator*, int, float);
    ~Encounter();

    int getLevel();
    string getCapsName();
    float getFinalInvValue();


    void clearScreen();
    void saveStandardisedInput(string&);

    bool isNum(string&);
    void remapKeyword(string&);

    void setInventoryForEncounter();

    bool runEncounter(bool&);

    void printTurns();
    void printOfferOnTable();

    void printTitle(string, bool = false);
    bool encounterIsOver(bool&);
    void handleEnd(bool);

};

#endif

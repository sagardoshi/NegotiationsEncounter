#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter : public Negotiator {

public:
    PlayerCharacter(string, float);

    map<string, int> inventory;
    map<string, int> invMap;

    void mapPlayerInventory();

    void placeInvObjOnTable(string, Negotiator*);
    void takeBackOffer(Negotiator*);

    void initInventory();
    void fillInventory();
    void checkInventory();
    float getInvValue();

    void printInventory(bool = false, int = 1); // Only special for level 0
    void printStrategy();
    void printHelp();

    void score(float, float, float);
};

#endif

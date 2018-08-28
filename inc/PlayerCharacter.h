#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"
#include "Offer.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter: public Negotiator {

public:
    PlayerCharacter(string, float);

    map<string, int> inventory;
    map<string, int> invMap;

    void mapPlayerInventory();

    void placeInvObjOnTable(string, Offer*);
    void clearTable(Offer*);

    void fillInventory();
    void checkInventory();
    void printInventory(bool = false);

    void printHelp();



};

#endif

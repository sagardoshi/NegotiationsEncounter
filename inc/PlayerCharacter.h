#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"
#include "Offer.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter: public Negotiator {

private:
    int levelReached;

public:
    PlayerCharacter(string, float);

    map<string, int> inventory;

    void addToInventory(string, int);
    void removeFromInventory(string);
    void placeInvObjOnTable(string, Offer*);
    void clearTable(Offer*);

    void enterNegotiation();

    void fillInventory();
    void checkInventory();
    void printInventory();

    void printHelp();



};

#endif

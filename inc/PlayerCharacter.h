#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"
#include "Inventory.h"
#include "Offer.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter: public Negotiator {

private:
    int levelReached;

public:
    PlayerCharacter(string, float);

    // string = name, int1 = default price, int2 = how many owned
    map< string, pair<int, int> > inventory;

    void addToInventory(string, int, int);
    void removeFromInventory(string);
    void placeInvObjOnTable(string, Offer*);

    void enterNegotiation();

    void fillInventory();
    void checkInventory();
    void printInventory();

    void printHelp();



};

#endif

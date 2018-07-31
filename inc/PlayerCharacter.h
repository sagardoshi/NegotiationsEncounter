#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"
#include "Inventory.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter: public Negotiator {

private:
    int levelReached;

    // name: [default price, how many owned]



public:
    PlayerCharacter(string, float);

    map< string, pair<int, int> > inventory;

    void addToInventory(string, int, int);
    void removeFromInventory(string);
    void decrementFromInventory(string);

    void enterNegotiation();

    void fillInventory();
    void checkInventory();
    void printInventory();

    void printHelp();



};

#endif

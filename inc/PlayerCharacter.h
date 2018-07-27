#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"
#include "Inventory.h"

class PlayerCharacter: public Negotiator {

private:
    int levelReached;

public:
    PlayerCharacter(float);
    void enterNegotiation();
    void checkInventory();

    struct Inventory inventory;

};

#endif

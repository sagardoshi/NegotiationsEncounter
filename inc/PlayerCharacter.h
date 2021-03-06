#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"


class PlayerCharacter : public Negotiator {
private:
    void fillPreferences();

public:
    PlayerCharacter(string, float);

    void initInventory();        // Polymorphism to keep clothes!
    void fillInventory();        // IMPT: sets player inv total at beg of game

    // Moves items between player's own inventory and the offer on the table
    void placeItemOnTable(string, Inventory*);
    void takeBackOffer(Inventory*);
};

#endif

#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"

#include <string>
#include <utility>
#include <map>

class PlayerCharacter : public Negotiator {

public:
    PlayerCharacter(string, float);

    map<string, int> inventory;  // Same economy, but distinct inv for player
    void initInventory();        // Polymorphism to keep clothes!
    void fillInventory();        // IMPT: sets inv total at beg of game
    float getInvValue();         // Must be redefined bc inventory is overloaded

    // Moves items between player's own inventory and the offer on the table
    bool knowsOfItem(string item); // Item in overall economy
    bool hasItem(string item);     // Checks if player has item
    void placeItemOnTable(string, Negotiator*);
    void takeBackOffer(Negotiator*);

    // Only special for level 0
    void printInventory(map<string, int>* = NULL);

};

#endif

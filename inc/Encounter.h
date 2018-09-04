#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "PlayerCharacter.h"


class Encounter {

private:
    bool isPrologue;
    int turns;
    float keyValue;

    // The key three parts of a negotiation
    PlayerCharacter* player;
    Negotiator* opponent;
    Inventory* table;

    // Could be in a struct, but easier to understand this way
    float startInvValue;
    float offerInvValue;
    float endInvValue;

    // Purely to order player's inventory items 1..n in a given encounter
    map<string, int> invMap;
    void mapPlayerInventory();
    void setInventoryForEncounter(); // Mother function for above (and others)

    // Header printout methods... repetitive due to unique encounter printouts
    string getCapsName();
    void centerText(string, string&);
    void clearScreen();
    void printTitle(string);

    // User entry methods... repetitive due to unique encounter printouts
    void lower(string&);
    void removeWS(string&);
    bool isNum(string&);            // Checks if string is number
    void remapKeyword(string&);     // Converts item order to item string
    void userEntry(string&, string&, string);

    // Print methods that obscure the UI
    void printTurns();
    void printStrategy();
    void printHelp();

    // Packages calls to Negotiators to print their inventories
    void printUI();

    // Handling end of encounter
    bool encounterIsOver(bool&);
    void printScore();
    void handleEnd(bool);

public:
    Encounter(PlayerCharacter*, Negotiator*, int, float, bool = false);
    ~Encounter();

    bool runEncounter(bool&); // Primary function of class
};

#endif

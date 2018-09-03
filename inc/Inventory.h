#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <map>

using namespace std;


class Inventory {
public:
    Inventory();

    string toPreciseString(float, int = 0); // Helper method for printing

    map<string, int> economy;       // Will be referenced by many negotiators
    void initEconomy();             // Base market values for reference
    bool knowsOfItem(string); // Is an ownable item

    // Inventory methods
    map<string, int> inventory;     // Must be public to move items around
    bool hasItem(string);           // At least 1 in this particular inventory
    bool isEmpty();                 // Convenience method for no items held
    void initInventory();           // Used to set or reset inventory to 0s
    float getInvValue();            // Total value of current inventory
    int getInvCount();              // How many items in inventory

    // Special inv print method for UI... used for printing table & player inv
    void printInv(map<string, int>* = NULL);

};

#endif

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <map>

using namespace std;


class Inventory {
private:
    void initEconomy();             // Base market values for reference

protected:
    map<string, int> economy;       // Will be referenced by many negotiators

public:
    Inventory();

    string toPreciseString(float, int = 0); // Helper method for printing

    bool knowsOfItem(string);       // Is an ownable item

    // Inventory methods
    map<string, int> inventory;     // Must be public to move items around
    void initInventory();           // Used to set or reset inventory to 0s
    bool isEmpty();                 // Convenience method for no items held
    bool hasItem(string);           // At least 1 in this particular inventory
    int getInvCount();              // How many items in inventory
    float getInvValue();            // Total value of current inventory

    // Special inv print method for UI... used for printing table & player inv
    void printInv(map<string, int>* = NULL);

};

#endif

#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

#include <string>
#include <map>


using namespace std;

class Negotiator {

private:
    string name;              // Used to establish preferences and dialogue
    float amiability;         // Personality affects offer evaluations
    float generosityOfOffer;  // Remembered for offer evaluation/player feedback

    float getRandWeight();    // Helper for reactToOffer()

    void initEconomy();       // Base market values for reference
    void fillPreferences();   // Identifies preferences for this negotiator


protected:
    map<string, int> economy;
    map<string, float> prefs;

public:
    Negotiator(string, float);

    string getName();          // Simple getter used for title printout
    void resetGenerosity();    // Resets attribute to 0.0

    // Helper function used throughout so negotiators can print out scores
    string toPreciseString(float, int = 0);

    // Inventory methods
    map<string, int> inventory;    // Must be public to move items around
    void initInventory();          // Used to set or reset inventory to 0s
    float getInvValue();           // Gets value of negotiator's own inventory
    int getInvCount();             // How many items in inventory
    // Special inv print method for UI... used for printing table & player inv
    void printInv(map<string, int>* = NULL);

    // Major offer evaluation and outcome handling... all requested by Encounter
    bool reactToOffer(Negotiator*, float);
    void rejectTerms(int);
    void acceptTerms();
};

#endif

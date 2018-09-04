#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

#include "Inventory.h"


class Negotiator : public Inventory {

private:
    string name;              // Used to establish preferences and dialogue
    float amiability;         // Personality affects offer evaluations
    float generosityOfOffer;  // Remembered for offer evaluation/player feedback

    // Methods exclusively used by Negotiator
    float getRandWeight();    // Helper for reactToOffer()
    void fillPreferences();   // Identifies preferences for this negotiator

protected:
    map<string, float> prefs;

public:
    Negotiator(string, float);

    string getName();          // Simple getter used for title printout
    void resetGenerosity();    // Resets attribute to 0.0

    // Major offer evaluation and outcome handling... all requested by Encounter
    bool reactToOffer(Inventory*, float);
    void handlePrologueRejection(int, string&);
    void rejectTerms(int, bool);
    void acceptTerms();
};

#endif

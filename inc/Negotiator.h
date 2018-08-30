#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

// #include "Offer.h"

#include <string>
#include <map>


using namespace std;

class Negotiator {

private:
    string name;
    float amiability;
    float generosityOfOffer;
    float getRandWeight();

protected:
    map<string, int> economy;
    map<string, float> prefs;

public:
    Negotiator(string, float);

    map<string, int> inventory;

    string toPreciseString(float, const int = 2);

    string getName();
    float getAmiability();

    // Values and economics
    void fillPreferences();
    void initEconomy();
    void initInventory();
    float getInvValue();

    // Dealing with Offers
    bool reactToOffer(Negotiator*, float);
    void rejectTerms(int);
    void acceptTerms();
};

#endif

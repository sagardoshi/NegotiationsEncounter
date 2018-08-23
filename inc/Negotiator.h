#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

#include "Offer.h"

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
    map<string, float> economy;
    map<string, float> prefs;

public:
    Negotiator(string, float);

    string getName();
    float getAmiability();
    void checkpoint();

    // Values and economics
    void fillPreferences();
    void fillEconomy();
    float getInvValue(map<string, int>*);
    void score(float, float, float);

    // Dealing with Offers
    bool reactToOffer(Offer*);
    void rejectTerms(int);
    void acceptTerms();
    void walkAway();
};

#endif

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
    float getRandWeight();


protected:
    map<string, int> economy;
    map<string, float> prefs;

public:
    Negotiator(string, float);

    string getName();
    float getAmiability();
    void setAmiability(float);

    void fillPreferences();
    bool reactToOffer(Offer*);

    void rejectTerms();
    void acceptTerms();
    void walkAway();

};

#endif

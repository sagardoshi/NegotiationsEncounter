#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

#include <string>

using namespace std;

class Negotiator {

private:
    string name;
    float amiability;


protected:
    void walkAway();
    // void proposeOffer();
    void acceptTerms();

public:
    Negotiator(string, float);

    string getName();
    float getAmiability();
    void setAmiability(float);

};

#endif

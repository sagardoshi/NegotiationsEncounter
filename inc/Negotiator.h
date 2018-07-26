#ifndef NEGOTIATOR_H
#define NEGOTIATOR_H

class Negotiator {

private:
    float amiability;

protected:
    void walkAway();
    void proposeOffer();
    void acceptTerms();

public:
    Negotiator(float);

    float getAmiability();
    void setAmiability(float);

};

#endif

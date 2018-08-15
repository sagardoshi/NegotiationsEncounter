#ifndef OFFER_H
#define OFFER_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Offer {
public:
    Offer();

    map<string, int> offerInv;

    void addObjToTable(string);

    int inventoryCount();
    int issueCount();
    void printOffer();

};

#endif

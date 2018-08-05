#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <map>
#include <random>

using namespace std;


Negotiator::Negotiator(string n, float a) : name(n), amiability(a) {
    fillPreferences();

    // name: [default price, how many owned]
    economy["pomegranate"] = 10;
    economy["knuckle pads"] = 15;
    economy["silverback perfume"] = 10;
    economy["ginger cookie"] = 5;
    economy["coins"] = 1;
    economy["basket"] = 2;
    economy["porridge's key"] = 30;
}

string Negotiator::getName()                { return name;        }
float Negotiator::getAmiability()           { return amiability;  }
void Negotiator::setAmiability(float newAm) { amiability = newAm; }

// A trivial 0-1 random generator engine from time-based seed
float Negotiator::getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator)/100.0;
}


void Negotiator::fillPreferences() {
    // Player will take base value
    if (name == "You") {
        prefs["pomegranate"] = 1.0;
        prefs["knuckle pads"] = 1.0;
        prefs["silverback perfume"] = 1.0;
        prefs["ginger cookie"] = 1.0;
        prefs["coins"] = 1.0;
        prefs["basket"] = 1.0;
        prefs["porridge's key"] = 1.0;
    }


    else if (name == "Porridge") {
        prefs["pomegranate"] = 1.5; // loves fruit
        prefs["knuckle pads"] = 0.25; // irrelevant
        prefs["silverback perfume"] = 2.0; // wants to seem older
        prefs["ginger cookie"] = 1.5; // loves cookies
        prefs["coins"] = 1.0;
        prefs["basket"] = 0.25; // irrelevant
        prefs["porridge's key"] = 1.0;
    }
}


bool Negotiator::reactToOffer(Offer* offer) {
    offer->printOffer();
    map<string, int>::iterator it;

    float sum = 0.0;
    float generosityOfOffer = 0.0;
    float response = 0.0;
    float valuePorridgeKey = prefs["porridge's key"]*economy["porridge's key"];


    for (it = offer->inventory.begin(); it != offer->inventory.end(); it++) {
        if (it->second > 0) {
            sum += prefs[it->first] * economy[it->first] * it->second;
            // Uncomment the below to see how the values are calculated
            // cout << "Standard Value: " << it->first << ": ";
            // cout << economy[it->first] * it->second << endl;
            //
            // cout << "Porridge's Value: ";
            // cout << it->first << ": ";
            // Porridge's weighting * base value * quantity offered
            // cout << prefs[it->first] * economy[it->first] * it->second << endl;
        }
    }

    cout << "\nTotal value of offer to Porridge: " << sum << endl;
    cout << "Value of key to Porridge: " << valuePorridgeKey << endl << endl;

    generosityOfOffer = sum/valuePorridgeKey;

    if (generosityOfOffer >= 1) return true;
    else if (generosityOfOffer <= 0.5) return false;
    else {
        // See asterisked comment at bottom of file for method
        response = (amiability * getRandWeight()) + generosityOfOffer;

        if (response > 1) return true;
        else return false;
    }


}

void Negotiator::walkAway() {}

void Negotiator::acceptTerms() {
    cout << "Your opponent has accepted the offer on the table. ";
    cout << "Congratulations!\n\n";
}

void Negotiator::rejectTerms() {
    cout << "Your opponent rejects the offer on the table. Try again, ";
    cout << "but don't forget that you only have a few turns before your ";
    cout << "opponent loses patience.\n\n";




}


/*

** Explanation of acceptance determination from reactToOffer(Offer*)

---- 22 OFFERED FOR KEY OF 30 VALUE... POOR OFFER ----

Random values
0        .2        .4          .6         .8        1
If with *.8 amiability
0        .16       .32         .48        .64       .8
+ an offer 0.73 to target
0.73     0.89      1.05        1.21       1.37      1.53

Random values
0        .2        .4          .6         .8        1
If with *.2 amiability
0        .04       .08         .12        .16       .2
+ an offer 0.73 to target
0.73     0.77      0.81        0.85       0.89      0.93


---- 28 OFFERED FOR KEY OF 30 VALUE... GOOD OFFER ----

Random values
0        .2        .4          .6         .8        1
If with *.8 amiability
0        .16       .32         .48        .64       .8
+ an offer 0.93 to target
0.93     1.09      1.25        1.41       1.57      1.73

Random values
0        .2        .4          .6         .8        1
If with *.2 amiability
0        .04       .08         .12        .16       .2
+ an offer 0.93 to target
0.93     0.97      1.01        1.05       1.09      1.13

how close to target + (amiability * rando)
if > 1, accept!
else reject

*/

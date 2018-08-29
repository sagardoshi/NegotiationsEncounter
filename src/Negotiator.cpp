#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <random>

using namespace std;


Negotiator::Negotiator(string n, float a) : name(n), amiability(a),
                                            generosityOfOffer(0.0) {
    fillPreferences();
    fillEconomy();
}

string Negotiator::getName()                { return name;        }
float Negotiator::getAmiability()           { return amiability;  }

// A trivial 0-1 random generator engine from time-based seed
float Negotiator::getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator) / 100.0;
}

template <typename T>
string Negotiator::toPreciseString(const T input, const int precision) {
    ostringstream out;
    out << setprecision(precision) << input;
    return out.str();
}

// Pauses text for user to read and get a breather
void Negotiator::checkpoint() {
    string input = "";
    string PROMPT_DIVIDER =  "----------------------------------------";
           PROMPT_DIVIDER += "----------------------------------------\n";
    string anyInputText = "\n" + PROMPT_DIVIDER + "[Press return to continue.]";

    cout << anyInputText;
    getline(cin, input);
    cout << PROMPT_DIVIDER << "\n";
}

float Negotiator::getInvValue(map<string, int>* invPointer) {
    map<string, int> inv = *invPointer;

    map<string, int>::iterator it;
    string itemName = "";
    float baseValue = 0.0, totalValue = 0.0;
    int quantity = 0;

    // Multiply each inventory item's base value by amount held and sum up
    for (it = inv.begin(); it != inv.end(); it++) {
        itemName = it->first;
        quantity = it->second;
        baseValue = economy[itemName];
        totalValue += (baseValue * quantity);
    }
    return totalValue;
}

// Full copy (unfortunately) of economy in NegotiationMain.cpp
void Negotiator::fillEconomy() {
    // 0) Tutorial Lepha
    economy["black trousers"] = 1.0;
    economy["black tunic"] = 1.0;

    // 1) Mosta the Stork/Pepita the Pigeon -- spirits of inquisitiveness
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["packet of sunflower seeds"] = 5.0;

    // 2) Toto the #2 Rabbit of Centzon Totochtin -- spirit of drunkenness
    economy["pulque flask"] = 5.0;
    economy["paint canister"] = 2.0;
    economy["morning headache tonic"] = 10.0;
    economy["long ear warmers"] = 5.0;


    // 3) Burro the night river beaver -- spirit of industriousness
    economy["wood polish bottle"] = 6.0;
    economy["waterproof wax jar"] = 8.0;

    // 4) lepha -- spirit of order
}


void Negotiator::fillPreferences() {
    // Player will take base value
    if (name == "You") {
        prefs["black trousers"] = 1.0;
        prefs["black tunic"] = 1.0;
        prefs["burn relief ointment"] = 1.0;
        prefs["carved walking cane"] = 1.0;
        prefs["packet of sunflower seeds"] = 1.0;
        prefs["pulque flask"] = 1.0;
        prefs["paint canister"] = 1.0;
        prefs["morning headache tonic"] = 1.0;
        prefs["long ear warmers"] = 1.0;
        prefs["wood polish bottle"] = 1.0;
        prefs["waterproof wax jar"] = 1.0;
    }
    else if (name == "Mosta and Pepita") {
        prefs["black trousers"] = 0.1;
        prefs["black tunic"] = 0.1;
        prefs["burn relief ointment"] = 1.25;
        prefs["carved walking cane"] = 1.5;
        prefs["packet of sunflower seeds"] = 1.25;
        prefs["pulque flask"] = 0.2;
        prefs["paint canister"] = 0.1;
        prefs["morning headache tonic"] = 0.4;
        prefs["long ear warmers"] = 0.1;
        prefs["wood polish bottle"] = 0.3;
        prefs["waterproof wax jar"] = 0.4;
    }
    else if (name == "Toto") {
        prefs["black trousers"] = 0.1;
        prefs["black tunic"] = 0.1;
        prefs["burn relief ointment"] = 0.2;
        prefs["carved walking cane"] = 0.3;
        prefs["packet of sunflower seeds"] = 0.75;
        prefs["pulque flask"] = 1.5;
        prefs["paint canister"] = 1.75;
        prefs["morning headache tonic"] = 0.5;
        prefs["long ear warmers"] = 2.0;
        prefs["wood polish bottle"] = 0.2;
        prefs["waterproof wax jar"] = 0.3;
    }
    else if (name == "Burro") {
        prefs["black trousers"] = 0.1;
        prefs["black tunic"] = 0.1;
        prefs["burn relief ointment"] = 0.2;
        prefs["carved walking cane"] = 1.2;
        prefs["packet of sunflower seeds"] = 1.0;
        prefs["pulque flask"] = 1.5;
        prefs["paint canister"] = 1.25;
        prefs["morning headache tonic"] = 0.2;
        prefs["long ear warmers"] = 0.1;
        prefs["wood polish bottle"] = 1.8;
        prefs["waterproof wax jar"] = 1.5;
    }
}



bool Negotiator::reactToOffer(Offer* offer, float keyValue) {
    map<string, int>::iterator it;
    generosityOfOffer = 0.0; // Reset if hadn't already been
    float sum = 0.0, response = 0.0;

    string item = "";
    int amount = 0;

    for (it = offer->inventory.begin(); it != offer->inventory.end(); it++) {
        item = it->first;
        amount = it->second;

        if (amount > 0) sum += prefs[item] * economy[item] * amount;
    }

    generosityOfOffer = sum / keyValue;

    if (generosityOfOffer >= 1) return true;
    else if (generosityOfOffer <= 0.5) return false;
    else {
        // See asterisked comment at bottom of file for method
        response = (amiability * getRandWeight()) + generosityOfOffer;

        if (response > 1) return true;
        else return false;
    }

}

void Negotiator::score(float startVal, float offerVal, float endVal) {

    string border = "***** ENCOUNTER SCORE *****\n";
    string scoreText  = "You began with an inventory of market value: ";
           scoreText += toPreciseString(startVal) + "\n";
           scoreText += "You gave away a total market value of: ";
           scoreText += toPreciseString(offerVal) + "\n";
           scoreText += "You ended with an inventory of market value: ";
           scoreText += toPreciseString(endVal) + "\n";

    cout << border << scoreText << border << endl;
}

void Negotiator::walkAway() {
    // No need for more explanations, as player has seen them for each mistake

    // if (name == "Mosta and Pepita") loadScript("1/Level1Lose");
    // if (name == "Toto") loadScript("2/Level2Lose");
    // if (name == "Burro") loadScript("3/Level3Lose");
    // if (name == "Lepha") loadScript("4/Level4Lose");

    // exit(0);
}

void Negotiator::acceptTerms() {

    string acceptance = "You think over the encounter you just had.\n\n";

    // Try to give somewhat customised feedback after each rejected offer
    if (generosityOfOffer > 1) {
        acceptance += "Your offer was quite generous. In fact, " + name + " ";
        acceptance += "may have accepted a\n";
        acceptance += "lower offer, if you had a way to give it. ";
        acceptance += "You have progressed ";
        acceptance += "onward in your\njourney, but you may find ";
        acceptance += "yourself short of inventory in the future, having\n";
        acceptance += "given away what you have here.\n\n";
        acceptance += "Nevertheless, you survive to fight ";
        acceptance += "another day. Well done!\n";
    } else if (generosityOfOffer >= 0.5 && generosityOfOffer < 0.75) {
        acceptance += "Wow, you drove an extremely hard bargain and won! ";
        acceptance += "Very, very impressive. You've\nmoved along efficiently ";
        acceptance += "and set yourself up well for the future. Well done!\n";
    } else if (generosityOfOffer >= 0.75 && generosityOfOffer < 1.0) {
        acceptance += "You made a solid offer and won the day. ";
        acceptance += "It's up to you now to continue to manage\nyour stock ";
        acceptance += "efficiently and get out of here safely. Well done!\n";
    }

    cout << acceptance << endl;

}

void Negotiator::rejectTerms(int turnsLeft) {
    string rejection = "";
    bool plural = (name == "Mosta and Pepita" ? true : false);

    rejection  = name + " reject" + (plural ? " " : "s ");
    rejection += "the offer on the table.\n\n";

    // Try to give somewhat customised feedback after each rejected offer
    if (generosityOfOffer <= 0.5) {
        rejection += "Your offer was far from acceptable to the spirit";
        rejection += (plural ? "s. " : ". ");
        rejection += "Try looking for items\nthat might be more valuable ";
        rejection += "to " + name + " in particular.";
    } else { // Must have offered between 0.5 and 1
        if (amiability < 0.5) {  // offered 0.5 and 1; amiable opponent
            rejection += "A little off. Your offer was reasonable, but " + name;
            rejection += (plural ? " are " : " is ");
            rejection += "not easy to crack.\n";
            rejection += "Try a slightly bigger offer or select items that may";
            rejection += " appeal more to " + name + " in particular.";
        } else { // offered 0.5 and 1; non-amiable opponent
            rejection += "You just missed it! Your offer was reasonable, and ";
            rejection += name + (plural ? " are " : " is ");
            rejection += "relatively friendly.\n";
            rejection += "Try a slightly bigger offer or select items that may";
            rejection += " appeal more to " + name + " in particular.";
        }
    }

    // Note turns left
    rejection += "\n\nDon't forget that ";
    rejection += "you only have " + to_string(turnsLeft) + " more turn";
    rejection += (turnsLeft == 1 ? " " : "s ");
    rejection += "before ";
    rejection += (plural ? "their " : "the spirit's ");
    rejection += "patience runs out.\n\n";

    // Flush out string
    cout << rejection;
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

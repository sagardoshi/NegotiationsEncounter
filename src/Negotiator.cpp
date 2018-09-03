#include "../inc/Negotiator.h"
// #include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <map>
#include <random>

using namespace std;


Negotiator::Negotiator(string n, float a) : name(n), amiability(a),
                                            generosityOfOffer(0.0) {
    initEconomy();
    fillPreferences();

}

string Negotiator::getName()       { return name;             }
void Negotiator::resetGenerosity() { generosityOfOffer = 0.0; }

// A trivial 0-1 random generator engine from time-based seed
float Negotiator::getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator) / 100.0;
}

string Negotiator::toPreciseString(float input, int precision) {
    string asString = to_string(input);

    int decimalIndex = asString.find(".");
    int cutoff = decimalIndex + 1 + precision;

    // Don't include decimal if zero floats desired
    if (precision == 0) return asString.substr(0, decimalIndex);
    else return asString.substr(0, cutoff);
}

float Negotiator::getInvValue() {
    map<string, int>::iterator it;
    string itemName = "";
    float baseValue = 0.0, totalValue = 0.0;
    int quantity = 0;

    // Multiply each inventory item's base value by amount held and sum up
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        quantity = it->second;
        baseValue = economy[itemName];
        totalValue += (baseValue * quantity);
    }
    return totalValue;
}

void Negotiator::initEconomy() {

    // 1) Mosta the Stork/Pepita the Pigeon -- spirits of inquisitiveness
    economy["burn relief ointment"] = 7;
    economy["carved walking cane"] = 10;
    economy["sunflower seeds packet"] = 3;

    // 2) Toto the #2 Rabbit of Centzon Totochtin -- spirit of drunkenness
    economy["pulque flask"] = 4;
    economy["paint canister"] = 2;
    economy["morning headache tonic"] = 8;
    economy["long earmuffs"] = 5;


    // 3) Burro the night river beaver -- spirit of industriousness
    economy["wood varnish bottle"] = 6;
    economy["waterproof wax jar"] = 8;

    // 4) Lepha the Elephant -- pharmacologist to the spirits
    economy["loose leaf sencha tea"] = 9;
    economy["vinegar disinfectant"] = 3;

    // 0) Tutorial Lepha
    economy["personal black trousers"] = 2;
    economy["personal black tunic"] = 1;

}

void Negotiator::initInventory() {
    inventory["burn relief ointment"] = 0;
    inventory["carved walking cane"] = 0;
    inventory["sunflower seeds packet"] = 0;
    inventory["pulque flask"] = 0;
    inventory["paint canister"] = 0;
    inventory["morning headache tonic"] = 0;
    inventory["long earmuffs"] = 0;
    inventory["wood varnish bottle"] = 0;
    inventory["waterproof wax jar"] = 0;
    inventory["loose leaf sencha tea"] = 0;
    inventory["vinegar disinfectant"] = 0;
    inventory["personal black trousers"] = 0;
    inventory["personal black tunic"] = 0;
}

void Negotiator::fillPreferences() {
    // Player will take base value
    if (name == "You") {
        prefs["personal black trousers"] = 1.0;
        prefs["personal black tunic"] = 1.0;
        prefs["burn relief ointment"] = 1.0;
        prefs["carved walking cane"] = 1.0;
        prefs["sunflower seeds packet"] = 1.0;
        prefs["pulque flask"] = 1.0;
        prefs["paint canister"] = 1.0;
        prefs["morning headache tonic"] = 1.0;
        prefs["long earmuffs"] = 1.0;
        prefs["wood varnish bottle"] = 1.0;
        prefs["waterproof wax jar"] = 1.0;
        prefs["loose leaf sencha tea"] = 1.0;
        prefs["vinegar disinfectant"] = 1.0;
    }
    else if (name == "Mosta and Pepita") {
        prefs["personal black trousers"] = 0.1;
        prefs["personal black tunic"] = 0.1;
        prefs["burn relief ointment"] = 1.25;
        prefs["carved walking cane"] = 1.5;
        prefs["sunflower seeds packet"] = 1.25;
        prefs["pulque flask"] = 0.7;
        prefs["paint canister"] = 0.1;
        prefs["morning headache tonic"] = 0.4;
        prefs["long earmuffs"] = 0.1;
        prefs["wood varnish bottle"] = 0.3;
        prefs["waterproof wax jar"] = 0.4;
        prefs["loose leaf sencha tea"] = 1.0;
        prefs["vinegar disinfectant"] = 0.5;
    }
    else if (name == "Toto") {
        prefs["personal black trousers"] = 0.1;
        prefs["personal black tunic"] = 0.1;
        prefs["burn relief ointment"] = 0.2;
        prefs["carved walking cane"] = 0.3;
        prefs["sunflower seeds packet"] = 0.75;
        prefs["pulque flask"] = 1.5;
        prefs["paint canister"] = 1.75;
        prefs["morning headache tonic"] = 0.5;
        prefs["long earmuffs"] = 2.0;
        prefs["wood varnish bottle"] = 0.2;
        prefs["waterproof wax jar"] = 0.3;
        prefs["loose leaf sencha tea"] = 0.4;
        prefs["vinegar disinfectant"] = 0.2;
    }
    else if (name == "Burro") {
        prefs["personal black trousers"] = 0.1;
        prefs["personal black tunic"] = 0.1;
        prefs["burn relief ointment"] = 0.2;
        prefs["carved walking cane"] = 1.2;
        prefs["sunflower seeds packet"] = 1.0;
        prefs["pulque flask"] = 1.5;
        prefs["paint canister"] = 1.25;
        prefs["morning headache tonic"] = 0.2;
        prefs["long earmuffs"] = 0.1;
        prefs["wood varnish bottle"] = 1.8;
        prefs["waterproof wax jar"] = 1.5;
        prefs["loose leaf sencha tea"] = 0.7;
        prefs["vinegar disinfectant"] = 0.6;
    }
    else if (name == "Lepha") {
        prefs["personal black trousers"] = 0.1;
        prefs["personal black tunic"] = 0.1;
        prefs["burn relief ointment"] = 1.4;
        prefs["carved walking cane"] = 0.2;
        prefs["sunflower seeds packet"] = 0.5;
        prefs["pulque flask"] = 0.8;
        prefs["paint canister"] = 0.2;
        prefs["morning headache tonic"] = 1.25;
        prefs["long earmuffs"] = 0.9;
        prefs["wood varnish bottle"] = 1.2;
        prefs["waterproof wax jar"] = 0.2;
        prefs["loose leaf sencha tea"] = 2.0;
        prefs["vinegar disinfectant"] = 2.0;
    }
}

bool Negotiator::reactToOffer(Negotiator* offer, float keyValue) {
    map<string, int>::iterator it;
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

void Negotiator::acceptTerms() {

    string acceptance = "You think over the encounter you just had.\n\n";

    // Try to give somewhat customised feedback after each rejected offer
    if (generosityOfOffer > 1.5) {
        acceptance += "Your offer was far too generous! " + name + " ";
        acceptance += "would have accepted a much\n";
        acceptance += "lower offer, if you had a way to give it. ";
        acceptance += "You have progressed ";
        acceptance += "onward in your\njourney, but you may find ";
        acceptance += "yourself short of inventory in the future, having\n";
        acceptance += "given away what you have here.\n\n";
        acceptance += "Nevertheless, you survive to fight ";
        acceptance += "another day. Well done!\n";
    } else if (generosityOfOffer >= 1 && generosityOfOffer <= 1.5) {
        acceptance += "Your offer was generous. A little more, even, ";
        acceptance += "than what may have sufficed.\n";
        acceptance += "Manage your inventory carefully in future ";
        acceptance += "encounters.\n\n";
        acceptance += "Still, you survive to fight ";
        acceptance += "another day. Well done!\n";
    }
     else if (generosityOfOffer >= 0.5 && generosityOfOffer < 0.75) {
        acceptance += "Wow, you drove an extremely hard bargain and won! ";
        acceptance += "Very, very impressive. You've\nmoved along efficiently ";
        acceptance += "and set yourself up well for the future. Well done!\n";
    } else if (generosityOfOffer >= 0.75 && generosityOfOffer < 1.0) {
        acceptance += "You made a solid offer and won the day. ";
        acceptance += "It's up to you now to continue to manage\nyour stock ";
        acceptance += "efficiently and get out of here safely. Well done!\n";
    } else {
        acceptance += "It's impossible to win the way you did, so you must ";
        acceptance += "be a superuser. All hail\n";
        acceptance += "the superuser. Happy supering.\n";
    }

    cout << acceptance << endl;

}

void Negotiator::rejectTerms(int turnsLeft) {
    string rejection = "";
    bool plural = (name == "Mosta and Pepita" ? true : false);

    rejection  = name + " reject" + (plural ? " " : "s ") + "your offer.";

    // You offered something at least
    if (generosityOfOffer > 0) rejection += " You take it back.";
    rejection += "\n\n";

    // Try to give somewhat customised feedback after each rejected offer
    if (generosityOfOffer == 0) {
        rejection += "You offered nothing! Do you really expect that " + name;
        rejection += " would consider that?";
    } else if (generosityOfOffer <= 0.5) {
        rejection += "It was far from acceptable to the spirit";
        rejection += (plural ? "s. " : ". ");
        rejection += "Try looking for items that might\n";
        rejection += "be more valuable to " + name + " in particular.";
    } else if (generosityOfOffer > 0.5 && generosityOfOffer < 1) {
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
    rejection += "\n\nYou only have " + to_string(turnsLeft) + " more turn";
    rejection += (turnsLeft == 1 ? " " : "s ");
    rejection += "before " + name + "'s patience runs out.\n\n";

    // Flush out string
    cout << rejection;
}


/*

** Explanation of acceptance determination from reactToOffer()

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

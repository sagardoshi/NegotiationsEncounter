#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <map>
#include <random>
#include <chrono>

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
    economy["rhubarb's key"] = 40;
    economy["chamoy's key"] = 50;
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
        prefs["rhubarb's key"] = 1.0;
        prefs["chamoy's key"] = 1.0;
    }

    else if (name == "Porridge") {
        prefs["pomegranate"] = 1.25; // loves fruit
        prefs["knuckle pads"] = 0.25; // has young, fresh knuckles
        prefs["silverback perfume"] = 2.0; // wants to seem older
        prefs["ginger cookie"] = 1.25; // loves cookies
        prefs["coins"] = 1.0;
        prefs["basket"] = 0.25; // irrelevant
        prefs["porridge's key"] = 1.0;
        prefs["rhubarb's key"] = 0.0; // irrelevant
        prefs["chamoy's key"] = 0.0; // irrelevant
    }

    else if (name == "Rhubarb") {
        prefs["pomegranate"] = 1.5; // loves fruit
        prefs["knuckle pads"] = 1.0;
        prefs["silverback perfume"] = 0.5; // is female
        prefs["ginger cookie"] = 0.5; // no nutrition
        prefs["coins"] = 1.25; // cash poor, idea rich
        prefs["basket"] = 1.25; // can never have enough
        prefs["porridge's key"] = 0.0; // irrelevant
        prefs["rhubarb's key"] = 1.0;
        prefs["chamoy's key"] = 0.0; // irrelevant
    }

    else if (name == "Chamoy") {
        prefs["pomegranate"] = 1.5; // loves fruit
        prefs["knuckle pads"] = 3.0; // very important in old age!
        prefs["silverback perfume"] = 0.25; // ain't got no time for romance
        prefs["ginger cookie"] = 1.5; // who doesn't love a cookie?
        prefs["coins"] = 0.5; // you can't take it with you
        prefs["basket"] = 1.0; // sure
        prefs["porridge's key"] = 0.0; // irrelevant
        prefs["rhubarb's key"] = 0.0; // irrelevant
        prefs["chamoy's key"] = 1.0;
    }
}


bool Negotiator::reactToOffer(Offer* offer) {
    map<string, int>::iterator it;

    string ownKey = "";
    float sum = 0.0;
    float generosityOfOffer = 0.0;
    float response = 0.0;

    if (name == "Porridge") ownKey = "porridge's key";
    if (name == "Rhubarb") ownKey = "rhubarb's key";
    if (name == "Chamoy") ownKey = "chamoy's key";
    float valueOfKey = prefs[ownKey] * economy[ownKey];

    for (it = offer->inventory.begin(); it != offer->inventory.end(); it++)
        if (it->second > 0)
            sum += prefs[it->first] * economy[it->first] * it->second;

    generosityOfOffer = sum / valueOfKey;

    if (generosityOfOffer >= 1) return true;
    else if (generosityOfOffer <= 0.5) return false;
    else {
        // See asterisked comment at bottom of file for method
        response = (amiability * getRandWeight()) + generosityOfOffer;

        if (response > 1) return true;
        else return false;
    }

}

void Negotiator::walkAway() {
    if (name == "Porridge") {
        cout << "The little gorilla screws his face up in frustration. ";
        cout << "\"Grandma Chamoy told me I'd be seeing someone who ";
        cout << "came with interesting goodies to taste and to help ";
        cout << "me feel more grown-up. But you just came with boring ";
        cout << "stuff!\"\n\n";

        cout << "You shrug your shoulders, but he just ";
        cout << "balls his little fists in rage. ";
        cout << "\"This was all supposed to go so much better! ";
        cout << "He looks away from you, still seething.\n\n";

        cout << "Then, without ";
        cout << "warning, he snaps his head back up to you, staring you ";
        cout << "down. He looks side to side, suddenly conspiratorial. ";
        cout << "With one padded finger, he beckons you toward him.\n\n";

        cout << "You lean down to get eye to eye with him. He cups his ";
        cout << "hand and whispers something soft to you. \"Grandma ";
        cout << "Chamoy taught me to do this if my negotiations ";
        cout << "turn out bad, like this. Hope it doesn't hurt too ";
        cout << "much.\"\n\n";

        cout << "Before you realise it, he has spread his arms wide, to ";
        cout << "either side of ";
        cout << "your head. As you begin to panic and back away, it is ";
        cout << "already too late. Porridge brings his arms ";
        cout << "together with shocking speed, clapping your ears and ";
        cout << "temples. You feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Porridge Trigor.\n\n";

        cout << "Perhaps you might do better in another life if you keep ";
        cout << "a closer eye on your turns left and offer items more ";
        cout << "attractive to Porridge. Even young gorillas are far ";
        cout << "stronger than you, flimsy bipedal human.\n\n";

    }

    if (name == "Rhubarb") {
        cout << "Rhubarb never liked you. She seems gleeful for a moment.\n\n";
        cout << "A pause. Another pause.\n\n";
        cout << "Before you know what's happening, she has let out an ";
        cout << "roar and comes barreling toward you. You feel a sharp pain, ";
        cout << " and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Rhubarb Trigor.\n\n";

        cout << "Perhaps you might do better in another life if you are ";
        cout << "a bit more careful with your inventory ahead of time. ";
        cout << "And don't forget –- Rhubarb isn't as amenable as her son is. ";
        cout << "You have to suss out her needs better.\n\n";

        cout << "Or else, you'll let her engage in her most important ";
        cout << "job: defending her nest from spindly hairless intruders ";
        cout << "like you.\n\n";
    }

    if (name == "Chamoy") {
        cout << "Chamoy looks sleepy, like she always does.\n\n";
        cout << "She yawns wide, and just keeps yawning, until she ends it ";
        cout << "in a smile. But this isn't the smile you've been given so ";
        cout << "far, toothless and all lip. This smile is a rictus, a fang-";
        cout << "bearing, blood-thumping sign of aggression.\n\n";

        cout << "You will not be okay.\n\n";

        cout << "Sure enough, she brings her head down onto yours, and your ";
        cout << "time of consciousness is kept mercifully short. You simply ";
        cout << "feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Chamoy Trigor.\n\n";

        cout << "By the third negotiation, things are tough. You need to have ";
        cout << "conserved the right resources. Think ahead to what an older ";
        cout << "matriarch like Chamoy might want the most, and save those ";
        cout << "resources if you can. At least Chamoy's predominant ";
        cout << "characteristic is languor, rather than anger.\n\n";

        cout << "But even then, you cannot mistake it for passivity. She ";
        cout << "might seem lighthearted, but it is her home that you have ";
        cout << "invaded. She is giving you a way out, feeling sorry for the ";
        cout << "rickety hominin lacking even a reasonable nose print, but ";
        cout << "having failed to prove yourself worthy, away you go.\n\n";
    }

    cout << "Goodbye, and happy haggling!\n\n\n\n";
    exit(0);
}

void Negotiator::acceptTerms() {
    if (name == "Porridge") {
        cout << "Porridge grins just with his lips, a sparkle in his ";
        cout << "eyes. \"Yes! I'll take that offer!\" he shouts, ";
        cout << "leaping up and beginning to scoop up his winnings. ";
        cout << "He tosses you his key, and you snatch it out of the ";
        cout << "air.\n\n";

        cout << "Congratulations, you now have Porridge's Key! You are ";
        cout << "one step toward your escape. Two more to go...\n\n";

        cout << "Porridge is lost in his own bliss, inspecting his ";
        cout << "winnings, when you ";
        cout << "clear your throat. \"Oh,\" he says, remembering ";
        cout << "you, \"I guess you still have to get past the rest of ";
        cout << "the family, right?\n\n";

        cout << "You nod, and Porridge says, \"Well, just go back ";
        cout << "through the family room, where Grandma sits, and keep ";
        cout << "on heading through until you get to my Momma's door.\" ";
        cout << "He pauses, checks the clock on the wall, and scratches ";
        cout << "his head. \"Umm, just be careful, okay? If you catch ";
        cout << "her before lunchtime, she can be a little cranky.\n\n";

        cout << "You give Porridge a high-five and go back out of the ";
        cout << "room and down the hallway.";
    }

    if (name == "Rhubarb") {
        cout << "Rhubarb accepts your offer. Nice job! [More flavour text ";
        cout << "to be filled out here.]\n\n";
    }

    if (name == "Chamoy") {
        cout << "Chamoy accepts your offer. Nice job! You win! Wooooo! ";
        cout << "[More flavour text to be filled out here.]\n\n";
    }
}


void Negotiator::rejectTerms(int turnsLeft) {
    if (turnsLeft == 0) walkAway();

    cout << name << " rejects the offer on the table. Try again, ";
    cout << "but don't forget that you only have " << turnsLeft;
    cout << (turnsLeft == 1 ? " turn" : " turns") << " left ";
    cout << "before " << name << " loses patience.\n\n";
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

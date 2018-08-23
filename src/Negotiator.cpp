#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <random>

using namespace std;


Negotiator::Negotiator(string n, float a) : name(n), amiability(a) {
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
    return distribution(generator)/100.0;
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
    float baseValue = 0.0;
    int quantity = 0;
    float totalValue = 0.0;

    for (it = inv.begin(); it != inv.end(); it++) {
        itemName = it->first;
        quantity = it->second;
        baseValue = economy[itemName];
        totalValue += (baseValue * quantity);
    }
    return totalValue;
}

void Negotiator::fillEconomy() {
    // economy[name] = base value
    economy["burn relief ointment"] = 10.0;
    economy["carved walking cane"] = 15.0;
    economy["sunflower seeds"] = 10.0;
    economy["bird spirit key packet"] = 10.0;
}


void Negotiator::fillPreferences() {
    // Player will take base value
    if (name == "You") {
        prefs["burn relief ointment"] = 1.0;
        prefs["carved walking cane"] = 1.0;
        prefs["sunflower seeds packet"] = 1.0;
        prefs["bird spirit key"] = 1.0;
    }
    else if (name == "Mosta and Pepita") {
        prefs["burn relief ointment"] = 2.0;
        prefs["carved walking cane"] = 2.0;
        prefs["sunflower seeds packet"] = 1.5;
        prefs["bird spirit key"] = 1.0;
    }
}



bool Negotiator::reactToOffer(Offer* offer) {
    map<string, int>::iterator it;

    string ownKey = "";
    float sum = 0.0;
    float generosityOfOffer = 0.0;
    float response = 0.0;

    string item = "";
    int amount = 0;

    if (name == "Mosta and Pepita") ownKey = "bird spirit key";
    float valueOfKey = prefs[ownKey] * economy[ownKey];


    for (it = offer->offerInv.begin(); it != offer->offerInv.end();
         it++) {
        item = it->first;
        amount = it->second;

        if (amount > 0) sum += prefs[item] * economy[item] * amount;
    }

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

void Negotiator::score(float startVal, float offerVal, float endVal) {

    cout << "You began with an inventory of base value: ";
    cout << startVal << endl;

    cout << "You gave away a total value of: ";
    cout << offerVal << endl;

    cout << "You ended with an inventory of base value: ";
    cout << endVal << endl << endl;



    return;
}

void Negotiator::walkAway() {
    if (name == "Porridge") {
        cout << "The little gorilla screws his face up in frustration. ";
        cout << "\"Grandma Chamoy told me I'd be seeing someone who ";
        cout << "came with interesting goodies to taste and to help ";
        cout << "me feel more grown-up. But you just came with boring ";
        cout << "stuff!\"\n\n";

        checkpoint();

        cout << "You shrug your shoulders, but he just ";
        cout << "balls his little fists in rage. ";
        cout << "\"This was all supposed to go so much better! ";
        cout << "He looks away from you, still seething.\n\n";

        cout << "Then, without ";
        cout << "warning, he snaps his head back up to you, staring you ";
        cout << "down. He looks side to side, suddenly conspiratorial. ";
        cout << "With one padded finger, he beckons you toward him.\n\n";

        checkpoint();

        cout << "You lean down to get eye to eye with him. He cups his ";
        cout << "hand and whispers something soft to you. \"Grandma ";
        cout << "Chamoy taught me to do this if my negotiations ";
        cout << "turn out bad, like this. Hope it doesn't hurt too ";
        cout << "much.\"\n\n";

        checkpoint();

        cout << "Before you realise it, he has spread his arms wide, to ";
        cout << "either side of ";
        cout << "your head. As you begin to panic and back away, it is ";
        cout << "already too late. Porridge brings his arms ";
        cout << "together with shocking speed, clapping your ears and ";
        cout << "temples. You feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Porridge Trigor.\n\n";

        checkpoint();

        cout << "Perhaps you might do better in another life if you keep ";
        cout << "a closer eye on your turns left and offer items more ";
        cout << "attractive to Porridge. Even young gorillas are far ";
        cout << "stronger than you, flimsy bipedal human.\n\n";

    }

    if (name == "Rhubarb") {
        cout << "Rhubarb never liked you. She seems gleeful for a moment.\n\n";
        cout << "A pause. Another pause.\n\n";

        checkpoint();

        cout << "Before you know what's happening, she has let out an ";
        cout << "roar and comes barreling toward you. You feel a sharp pain, ";
        cout << " and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Rhubarb Trigor.\n\n";

        checkpoint();

        cout << "Perhaps you might do better in another life if you are ";
        cout << "a bit more careful with your inventory ahead of time. ";
        cout << "And don't forget –- Rhubarb isn't as amenable as her son is. ";
        cout << "You have to suss out her needs better.\n\n";

        cout << "Or else, you'll remind her of her most important ";
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

        checkpoint();

        cout << "Sure enough, she brings her head down onto yours, and your ";
        cout << "time of consciousness is kept mercifully short. You simply ";
        cout << "feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Chamoy Trigor.\n\n";

        checkpoint();

        cout << "By the third negotiation, things are tough. You need to have ";
        cout << "conserved the right resources. Think ahead to what an older ";
        cout << "matriarch like Chamoy might want the most, and save those ";
        cout << "resources if you can. At least Chamoy's predominant ";
        cout << "characteristic is languor, rather than anger.\n\n";

        checkpoint();

        cout << "But even then, you cannot mistake it for passivity. She ";
        cout << "might seem lighthearted, but it is her home that you have ";
        cout << "invaded. She is giving you a way out, feeling sorry for the ";
        cout << "rickety hominin lacking even a reasonable nose print, but ";
        cout << "having failed to prove yourself worthy, away you go.\n\n";
    }

    cout << "Goodbye, and happy haggling!\n\n\n\n";
    exit(0);
}

void Negotiator::acceptTerms() {}


void Negotiator::rejectTerms(int turnsLeft) {
    cout << name << " rejects the offer on the table. Try again, ";
    cout << "but don't forget that you only have " << turnsLeft;
    cout << (turnsLeft == 1 ? " turn" : " turns") << " left ";
    cout << "before the patience of " << name << " runs out.\n\n";
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



// void Negotiator::fillEconomy() {
//     // name: [default price, how many owned]
//     // economy["pomegranate"] = 10.0;
//     // economy["knuckle pads"] = 15.0;
//     // economy["silverback perfume"] = 10.0;
//     // economy["ginger cookie"] = 5.0;
//     // economy["coins"] = 1.0;
//     // economy["basket"] = 2.0;
//     // economy["porridge's key"] = 30.0;
//     // economy["rhubarb's key"] = 40.0;
//     // economy["chamoy's key"] = 50.0;
// }

//
// void Negotiator::fillPreferences() {
//     // Player will take base value
//     if (name == "You") {
//         // prefs["pomegranate"] = 1.0;
//         // prefs["knuckle pads"] = 1.0;
//         // prefs["silverback perfume"] = 1.0;
//         // prefs["ginger cookie"] = 1.0;
//         // prefs["coins"] = 1.0;
//         // prefs["basket"] = 1.0;
//         // prefs["porridge's key"] = 1.0;
//         // prefs["rhubarb's key"] = 1.0;
//         // prefs["chamoy's key"] = 1.0;
//
//         prefs["burn relief ointment"] = 1.0;
//         prefs["carved walking cane"] = 1.0;
//         prefs["sunflower seeds"] = 1.0;
//         prefs["bird spirit key"] = 1.0;
//
//     }
//
//     // else if (name == "Porridge") {
//     //     prefs["pomegranate"] = 1.25; // loves fruit
//     //     prefs["knuckle pads"] = 0.25; // has young, fresh knuckles
//     //     prefs["silverback perfume"] = 2.0; // wants to seem older
//     //     prefs["ginger cookie"] = 1.25; // loves cookies
//     //     prefs["coins"] = 1.0;
//     //     prefs["basket"] = 0.25; // irrelevant
//     //     prefs["porridge's key"] = 1.0;
//     //     prefs["rhubarb's key"] = 0.0; // irrelevant
//     //     prefs["chamoy's key"] = 0.0; // irrelevant
//     // }
//     //
//     // else if (name == "Rhubarb") {
//     //     prefs["pomegranate"] = 1.25; // loves fruit
//     //     prefs["knuckle pads"] = 1.0;
//     //     prefs["silverback perfume"] = 0.5; // is female
//     //     prefs["ginger cookie"] = 0.5; // no nutrition
//     //     prefs["coins"] = 1.5; // cash poor, idea rich
//     //     prefs["basket"] = 1.25; // can never have enough
//     //     prefs["porridge's key"] = 0.0; // irrelevant
//     //     prefs["rhubarb's key"] = 1.0;
//     //     prefs["chamoy's key"] = 0.0; // irrelevant
//     // }
//     //
//     // else if (name == "Chamoy") {
//     //     prefs["pomegranate"] = 1.5; // loves fruit
//     //     prefs["knuckle pads"] = 3.0; // very important in old age!
//     //     prefs["silverback perfume"] = 0.25; // ain't got no time for romance
//     //     prefs["ginger cookie"] = 1.5; // who doesn't love a cookie?
//     //     prefs["coins"] = 0.5; // you can't take it with you
//     //     prefs["basket"] = 1.0; // sure
//     //     prefs["porridge's key"] = 0.0; // irrelevant
//     //     prefs["rhubarb's key"] = 0.0; // irrelevant
//     //     prefs["chamoy's key"] = 1.0;
//     // }
// }


// void Negotiator::acceptTerms() {
//     // if (name == "Porridge") {
//     //     cout << "Porridge grins just with his lips, a sparkle in his ";
//     //     cout << "eyes. \"Yes! I'll take that offer!\" he shouts, ";
//     //     cout << "leaping up and beginning to scoop up his winnings. ";
//     //     cout << "He tosses you his key, and you snatch it out of the ";
//     //     cout << "air.\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "Congratulations, you now have Porridge's Key! You are ";
//     //     cout << "one step toward your escape. Two more to go...\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "Porridge is lost in his own bliss, inspecting his ";
//     //     cout << "winnings, when you ";
//     //     cout << "clear your throat. \"Oh,\" he says, remembering ";
//     //     cout << "you, \"I guess you still have to get past the rest of ";
//     //     cout << "the family, right?\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "You nod, and Porridge says, \"Well, just go back ";
//     //     cout << "through the family room, where Grandma sits, and keep ";
//     //     cout << "on heading through until you get to my Momma's door.\" ";
//     //     cout << "He pauses, checks the clock on the wall, and scratches ";
//     //     cout << "his head. \"Umm, just be careful, okay? If you catch ";
//     //     cout << "her before lunchtime, she can be a little cranky.\n\n";
//     //
//     //     cout << "You give Porridge a high-five and go back out of the ";
//     //     cout << "room and down the hallway.";
//     //
//     //     checkpoint();
//     // }
//     //
//     // if (name == "Rhubarb") {
//     //     cout << "Rhubarb chuckles to herself. \"I don't know why Mother ";
//     //     cout << "only likes giving me things through an intermediary.\" She ";
//     //     cout << "looks up to you. \"I suppose it's her idea of a fun game. ";
//     //     cout << "Ah, well. At least she still gets me the best presents.\"\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "Rhubarb steps up to her full height, out from behind the ";
//     //     cout << "desk, and she steps around, leaning back against it and ";
//     //     cout << "crossing her arms. You hold your breath for a moment, until ";
//     //     cout << "she speaks again.\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "\"Alright, young human. I'll accept your offer. Leave me ";
//     //     cout << "all that is here on the table, and you may have this.\" From ";
//     //     cout << "a pocket, she pulls out a silver key. It stands out sharply ";
//     //     cout << "against the black pads of her palm.\n\n";
//     //
//     //     cout << "Before she can change her mind, you gingerly take the key ";
//     //     cout << "out of her hand.\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "You turn to walk away, and she calls back to you. \"There's ";
//     //     cout << "only one more of us for you to face, you know.\" You turn ";
//     //     cout << "back, waiting for more. She's still smirking, arms crossed. ";
//     //     cout << "\"And it's whom you think it is. A word of advice: she's not ";
//     //     cout << " as easy-going as she might seem. And after all, although ";
//     //     cout << "a couple of you intruders have made it this far, no one has ";
//     //     cout << "finished successfully. You are, after all, notably ";
//     //     cout << "diminished from your efforts, are you not?\"\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "You check your inventory, and indeed, your holdings are a ";
//     //     cout << "far cry from the full basket Chamoy had originally given ";
//     //     cout << "you.\n\n";
//     //
//     //     cout << "Rhubarb laughs again, watching you. This time, it's ";
//     //     cout << "sinister. \"Let's see if you can make your way through ";
//     //     cout << "this one.\" She goes back to her desk and sits down, ";
//     //     cout << "leaning over it and steepling her fingers. \"You know the ";
//     //     cout << "way. Get going. And good luck.\"\n\n";
//     //
//     //     checkpoint();
//     //
//     //     cout << "She laughs again, and the sound of it echoes in your mind as ";
//     //     cout << "you walk back down the hallway, shivering.\n\n";
//     // }
//     //
//     // if (name == "Chamoy") {
//     //     cout << "Chamoy accepts your offer. Nice job! You win! Wooooo! ";
//     //     cout << "[More flavour text to be filled out here.]\n\n";
//     // }
// }

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
    return distribution(generator)/100.0;
}

// COPY: Prints txt script and replaces key symbols with frequent functions
void Negotiator::loadScript(string filename) {
    string filepath = "txt/" + filename + ".txt";

    string output = "";
    string line = "";

	ifstream in;
	in.open(filepath);

	while (getline(in, line)) {
        if (line == "***") {
            cout << output;
            checkpoint();
            output = "";
        } else output = output + "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
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
    economy["sunflower seeds packet"] = 10.0;
    economy["bird spirit key"] = 10.0;
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
    generosityOfOffer = 0.0; // Reset if hadn't already been
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
    if (name == "Mosta and Pepita") loadScript("1/Level1Lose");
    if (name == "Totochtin") loadScript("2/Level2Lose");
    if (name == "Burro") loadScript("3/Level3Lose");
    if (name == "Lepha") loadScript("4/Level4Lose");
    exit(0);
}

void Negotiator::acceptTerms() {
    string acceptance = "";
    bool plural = (name == "Mosta and Pepita" ? true : false);

    acceptance  = name + " accept" + (plural ? " " : "s ");
    acceptance += "the offer on the table.\n\n";

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
        acceptance += "Very, very impressive. You've moved along efficiently ";
        acceptance += "and set yourself up well for the future. Well done!\n";
    } else if (generosityOfOffer >= 0.75 && generosityOfOffer < 1.0) {
        acceptance += "You made a solid offer and won the day. ";
        acceptance += "It's up to you now to continue to manage your stock ";
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

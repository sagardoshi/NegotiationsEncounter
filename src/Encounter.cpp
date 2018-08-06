#include <iostream>
#include <string>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Encounter.h"
#include "../inc/Issue.h"
#include "../inc/Offer.h"

using namespace std;


Encounter::Encounter(PlayerCharacter* pc, Negotiator* opp, int l, int t) :
                     level(l), turns(t), player(pc), opponent(opp) {
        offer = new Offer(); // Create empty offer
}

Encounter::~Encounter()             { delete offer;            }
int Encounter::getLevel()           { return level;                      }
void Encounter::useOneTurn()        { turns--;                           }
void Encounter::printTurns()        { cout << turns << " turns left.\n"; }
void Encounter::printOfferOnTable() { offer->printOffer();               }


void Encounter::loadFloatIssue(Issue<float> issueToAdd) {
    floatIssues.push_back(issueToAdd);
    offer->loadFloatIssue(issueToAdd);
}

void Encounter::loadIntIssue(Issue<int> issueToAdd) {
    intIssues.push_back(issueToAdd);
    offer->loadIntIssue(issueToAdd);
}


int Encounter::issueCount() {
    return (floatIssues.size() + intIssues.size());
}


void Encounter::printIssues() {
    // Currently just going through floats first and then ints
    for (int i = 0; i < floatIssues.size(); i++) floatIssues[i].printIssue();
    for (int j = 0; j < intIssues.size(); j++) intIssues[j].printIssue();
}

// Gets simple, unverified user input and converts to lowercase
string Encounter::saveStandardisedInput(string keyword) {
    // Get standard input (including whitespaces)
    cout << "\n\n--------------------------\n";
    cout << ">>>> ";
    getline(cin, keyword);
    cout << "--------------------------\n\n\n\n";

    // Convert any capitals to lowercase in place
    int i = 0;
    while (keyword[i]) {
        keyword[i] = tolower(keyword[i]);
        i++;
    }

    if (keyword == "1") keyword = "pomegranate";
    if (keyword == "2") keyword = "knuckle pads";
    if (keyword == "3") keyword = "silverback perfume";
    if (keyword == "4") keyword = "ginger cookie";
    if (keyword == "5") keyword = "coins";
    if (keyword == "6") keyword = "basket";

    return keyword;
}


void Encounter::buildValidOffer(map<string, int> econ) {

    string prop;
    bool forProp = true;

    for (int i = 0; i < floatIssues.size(); i++) {
        while (true) {
            prop = "";
            player->printInventory(forProp);
            prop = saveStandardisedInput(prop);

            if (prop == "quit") exit(0);
            else if (prop == "inventory") player->printInventory();
            else if (prop == "help") player->printHelp();
            else if (prop == "on table") offer->printOffer();
            else if (prop == "done") {
                offer->printOffer();

                // Move on to opponent response
                cout << opponent->getName() << "'s response:" << endl;

                bool approval = opponent->reactToOffer(offer);
                if (approval) opponent->acceptTerms();
                else { // Must return items to inventory first
                    player->clearTable(offer);
                    opponent->rejectTerms(turns);
                }
                break;
            }

            // Must be a legit item, though the player may not have it
            else if (player->inventory.count(prop)) {
                if (player->inventory[prop]) {
                    cout << "You place your " << prop << " on the table.";
                    cout << "\n\n";
                    player->placeInvObjOnTable(prop, offer);
                } else {
                    cout << "You have no " << prop << " to give.\n\n";
                }
                offer->printOffer();
            }
            else cout << "You have no such thing. Try again.\n\n";
        }
    }
}


void Encounter::beginEncounter() {

    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);

    cout << "==========================================================\n";
    cout << "NEGOTIATE WITH " << opp << endl;
    cout << "==========================================================\n\n";

    player->printHelp();
}

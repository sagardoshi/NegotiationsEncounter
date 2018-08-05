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
                level(l), turns(t), isPCTurn(true), player(pc), opponent(opp) {

    offerOnTheTable = new Offer(); // Create empty offer
}

Encounter::~Encounter() {
    delete offerOnTheTable;
}

int Encounter::getLevel()       { return level; }

void Encounter::useOneTurn() {
    turns--;
    isPCTurn = !isPCTurn;
}

void Encounter::loadFloatIssue(Issue<float> issueToAdd) {
    floatIssues.push_back(issueToAdd);
    offerOnTheTable->loadFloatIssue(issueToAdd);
}

void Encounter::loadIntIssue(Issue<int> issueToAdd) {
    intIssues.push_back(issueToAdd);
    offerOnTheTable->loadIntIssue(issueToAdd);
}


int Encounter::countIssuesInEncounter() {
    return (floatIssues.size() + intIssues.size());
}

void Encounter::printTurnsLeft() {

    cout << "Total turns left (between both of you): " << turns << endl;
    cout << (isPCTurn ? "It is your turn." : "It is your opponent's turn.") << endl;

}

void Encounter::printEncounterIssues() {
    // Currently just going through floats first and then ints
    for (int i = 0; i < floatIssues.size(); i++) floatIssues[i].printIssue();
    for (int j = 0; j < intIssues.size(); j++) intIssues[j].printIssue();
}

void Encounter::printOfferOnTable() {
    offerOnTheTable->printOffer();
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

    string proposal;
    bool isFirst = true;

    for (int i = 0; i < floatIssues.size(); i++) {
        while (true) {
            proposal = "";

            // First versus subsequent iterations
            if (isFirst) {
                cout << "[What do you propose to give for ";
                cout << floatIssues[i].getName() << "? Or type ";
                cout << " \"done\" if you have finished.]\n";
                isFirst = false;
            } else {
                cout << "[Anything else? Type \"done\" if you have ";
                cout << "finished.]\n";
            }

            cout << "Here's what you have in your inventory:\n\n";
            player->printInventory();


            proposal = saveStandardisedInput(proposal);



            if (proposal == "quit") exit(0);
            else if (proposal == "done") {
                // Move on to opponent response
                cout << "Let us see what " << opponent->getName();
                cout << "'s response is to your offer:" << endl;

                bool approval = opponent->reactToOffer(offerOnTheTable);
                if (approval) opponent->acceptTerms();
                else { // Must return items to inventory first
                    player->clearTable(offerOnTheTable);
                    opponent->rejectTerms();
                }

                break;
            }
            else if (proposal == "inventory") player->printInventory();
            else if (proposal == "help") player->printHelp();
            // Must be a legit item, though the player may not have it
            else if (player->inventory.count(proposal)) {
                if (player->inventory[proposal]) {
                    cout << "You place your " << proposal << " on the table.";
                    cout << "\n\n";
                    player->placeInvObjOnTable(proposal, offerOnTheTable);
                } else {
                    cout << "You have no " << proposal << " to give.\n\n";
                }
            }
            else cout << "You do not have such a thing. Try again.\n\n";
        }

        isFirst = true; // For the next item, ask afresh
    }
}


void Encounter::beginEncounter() {

    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);

    cout << "==========================================================\n";
    cout << "NEGOTIATE WITH " << opp << endl;
    cout << "==========================================================\n\n";

    printTurnsLeft();
}

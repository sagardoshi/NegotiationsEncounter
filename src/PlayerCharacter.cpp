#include <iostream>
#include <string>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a),
                                                      levelReached(0) {

    inventory["burn relief ointment"] = 0;
    inventory["carved walking cane"] = 0;
    inventory["sunflower seeds packet"] = 0;
    inventory["bird spirit key"] = 0;
}

void PlayerCharacter::fillInventory() {
    inventory["burn relief ointment"] = 1;
    inventory["carved walking cane"] = 1;
    inventory["sunflower seeds packet"] = 1;
    inventory["bird spirit key"] = 0;
}


void PlayerCharacter::placeInvObjOnTable(string itemName, Offer* onTable) { inventory[itemName]--;
    onTable->addObjToTable(itemName);
}

void PlayerCharacter::clearTable(Offer* offer) {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;

    for (it = offer->offerInv.begin(); it != offer->offerInv.end(); it++) {
        item = it->first;
        amount = it->second;
        if (amount > 0) {
            inventory[item] += amount;
            offer->offerInv[item] = 0;
        }
    }
}

void PlayerCharacter::printHelp() {
    string helpBorder = "***** HELP *****\n";
    string helpText   = "Add your inventory items by number to your offer\n\n";
           helpText  += "propose: type this to send your current offer\n";
           helpText  += "turns: see how many turns you have left\n";
           helpText  += "inventory: double-check what you currently hold\n";
           helpText  += "help: see this menu\n";
           helpText  += "quit: exit the game\n";
           helpText   = helpBorder + helpText + helpBorder;

    cout << helpText << endl;
}

void PlayerCharacter::printInventory(bool forProposal) {

    // Print appropriate header
    string header, footer = "";

    if (forProposal) {
        header += "***** PROPOSAL *****\n";
        footer  = header;
        header += "[To build an offer, add items individually by number.]\n";
        header += "[Type \"propose\" to finalise and send this offer.]\n\n";
    } else {
        header += "***** INVENTORY *****\n";
        footer =  header;
    }

    cout << header;

    // Local vars for iterator
    map<string, int>::iterator it;
    int numItemsHeld = 0;
    int amount = 0;

    string itemName = "";
    string itemMap = "";
    string listItemText = "";
    string amountText = "";


    // Depending on whether within proposal, print list of items one by one
    // Skip items with <= 0 inventory
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;
        amountText = to_string(amount);
        itemMap = to_string(invMap[itemName]);

        if (amount > 0) {
            numItemsHeld += amount;

            listItemText += "[";
            listItemText += (forProposal ? (itemMap + ": ") : "");
            listItemText += itemName + ", " + amountText + " owned]\n";

            cout << listItemText; // Flush out item and go on to the next one
        }
        listItemText = "";

    }

    // Prepend footer if necessary with zero inventory notice
    if (!numItemsHeld) {
        footer = "You have nothing in your inventory.\n" + footer;
    }
    cout << footer << endl;

}



// void PlayerCharacter::printInventory(bool forProposal) {
    //
    // if (!inventory["pomegranate"] &&
    //     !inventory["knuckle pads"] &&
    //     !inventory["silverback perfume"] &&
    //     !inventory["ginger cookie"] &&
    //     !inventory["coins"] &&
    //     !inventory["basket"]) {
    //     cout << "***************** INVENTORY ****************\n";
    //     cout << "You have nothing in your inventory.\n\n";
    //     cout << "********************************************\n\n";
    //     return;
    // }
    //
    // if (forProposal) {
    //     cout << "****************** PROPOSE *****************\n";
    //     cout << "[To build an offer, add your items by number, one by one.]\n";
    //     cout << "[Type \"done\" when you don't wish to add any more.]\n\n";
    // } else cout << "***************** INVENTORY ****************\n";
    // if (inventory["pomegranate"]) {
    //     cout << (forProposal ? "[1: " : "[") << "Pomegranate, ";
    //     cout << inventory["pomegranate"] << " owned]" << endl;
    // }
    // if (inventory["knuckle pads"]) {
    //     cout << (forProposal ? "[2: " : "[") << "Knuckle Pads, ";
    //     cout << inventory["knuckle pads"] << " owned]" << endl;
    // }
    // if (inventory["silverback perfume"]) {
    //     cout << (forProposal ? "[3: " : "[") << "Silverback Perfume, ";
    //     cout << inventory["silverback perfume"] << " owned]" << endl;
    // }
    // if (inventory["ginger cookie"]) {
    //     cout << (forProposal ? "[4: " : "[") << "Ginger Cookie";
    //     cout << (inventory["ginger cookie"] > 1 ? "s" : "");
    //     cout << ", " << inventory["ginger cookie"] << " owned]" << endl;
    //
    // }
    // if (inventory["coins"]) {
    //     cout << (forProposal ? "[5: " : "[") << "Coin";
    //     cout << (inventory["coins"] > 1 ? "s" : "");
    //     cout << ", " << inventory["coins"] << " owned]" << endl;
    // }
    // if (inventory["basket"]) {
    //     cout << (forProposal ? "[6: " : "[") << "Basket, ";
    //     cout << inventory["basket"] << " owned]" << endl;
    // }
    // cout << "********************************************\n\n";
// }

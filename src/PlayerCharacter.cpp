#include <iostream>
#include <string>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a) {
    initInventory();
}

void PlayerCharacter::initInventory() {
    inventory["burn relief ointment"] = 0;
    inventory["carved walking cane"] = 0;
    inventory["packet of sunflower seeds"] = 0;
    inventory["pulque bottle"] = 0;
    inventory["paint jar"] = 0;
    inventory["morning headache tonic"] = 0;
    inventory["long ear warmers"] = 0;
}

// How many of each item you carry
void PlayerCharacter::fillInventory() {
    inventory["burn relief ointment"] = 1;
    inventory["carved walking cane"] = 1;
    inventory["packet of sunflower seeds"] = 1;
    inventory["pulque bottle"] = 3;
    inventory["paint jar"] = 3;
    inventory["morning headache tonic"] = 1;
    inventory["long ear warmers"] = 1;
}

// Goes through every item player has in an encounter, and assigns a number
void PlayerCharacter::mapPlayerInventory() {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;
    int itemOrder = 1;

    for (it = inventory.begin(); it != inventory.end(); it++) {
        item = it->first;
        amount = it->second;

        if (amount > 0) {
            invMap[item] = itemOrder;
            itemOrder++;
        }
    }
}


void PlayerCharacter::placeInvObjOnTable(string itemName, Offer* onTable) { inventory[itemName]--;
    onTable->addObjToTable(itemName);
}

void PlayerCharacter::clearTable(Offer* offer) {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;

    for (it = offer->inventory.begin(); it != offer->inventory.end(); it++) {
        item = it->first;
        amount = it->second;
        if (amount > 0) {
            inventory[item] += amount;
            offer->inventory[item] = 0;
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
        header += "[Add items individually to the table by their numbers.]\n";
        header += "[When your offer is ready, type \"propose\" ";
        header += "to finalise it.]\n\n";
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

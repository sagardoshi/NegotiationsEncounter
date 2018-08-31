#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a) {
    initInventory();
}

void PlayerCharacter::initInventory() {
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
    inventory["your black trousers"] = 1;
    inventory["your black tunic"] = 1;
}

// How many of each item you carry
void PlayerCharacter::fillInventory() {
    inventory["burn relief ointment"] = 1;
    inventory["carved walking cane"] = 1;
    inventory["sunflower seeds packet"] = 1;
    inventory["pulque flask"] = 2;
    inventory["paint canister"] = 3;
    inventory["morning headache tonic"] = 1;
    inventory["long earmuffs"] = 1;
    inventory["wood varnish bottle"] = 2;
    inventory["waterproof wax jar"] = 1;
    inventory["loose leaf sencha tea"] = 2;
    inventory["vinegar disinfectant"] = 4;
    inventory["your black trousers"] = 1; // these were already added
    inventory["your black tunic"] = 1; // but just repeating to avoid mistakes
}


float PlayerCharacter::getInvValue() {
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


void PlayerCharacter::placeInvObjOnTable(string itemName, Negotiator* offer) {
    inventory[itemName]--; // Take item out of bag
    offer->inventory[itemName]++; // Place it on table
}

void PlayerCharacter::takeBackOffer(Negotiator* offer) {
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

void PlayerCharacter::printStrategy() {
    string helpBorder = "***** STRATEGY HELP *****\n";
    string helpText  = "The spirits are fickle. ";
           helpText += "To win their favour, offer them a package of ";
           helpText += "as many\n";
           helpText += "or as few items as you wish.\n\n";

           helpText += "Be as efficient as possible. You can ";
           helpText += "see the market value of ";
           helpText += "your items, but\n";
           helpText += "each spirit ";
           helpText += "values each item uniquely, based on their needs ";
           helpText += "and their\n";
           helpText += "personality. Some spirits are just strict by nature. ";
           helpText += "Others may be more willing\n";
           helpText += "to accept ";
           helpText += "weaker offers. It's up to you to predict what might ";
           helpText += "appeal most to\n";
           helpText += "your opponent, while ";
           helpText += "keeping a watchful eye on your own stock.\n\n";

           helpText += "Remember: you have only a few tries before they lose ";
           helpText += "patience only a finite\n";
           helpText += "inventory. The more you retain by the end, ";
           helpText += "the better you will score. Good luck.\n";
           helpText  = helpBorder + helpText + helpBorder;

    cout << helpText << endl;
}

void PlayerCharacter::printHelp() {
    string optBorder = "***** HELP *****\n";
    string optText   = "Add items one by one to the table with their ";
           optText  += "corresponding number on the left.\n";
           optText  += "When your offer is ready, type \"propose\" ";
           optText  += "to finalise it.\n\n";

           optText  += "propose:  type this to send your current offer\n";
           optText  += "cancel:   take your current offer off the table\n";
           optText  += "turns:    see how many turns you have left\n";
           optText  += "strategy: see strategy hints\n";
           optText  += "help:     see this menu\n";
           optText  += "forfeit:  admit defeat\n";
           optText  += "quit:     immediately exit the game\n";
           optText   = optBorder + optText + optBorder;

    cout << optText << endl;
}

void PlayerCharacter::printInventory(bool firstPrint, int level) {

    // Print appropriate header
    string border = "----- YOUR LOOT -----\n";
    string header = border;
    string footer = border;

    string extraHelp  = "Add items to the table by the number on the left.\n";
           extraHelp += "When ready, type \"propose\" to finalise your offer ";
           extraHelp += "or \"cancel\" to undo it.\n\n";

    // Always print at start of encounter and throughout for the tutorial
    if (firstPrint || (level == 0)) {
        header += extraHelp;
        firstPrint = false;
    }

    cout << header;

    // Local vars for iterator
    map<string, int>::iterator it;

    const int MAX_ITEM_LEN = 22;
    string itemName = "";
    string itemText = "";

    int numItemsHeld = 0;
    int amount = 0;
    string amountText = "";

    int itemOrder = 0;
    string itemOrderText = "";

    int itemValue = 0;
    string itemValueText = "";




    // Depending on whether within proposal, print list of items one by one
    // Skip items with <= 0 inventory
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;
        amountText = to_string(amount);
        itemOrder = invMap[itemName];
        itemOrderText = to_string(itemOrder);
        itemValue = economy[itemName];
        itemValueText = to_string(itemValue);


        if (amount > 0) {
            numItemsHeld += amount;

            // First add itemOrder, if appropriate, keeping aligned
            if (itemOrder) {
                if (itemOrder < 10) itemText += " " + itemOrderText + ": ";
                else itemText += itemOrderText + ": ";
            }

            // Then add itemName with extra spaces at end for alignment
            itemText += itemName;
            int gap = MAX_ITEM_LEN - itemName.length();
            if (gap > 0) {
                for (int i = 0; i < gap; i++) itemText += " ";
            }

            itemText += " ... ";

            // Next add amount, with extra spaces for alignment
            if (amount < 10) itemText += " " + amountText + " owned";
            else itemText += amountText + " owned";

            itemText += " ... ";

            // Finally add itemValue, again with spaces for alignment
            if (itemValue < 10) itemText += " £" + itemValueText;
            else itemText += "£" + itemValueText;
            itemText += " base market value per unit";

            itemText += "\n";

            // At last, flush out itemText, empty it, and go on to next
            cout << itemText;
            itemText = "";
        }

    }

    string total = "";
    // Print totals or empty notice
    if (numItemsHeld) {
        total += "\n Loot Market Value: ";
        total += "£" + toPreciseString(getInvValue()) + "\n";
    } else footer = "[Empty]\n" + footer;

    cout << total << footer << endl;
}

void PlayerCharacter::score(float startVal, float offerVal, float endVal) {

    string border = "***** ENCOUNTER SCORE *****\n";
    string scoreText  = "You began with an inventory of market value: ";
           scoreText += (startVal >= 10 ? "" : " ");
           scoreText += "£" + toPreciseString(startVal) + "\n";
           scoreText += "You gave away a total market value of:       ";
           scoreText += (offerVal >= 10 ? "" : " ");
           scoreText += "£" + toPreciseString(offerVal) + "\n";
           scoreText += "You ended with an inventory of market value: ";
           scoreText += (endVal >= 10 ? "" : " ");
           scoreText += "£" + toPreciseString(endVal) + "\n";

    cout << border << scoreText << border << endl;
}

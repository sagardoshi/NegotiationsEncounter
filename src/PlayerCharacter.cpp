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

    inventory["pomegranate"] = 0;
    inventory["knuckle pads"] = 0;
    inventory["silverback perfume"] = 0;
    inventory["ginger cookie"] = 0;
    inventory["coins"] = 0;
    inventory["basket"] = 0;
}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

void PlayerCharacter::fillInventory() {
    inventory["pomegranate"] = 1;
    inventory["knuckle pads"] = 1;
    inventory["silverback perfume"] = 1;
    inventory["ginger cookie"] = 4;
    inventory["coins"] = 30;
    inventory["basket"] = 1;
}

void PlayerCharacter::addToInventory(string itemName, int quantity) {
    inventory[itemName] = quantity;
}

void PlayerCharacter::removeFromInventory(string itemName) {
    inventory[itemName] = 0;
}

void PlayerCharacter::placeInvObjOnTable(string itemName, Offer* onTable) { inventory[itemName]--;
    onTable->addObjToTable(itemName);
}

void PlayerCharacter::clearTable(Offer* table) {
    map<string, int>::iterator it;
    for (it = table->inventory.begin(); it != table->inventory.end(); it++) {
        if (it->second > 0) {
            table->inventory[it->first] = 0;
            addToInventory(it->first, it->second);
        }
    }
}

void PlayerCharacter::printHelp() {
    cout << "******************* GENERAL HELP *******************\n";
    cout << "[Type \"help\" to see this menu.]\n";
    cout << "[Type \"quit\" to quit the game.]\n";
    cout << "[Type \"inventory\" to see what you currently hold.]\n";
    cout << "[Type \"negotiate\" when prompted to start a level.]\n";
    cout << "****************************************************\n\n";

    cout << "***************** NEGOTIATION HELP *****************\n";
    cout << "[DURING NEGOTIATION: type \"see turns left\" to see how many ";
    cout << "rounds remain to conclude the negotiation.]\n";

    cout << "[DURING NEGOTIATION: type \"see issues\" to see all ";
    cout << "issues on the table, along with their min and max possible ";
    cout << "values.]\n";

    cout << "[DURING NEGOTIATION: type \"see current offer\" to see the ";
    cout << "current offer on the table.]\n";

    cout << "[DURING NEGOTIATION: type \"propose offer\" to use a turn ";
    cout << "to build your own offer.]\n";

    // cout << "[DURING NEGOTIATION: type \"accept terms\" to confirm that you ";
    // cout << "are willing to take the offer currently on the table.]\n";
    //
    // cout << "[DURING NEGOTIATION: type \"walk away\" if you see no possible ";
    // cout << "solution to this negotiation.]\n";
    cout << "****************************************************\n\n";
}

void PlayerCharacter::printInventory() {
    cout << "***************** INVENTORY ****************\n";
    if (!inventory["pomegranate"] &&
        !inventory["knuckle pads"] &&
        !inventory["silverback perfume"] &&
        !inventory["ginger cookie"] &&
        !inventory["coins"] &&
        !inventory["basket"]) {
        cout << "You have nothing in your inventory.\n\n";
        cout << "********************************************\n\n";
        return;
    }

    cout << "[You may add the following items from your inventory to the ";
    cout << "table one by one by typing the associated number on the left:] \n";
    if (inventory["pomegranate"]) {
        cout << "[1: Pomegranate, " << inventory["pomegranate"];
        cout << " owned]" << endl;
    }
    if (inventory["knuckle pads"]) {
        cout << "[2: Knuckle Pads, " << inventory["knuckle pads"];
        cout << " owned]" << endl;
    }
    if (inventory["silverback perfume"]) {
        cout << "[3: Silverback Perfume, ";
        cout << inventory["silverback perfume"] << " owned]" << endl;
    }
    if (inventory["ginger cookie"]) {
        cout << "[4: Ginger Cookie";
        cout << (inventory["ginger cookie"] > 1 ? "s" : "");
        cout << ", " << inventory["ginger cookie"] << " owned]" << endl;

    }
    if (inventory["coins"]) {
        cout << "[5: Coin";
        cout << (inventory["coins"] > 1 ? "s" : "");
        cout << ", " << inventory["coins"] << " owned]" << endl;
    }
    if (inventory["basket"]) {
        cout << "[6: Basket, " << inventory["basket"];
        cout << " owned]" << endl;
    }

    cout << "********************************************\n\n";

}

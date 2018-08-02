#include <iostream>
#include <string>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Inventory.h"
#include "../inc/Offer.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a),
                                                      levelReached(0) {

    inventory["pomegranate"] = make_pair(10, 0);
    inventory["knuckle pads"] = make_pair(15, 0);
    inventory["silverback perfume"] = make_pair(10, 0);
    inventory["ginger cookie"] = make_pair(1, 0);
    inventory["coin purse"] = make_pair(5, 0);
    inventory["basket"] = make_pair(2, 0);
}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

void PlayerCharacter::fillInventory() {
    inventory["pomegranate"] = make_pair(10, 1);
    inventory["knuckle pads"] = make_pair(15, 1);
    inventory["silverback perfume"] = make_pair(10, 1);
    inventory["ginger cookie"] = make_pair(1, 4);
    inventory["coin purse"] = make_pair(5, 30);
    inventory["basket"] = make_pair(2, 1);
}

void PlayerCharacter::addToInventory(string itemName, int value, int quantity) {
    inventory[itemName] = make_pair(value, quantity);
}

void PlayerCharacter::removeFromInventory(string itemName) {
    inventory[itemName].second = 0;
}

void PlayerCharacter::placeInvObjOnTable(string itemName, Offer* onTable) { inventory[itemName].second--;
    onTable->addObjToTable(itemName);

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

    cout << "[DURING NEGOTIATION: type \"propose offer\" to make your own ";
    cout << "offer. You will be prompted for a value for each issue.]\n";

    cout << "[DURING NEGOTIATION: type \"accept terms\" to confirm that you ";
    cout << "are willing to take the offer currently on the table.]\n";

    cout << "[DURING NEGOTIATION: type \"walk away\" if you see no possible ";
    cout << "solution to this negotiation.]\n";
    cout << "****************************************************\n\n";
}

void PlayerCharacter::printInventory() {
    cout << "***************** INVENTORY ****************\n";
    if (!inventory["pomegranate"].second &&
        !inventory["knuckle pads"].second &&
        !inventory["silverback perfume"].second &&
        !inventory["ginger cookie"].second &&
        !inventory["coin purse"].second &&
        !inventory["basket"].second) {
        cout << "You have nothing in your inventory.\n\n";
        cout << "********************************************\n\n";
        return;
    }

    cout << "In your inventory, you have: \n";
    if (inventory["pomegranate"].second) {
        cout << "--" << inventory["pomegranate"].second << " pomegranate\n";
    }
    if (inventory["knuckle pads"].second) {
        cout << "--" << inventory["knuckle pads"].second << " set of knuckle pads\n";
    }
    if (inventory["silverback perfume"].second) {
        cout << "--" << inventory["silverback perfume"].second << " bottle of silverback perfume\n";
    }
    if (inventory["ginger cookie"].second) {
        cout << "--" << inventory["ginger cookie"].second << " ginger cookie";
        cout << (inventory["ginger cookie"].second > 1 ? "s" : "") << endl;
    }
    if (inventory["coin purse"].second) {
        cout << "--1 coin purse with " << inventory["coin purse"].second;
        cout << " coin" << (inventory["coin purse"].second == 1 ? "" : "s");
        cout << endl;
    }
    if (inventory["basket"].second) {
        cout << "--and 1 basket to put it all in.\n";
    }

    cout << "********************************************\n\n";

}

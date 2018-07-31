#include <iostream>
#include <string>
#include <utility>
#include <map>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Inventory.h"

using namespace std;

PlayerCharacter::PlayerCharacter(string n, float a) : Negotiator(n, a),
                                                      levelReached(0) {
    // inventory.hasPomegranate = false;
    // inventory.hasKnucklePads = false;
    // inventory.hasSilverbackPerfume = false;
    // inventory.hasBasket = false;
    // inventory.hasGingerCookie = false;
    // inventory.hasMoneySack = false;
    // inventory.money = 0;

    // inventory.insert("Pomegranate", make_pair(10, 0)));
    // inventory.insert("Knuckle Pads", make_pair(15, 0)));
    // inventory.insert("Silverback Perfume", make_pair(10, 0)));
    // inventory.insert("Ginger Cookie", make_pair(5, 0)));
    // inventory.insert("Coin Purse", make_pair(5, 0)));
    // inventory.insert("Basket", make_pair(2, 0)));

    // inventory["Pomegranate"] = pomInv{10, 0};
    // inventory["Knuckle Pads"] = knuckInv{15, 0};
    // inventory["Silverback Perfume"] = silvInv{10, 0};
    // inventory["Ginger Cookie"] = gingInv{5, 0};
    // inventory["Coin Purse"] = coinInv{5, 0};
    // inventory["Basket"] = baskInv{2, 0};

    inventory["Pomegranate"] = make_pair(10, 0);
    inventory["Knuckle Pads"] = make_pair(15, 0);
    inventory["Silverback Perfume"] = make_pair(10, 0);
    inventory["Ginger Cookie"] = make_pair(1, 0);
    inventory["Coin Purse"] = make_pair(5, 0);
    inventory["Basket"] = make_pair(2, 0);
}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

void PlayerCharacter::fillInventory() {
    inventory["Pomegranate"] = make_pair(10, 1);
    inventory["Knuckle Pads"] = make_pair(15, 1);
    inventory["Silverback Perfume"] = make_pair(10, 1);
    inventory["Ginger Cookie"] = make_pair(1, 4);
    inventory["Coin Purse"] = make_pair(5, 30);
    inventory["Basket"] = make_pair(2, 1);
}


void PlayerCharacter::printInventory() {
    cout << "***************** INVENTORY ****************\n";
    if (!inventory["Pomegranate"].second &&
        !inventory["Knuckle Pads"].second &&
        !inventory["Silverback Perfume"].second &&
        !inventory["Ginger Cookie"].second &&
        !inventory["Coin Purse"].second &&
        !inventory["Basket"].second) {
        cout << "You have nothing in your inventory.\n\n";
        cout << "********************************************\n\n";
        return;
    }

    cout << "In your inventory, you have: \n";
    if (inventory["Pomegranate"].second) {
        cout << "--" << inventory["Pomegranate"].second << " Pomegranate\n";
    }
    if (inventory["Knuckle Pads"].second) {
        cout << "--" << inventory["Knuckle Pads"].second << " Set of Knuckle Pads\n";
    }
    if (inventory["Silverback Perfume"].second) {
        cout << "--" << inventory["Silverback Perfume"].second << " Bottle of Silverback Perfume\n";
    }
    if (inventory["Ginger Cookie"].second) {
        cout << "--" << inventory["Ginger Cookie"].second << " Ginger Cookie(s)\n";
    }
    if (inventory["Coin Purse"].second) {
        cout << "--1 Coin Purse with " << inventory["Coin Purse"].second << " coins\n";
    }
    if (inventory["Basket"].second) {
        cout << "--and 1 Basket to put it all in.\n";
    }

    cout << "********************************************\n\n";

}

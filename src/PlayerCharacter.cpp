#include <iostream>
#include <string>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"
#include "../inc/Inventory.h"

using namespace std;

PlayerCharacter::PlayerCharacter(float a) : Negotiator(a), levelReached(0) {
    inventory.hasPomegranate = false;
    inventory.hasKnucklePads = false;
    inventory.hasSilverbackPerfume = false;
    inventory.hasBasket = false;
    inventory.numGingerCookies = 0;
}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

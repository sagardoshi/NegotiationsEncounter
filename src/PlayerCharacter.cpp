#include <iostream>
#include <string>

#include "../inc/PlayerCharacter.h"
#include "../inc/Negotiator.h"

using namespace std;

PlayerCharacter::PlayerCharacter(float a) : Negotiator(a), levelReached(0) {}

void PlayerCharacter::enterNegotiation() {
    cout << "Player has entered encounter (this is just a printout).\n" << endl;
}

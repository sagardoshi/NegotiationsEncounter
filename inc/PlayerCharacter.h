#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Negotiator.h"

class PlayerCharacter: public Negotiator {

private:
    int levelReached;

public:
    PlayerCharacter(float);
    void enterNegotiation();

};

#endif

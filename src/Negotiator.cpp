#include <iostream>
#include <string>

#include "../inc/Negotiator.h"

using namespace std;

Negotiator::Negotiator(float a) : amiability(a) {}

float Negotiator::getAmiability() { return amiability; }

void Negotiator::setAmiability(float newAmiability) {
    amiability = newAmiability;
}

void Negotiator::walkAway() {}

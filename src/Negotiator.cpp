#include <iostream>
#include <string>

#include "../inc/Negotiator.h"

using namespace std;

Negotiator::Negotiator(string n, float a) : name(n), amiability(a) {}

string Negotiator::getName()                { return name;        }
float Negotiator::getAmiability()           { return amiability;  }
void Negotiator::setAmiability(float newAm) { amiability = newAm; }


void Negotiator::walkAway() {}

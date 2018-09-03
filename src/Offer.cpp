#include "../inc/Negotiator.h"
#include "../inc/Offer.h"

#include <iostream>
#include <string>
#include <map>

using namespace std;

// Init required issue numbers to 0
Offer::Offer(string n, float a) : Negotiator(n, a) {}

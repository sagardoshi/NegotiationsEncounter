#include "../inc/Inventory.h"


Inventory::Inventory() : latestInvValue(0.0) {
    initEconomy();
}

////////////////////////////////////////////////////////////////////////////////
// Economy Methods
////////////////////////////////////////////////////////////////////////////////

string Inventory::toPreciseString(float input, int precision) {
    string asString = to_string(input);

    int decimalIndex = asString.find(".");
    int cutoff = decimalIndex + 1 + precision;

    // Don't include decimal if zero floats desired
    if (precision == 0) return asString.substr(0, decimalIndex);
    else return asString.substr(0, cutoff);
}

void Inventory::initEconomy() {

    // 1) Mosta the Stork/Pepita the Pigeon -- spirits of inquisitiveness
    economy["burn relief ointment"] = 7;
    economy["carved walking cane"] = 10;
    economy["sunflower seeds packet"] = 3;

    // 2) Toto the #2 Rabbit of Centzon Totochtin -- spirit of drunkenness
    economy["pulque flask"] = 4;
    economy["paint canister"] = 2;
    economy["morning headache tonic"] = 8;
    economy["long earmuffs"] = 5;


    // 3) Burro the night river beaver -- spirit of industriousness
    economy["wood varnish bottle"] = 6;
    economy["waterproof wax jar"] = 8;

    // 4) Lepha the Elephant -- pharmacologist to the spirits
    economy["loose leaf sencha tea"] = 9;
    economy["vinegar disinfectant"] = 3;

    // 0) Tutorial Lepha
    economy["personal black trousers"] = 2;
    economy["personal black tunic"] = 1;

}

bool Inventory::knowsOfItem(string item) {
    return ((economy.count(item) > 0) ? true : false); // If item exists
}


////////////////////////////////////////////////////////////////////////////////
// Inventory Methods
////////////////////////////////////////////////////////////////////////////////

void Inventory::initInventory() {
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
    inventory["personal black trousers"] = 0;
    inventory["personal black tunic"] = 0;
}

bool Inventory::isEmpty() {
    return (getInvCount() ? false : true);
}

bool Inventory::hasItem(string item) {
    return ((inventory[item] > 0) ? true : false); // At least one
}

int Inventory::getInvCount() {
    map<string, int>::iterator it;
    int total = 0, amount = 0;
    for (it = inventory.begin(); it != inventory.end(); it++) {
        amount = it->second;
        total += amount;
    }
    return total;
}

float Inventory::getInvValue() {
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

    latestInvValue = totalValue; // Update latestInvValue;

    return latestInvValue;
}


void Inventory::printInv(map<string, int>* mapPtr) {

    // Local vars for iterator
    map<string, int>::iterator it;

    const int MAX_ITEM_LEN = 23;
    string itemName = "";
    string itemText = "";

    int numItemsHeld = 0;
    int amount = 0;
    string amountText = "";

    int itemOrder = 0;
    bool printingTable = (mapPtr == NULL); // == if invMap passed
    string itemOrderText = "";

    int itemValue = 0;
    string itemValueText = "";

    // Print appropriate header

    string header  = "----- ";
           header += (!printingTable ? "YOUR LOOT" : "THE TABLE");
           header += " -----\n";
    string footer = header;

    // Depending on whether within proposal, print list of items one by one
    // Skip items with <= 0 inventory
    for (it = inventory.begin(); it != inventory.end(); it++) {
        itemName = it->first;
        amount = it->second;
        amountText = to_string(amount);
        itemOrder = ((mapPtr == NULL) ? 0 : (*mapPtr)[itemName]);
        itemOrderText = to_string(itemOrder);
        itemValue = economy[itemName];
        itemValueText = to_string(itemValue);


        if (amount > 0) {
            numItemsHeld += amount;

            // First add itemOrder, if appropriate, keeping aligned
            if (!printingTable) {
                if (itemOrder < 10) itemText += " " + itemOrderText + ": ";
                else itemText += itemOrderText + ": ";
            } else itemText += "    "; // This only prints for table, not inv

            // Then add itemName with extra spaces at end for alignment
            itemText += itemName;
            int gap = MAX_ITEM_LEN - itemName.length();
            if (gap > 0) {
                for (int i = 0; i < gap; i++) itemText += " ";
            }

            itemText += " ... ";

            // Next add amount, with extra spaces for alignment
            itemText += (amount < 10 ? " " + amountText : amountText);
            itemText += (printingTable ? " given" : " owned");

            itemText += " ... ";

            // Finally add itemValue, again with spaces for alignment
            itemText += (itemValue < 10 ? " " : "");
            itemText += "£" + itemValueText + " base market value per unit\n";
        }
        // Now loop
    }

    string total = "\n";
    // Print totals or empty notice
    if (numItemsHeld) {
        total += (!printingTable ? " Loot" : "Table");
        total += " Market Value: £" + toPreciseString(getInvValue()) + "\n";
    } else total = "\n[Empty]\n\n";

    // Space between two inventories
    footer += (printingTable ? "\n" : "");

    cout << header << itemText << total << footer; // Flush everything
}

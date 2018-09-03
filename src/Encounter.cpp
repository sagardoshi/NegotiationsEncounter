#include "../inc/PlayerCharacter.h"
#include "../inc/Encounter.h"

#include <iostream>


Encounter::Encounter(PlayerCharacter* pc, Negotiator* opp,
                     int l, int t, float kV) :
                     isPrologue(l == 0), turns(t), keyValue(kV),
                     player(pc), opponent(opp) {

        table = new Inventory(); // Table has no name, no personality
        setInventoryForEncounter(); // Order mapping + saving initial value
}

Encounter::~Encounter()             { delete table;        }


////////////////////////////////////////////////////////////////////////////////
// Item order methods (and other setup)
////////////////////////////////////////////////////////////////////////////////

// Goes through every item player has in an encounter, and assigns a number
void Encounter::mapPlayerInventory() {
    map<string, int>::iterator it;
    string item = "";
    int amount = 0;
    int itemOrder = 1;

    for (it = player->inventory.begin(); it != player->inventory.end(); it++) {
        item = it->first;
        amount = it->second;

        if (amount > 0) {
            invMap[item] = itemOrder;
            itemOrder++;
        }
    }

}

// New inventory order map for each encounter
void Encounter::setInventoryForEncounter() {
    mapPlayerInventory(); // Order all the player's inventory for encounter
    opponent->resetGenerosity(); // Ensure no offer at start of encounter

    // Calculate open market value of inventory at start of encounter
    startInvValue = player->getInvValue();
    offerInvValue = 0.0; // init to zero
    endInvValue = 0.0; // init to zero
}


////////////////////////////////////////////////////////////////////////////////
// Header printout methods
////////////////////////////////////////////////////////////////////////////////

// Get opponent name in all caps
string Encounter::getCapsName() {
    string opp = opponent->getName();
    transform(opp.begin(), opp.end(), opp.begin(), ::toupper);
    return opp;
}

// Center text, assuming 80 char wide
void Encounter::centerText(string title, string &toAdd) {
    const int WIDTH = 80;
    int len = title.length();
    int gap = WIDTH - len;

    if (len > WIDTH) return; // Not applicable in this case

    if (gap % 2 == 0) { // If even
        toAdd += string(gap/2, ' ') + title + string(gap/2, ' ') + "\n";
    } else { // If odd, add a space after title, and use decremented gap
        gap--;
        toAdd += string(gap/2, ' ') + title + string(gap/2, ' ') + " \n";
    }
}

// Somewhat pathetic helper function
void Encounter::clearScreen() {
    const int BIG_NUMBER = 100;
    string clear = "";
    for (int i = 0; i < BIG_NUMBER; i++) {
        clear += "\n";
    }
    cout << clear;
}

// Prints centered, capitalised title (with opponent name))
void Encounter::printTitle(string textToSay) {
    const int WIDTH = 80;

    string title  = "\n\n" + string(WIDTH, '=') + "\n";
           centerText(textToSay, title);
           title += string(WIDTH, '=') + "\n\n\n";

    cout << title;
}


////////////////////////////////////////////////////////////////////////////////
// User entry methods... repetitive to NegotiationsMain due to unique printouts
////////////////////////////////////////////////////////////////////////////////

// Converts a string fully to lowercase (in place)
void Encounter::lower(string &anyString) {
    int i = 0;
    while (anyString[i]) {
        anyString[i] = tolower(anyString[i]);
        i++;
    }
}

// Removes whitespaces in string (in place)
void Encounter::removeWS(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

// Verifies that string is a number by iterating through each char of string
bool Encounter::isNum(string &input) {
    string::const_iterator it = input.begin();

    // Advance iterator as long as every character in string is a digit
    while (it != input.end() && isdigit(*it)) ++it;

    // Return true if not empty AND iterator has reached the end
    return ( !input.empty() && (it == input.end()) );
}

// Takes int input and resaves it as appropriate string mapping
void Encounter::remapKeyword(string &keyword) {
    if (!isNum(keyword)) return; // Only do this for number inputs

    // Keyword should come in as number
    int keyNumber = stoi(keyword);

    // Re-save it as the appropriate string
    map<string, int>::iterator it;
    string currentItem = "";
    int currentOrder;

    for (it = invMap.begin(); it != invMap.end(); it++) {
        currentItem = it->first;
        currentOrder = it->second;

        // If the invMap leads to the player's entry, save that string mapping
        if (currentOrder == keyNumber) {
            keyword = currentItem;
            return;
        }
    }
}

// Gets simple, unverified user input and converts to lowercase
void Encounter::userEntry(string &keyword, string &echo, string prompt) {
    // This creates the user prompt
    printTitle(prompt);

    // Print echo below prompt but above entry area
    if (echo != "") cout << echo;
    echo = ""; // Reset to blank either way

    // Take in and process user entry
    cout << ">>>> ";
    getline(cin, keyword);
    lower(keyword);
    removeWS(keyword);
    remapKeyword(keyword); // In case user put item num, switch to item string
}


////////////////////////////////////////////////////////////////////////////////
// UI prints and menu prints
////////////////////////////////////////////////////////////////////////////////

void Encounter::printTurns() {
    string turnsText  = "You have " + to_string(turns) + " turns left before ";
           turnsText += opponent->getName() + "'s patience runs out.\n";

    cout << turnsText;
}

void Encounter::printStrategy() {
    string helpText  = "The spirits are fickle. ";
           helpText += "To win their favour, offer them a package of ";
           helpText += "as many\n";
           helpText += "or as few items as you wish.\n\n";

           helpText += "Be as efficient as possible. You can ";
           helpText += "see the market value of ";
           helpText += "your items, but\n";
           helpText += "each spirit ";
           helpText += "values each item uniquely, based on their needs ";
           helpText += "and their\n";
           helpText += "personality. Some spirits are just strict by nature. ";
           helpText += "Others may be more willing\n";
           helpText += "to accept ";
           helpText += "weaker offers. It's up to you to predict what might ";
           helpText += "appeal most to\n";
           helpText += "your opponent, while ";
           helpText += "keeping a watchful eye on your own stock.\n\n";

           helpText += "Remember: you have only a few tries before they lose ";
           helpText += "patience only a finite\n";
           helpText += "inventory. The more you retain by the end, ";
           helpText += "the better you will score. Good luck.\n";

    cout << helpText;
}

void Encounter::printHelp() {
    string optText  = "";
           optText += "Add items one by one to the table with their ";
           optText += "corresponding number on the left.\n";
           optText += "When ready, type \"propose\" to finalise your offer ";
           optText += "or \"cancel\" to undo it.\n\n";

           optText += "propose:  type this to send your current offer\n";
           optText += "cancel:   take your current offer off the table\n\n";

           optText += "turns:    see how many turns you have left\n";
           optText += "strategy: see strategy hints\n";
           optText += "help:     see this menu\n";
           optText += "<RETURN>: dismiss help / go to negotiating table\n\n";

           optText += "forfeit:  admit defeat\n";
           optText += "quit:     immediately exit the game\n";

    cout << optText;
}

// Packages table and inventory printouts
void Encounter::printUI() {
    table->printInv(); // Prints every time after title
    player->printInv(&invMap);
}


////////////////////////////////////////////////////////////////////////////////
// Handle end of encounter
////////////////////////////////////////////////////////////////////////////////

// Set flags if encounter over; move to next proposal if not yet over
bool Encounter::encounterIsOver(bool &didWin) {
    // Fill win with true or false, depending on acceptance
    didWin = opponent->reactToOffer(table, keyValue);

    // In case over, remember inventory
    endInvValue = player->getInvValue();

    // If won or if didn't win and no turns left
    if (didWin || (!didWin && turns <= 1)) return true;
    else { // but turns are left, must return items to inventory and continue
        turns--; // Use one turn
        player->takeBackOffer(table);
        opponent->rejectTerms(turns);
        return false;
    }
}

void Encounter::printScore() {
    string startedWith = player->toPreciseString(startInvValue);
    string gaveAway = player->toPreciseString(offerInvValue);
    string endedWith = player->toPreciseString(endInvValue);

    string border = "***** ENCOUNTER SCORE *****\n";
    string scoreText  = "You began with an inventory of market value: ";
           scoreText += (startInvValue >= 10 ? "" : " ");
           scoreText += "£" + startedWith + "\n";
           scoreText += "You gave away a total market value of:       ";
           scoreText += (offerInvValue >= 10 ? "" : " ");
           scoreText += "£" + gaveAway + "\n";
           scoreText += "You ended with an inventory of market value: ";
           scoreText += (endInvValue >= 10 ? "" : " ");
           scoreText += "£" + endedWith + "\n";

    cout << border << scoreText << border << endl;
}

// Print end title, score, and load correct scripts
void Encounter::handleEnd(bool didWin) {
    clearScreen(); // To remove the "YOU ARE NEGOTIATING VS." title

    string textToSay = "";
    if (didWin) textToSay = "YOU WON AGAINST " + getCapsName();
    else textToSay = "YOU LOST AGAINST " + getCapsName();

    printTitle(textToSay);
    if (didWin) {
        printScore();
        opponent->acceptTerms();
    } else player->takeBackOffer(table); // Returns stuff to you if loss ≠ end
}

float Encounter::getFinalInvValue() { return endInvValue;  }


////////////////////////////////////////////////////////////////////////////////
// Primary method of the class
////////////////////////////////////////////////////////////////////////////////

// Runs full encounter and only returns false if quit was requested
bool Encounter::runEncounter(bool &didWin) {
    string title = "YOU ARE NEGOTIATING WITH " + getCapsName();
    string promptWithUI = "Negotiate! (type \"help\" for the menu)";
    string promptWithoutUI = "Hit <RETURN> to see the negotiating table";
    string promptToPrint = "";

    string entry = "";
    string echo = "";
    bool firstLoop = true;
    bool uiOnScreen = true;

    while (true) {

        ////////////////////////////////////////////////////////////////////////
        // FIRST HANDLE UI AND PRINTOUTS
        ////////////////////////////////////////////////////////////////////////

        if (firstLoop) {
            firstLoop = false;
            clearScreen();                // To remove incorrect title
            printTitle(title);            // first need an immediate print

            // Special settings just for prologue's first loop
            if (isPrologue) {
                printStrategy();          // Just on firstLoop
                promptToPrint = promptWithoutUI;
            } else {
                printUI();                // After prologue, print UI, normally
                promptToPrint = promptWithUI;
            }
        }
        else {                            // For every time apart from the first
            promptToPrint = promptWithUI; // Assume UI visible
            if (uiOnScreen) printUI();
            else {                        // If on a menu screen
                uiOnScreen = true;
                promptToPrint = promptWithoutUI;
            }
        }

        // Print prompt, take user input, and convert number to item string
        userEntry(entry, echo, promptToPrint);


        // For the next screen, clear current view and start with title of next
        clearScreen();                    // Critical in this position
        if (!firstLoop) printTitle(title);



        ////////////////////////////////////////////////////////////////////////
        // NEXT HANDLE POSSIBLE USER ENTRIES
        ////////////////////////////////////////////////////////////////////////

        ///// IMMEDIATE QUIT
        if      (entry == "quit") {
            player->takeBackOffer(table); // In case quit typed halfway through
            return false;
        }

        ///// FOR NEXT THREE, HIDE UI AND PRINT ACCORDINGLY
        else if (entry == "strategy") {
            uiOnScreen = false;
            printStrategy();
        }
        else if (entry == "help") {
            uiOnScreen = false;
            printHelp();
        }
        else if (entry == "turns") {
            uiOnScreen = false;
            printTurns();
        }

        ///// EMPTY STRING RETURNS TO DEFAULT UI VIEW
        else if (entry == "" || entry == "back") continue; // loop empty input

        ///// NEXT TWO HANDLE IMMEDIATE ENCOUNTER ENDS
        else if (entry == "super") { // Superuser access, not shown in menu
            didWin = true;
            handleEnd(didWin);
            return true;
        }
        else if (entry == "forfeit") {
            didWin = false; // don't check for encounter end; just assume loss
            handleEnd(didWin);
            return true;
        }
        else if (entry == "propose" || entry == "proposal") {
            // Save current value, in case it's encounter-ending
            offerInvValue = table->getInvValue();

            // Check if nego done, and break if so
            if (encounterIsOver(didWin)) {
                handleEnd(didWin);
                return true;
            }
        }

        ///// NEXT THREE HANDLE PLACING AND TAKING ITEMS FROM THE TABLE
        else if (entry == "cancel") {
            if (table->isEmpty()) {
                echo = "There's nothing on the table for you to take back.\n\n";
            } else echo = "You take your offer back from the table.\n\n";
            player->takeBackOffer(table);
        }
        else if (player->knowsOfItem(entry)) {
            // Finally! Item exists, and player has it
            if (player->hasItem(entry)) {
                echo = "You place your " + entry + " on the table.\n\n";
                player->placeItemOnTable(entry, table);
            // Item exists, but player doesn't currently have it in inventory
            } else echo = "You have no " + entry + " to give.\n\n";
        }
        // Erroneous entry... there is no such thing in the economy
        else echo = "You have no such thing. Try again.\n\n";
    }
}

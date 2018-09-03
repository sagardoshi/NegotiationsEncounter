#ifndef ENCOUNTER_H
#define ENCOUNTER_H


class Encounter {

private:
    bool isPrologue;
    int turns;
    float keyValue;

    PlayerCharacter* player;
    Negotiator* opponent;
    Inventory* table;

    float startInvValue;
    float offerInvValue;
    float endInvValue;


    map<string, int> invMap;     // Used for numbering in encounters

    void mapPlayerInventory();   // Unique order based on player's inv at start

    void centerText(string, string&);
    void lower(string&);
    void removeWS(string&);


public:
    Encounter(PlayerCharacter*, Negotiator*, int, int, float);
    ~Encounter();


    string getCapsName();
    float getFinalInvValue(); // Returns inv value at end of encounter


    void clearScreen();
    void userEntry(string&, string&, string);

    bool isNum(string&);
    void remapKeyword(string&);

    void setInventoryForEncounter();

    bool runEncounter(bool&);

    void printExtraHelp();

    void printTurns();
    void printHelp();
    void printStrategy();
    void printUI();

    void printTitle(string);
    bool encounterIsOver(bool&);
    void printScore();
    void handleEnd(bool);

};

#endif

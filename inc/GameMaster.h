#ifndef GAME_MASTER_H
#define GAME_MASTER_H

#include "Encounter.h"

#include <vector>
#include <fstream>


class GameMaster {

private:
    // A series of "global" constants, quite useful throughout GameMaster
    const int NUM_LEVELS;
    const int WIDTH;

    const string LEVEL_FILEPATH;
    const string INV_FILEPATH;
    const string DIVIDER;

    // Triggers and markers shifted as levels and user inputs change
    int currLevel;
    bool doSkip;
    bool wonLevel;
    bool gameFinished;
    string uInput;

    // The core objects required to build levels
    PlayerCharacter*    player;
    vector<Negotiator*> opponents;
    vector<int>         turns;
    vector<float>       keyPrices;
    vector<Encounter*>  levels;
    vector<string>      titles;
    vector<string>      titleText;


    // Helpful methods for title printing
    void centerText(string, string&);
    void createTitle(char, int, string, string&, bool = true);

    // Managing the class overall, starting and ending the game, etc.
    void setUpGameVars();
    void releaseMemory();

    // All to do with starting the game and saving or loading saves
    void savePlayerLevel();
    void savePlayerInventory();
    int getStartingLevel();
    void loadPlayerSavedInventory();
    void startScreen();

    // The crux of story mode and interpreting user inputs
    void lower(string&);
    void removeWS(string&);
    void fillCleanUInput();
    void setUInput(string = "", string = "", string = "");
    void handleSpecificInput(string, string&);
    bool isAsteriskEntry(string);
    void checkpoint();
    void loadScript(string);

    // Actually running the game, managing level structure
    void runEnding();
    void startCurrentLevel();
    void endCurrentLevel();
    void runNextLevel(bool = false);
    void runIntro();

public:
    GameMaster();
    ~GameMaster();

    // These three methods called by main upon quit
    void printCurrentLevelTitle();
    bool getGameFinished();
    void clearScreen();

    // The primary mother function of the whole game
    void playGame();

};

#endif

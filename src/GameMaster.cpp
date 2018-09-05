#include "../inc/GameMaster.h"


GameMaster::GameMaster() : NUM_LEVELS(5),
                           WIDTH(80),
                           LEVEL_FILEPATH(".save/Level.txt"),
                           INV_FILEPATH(".save/Inventory.txt"),

                           DIVIDER("|"),
                           currLevel(-1),
                           doSkip(false),
                           wonLevel(false),
                           gameFinished(false),
                           uInput("") {
    setUpGameVars();
}

GameMaster::~GameMaster() {
    savePlayerLevel();
    savePlayerInventory();
    releaseMemory();
}


////////////////////////////////////////////////////////////////////////////////
// Managing the class overall, starting and ending the game, etc.
////////////////////////////////////////////////////////////////////////////////

// Creates player and all enemies
void GameMaster::setUpGameVars() {
    const float FRIENDLY = 0.8;
    const float GRUFF    = 0.2;
    const float MODERATE = 0.5;

    player    =  new PlayerCharacter("You", MODERATE);

    // Manual, but most straightforward way of allocating mem for negotiators
    opponents.resize(NUM_LEVELS);
    opponents[0] = new Negotiator("Lepha",            MODERATE);
    opponents[1] = new Negotiator("Mosta and Pepita", FRIENDLY);
    opponents[2] = new Negotiator("Toto",             MODERATE);
    opponents[3] = new Negotiator("Burro",            GRUFF);
    opponents[4] = opponents[0];

    levels       = vector<Encounter*>(NUM_LEVELS, nullptr);
    turns        = vector<int> (NUM_LEVELS, 3);
    keyPrices    = {40.0, 20.0, 30.0, 40.0, 40.0};


    bool doPrint = false;
    const int NUM_TITLES = NUM_LEVELS + 1 + 1; // For game title and "the end"
    titles.resize(NUM_TITLES);
    titleText    = {"Prologue: Caught",
                    "Level 1 / 4: Mosta and Pepita",
                    "Level 2 / 4: Toto",
                    "Level 3 / 4: Burro",
                    "Level 4 / 4: Lepha",
                    "The End",
                    "DEALING WITH THE SPIRITS"};


    char ch;
    for (int i = 0; i < NUM_TITLES; i++) {
        ((i < NUM_LEVELS) ? ch = '=' : ch = '|');
        createTitle(ch, WIDTH, titleText[i], titles[i], doPrint);
    }

}

// Does so for player and opponents
void GameMaster::releaseMemory() {
    // Delete characters
    delete player;
    delete opponents[0]; // No need to delete opponent4, as it's a duplicate
    delete opponents[1];
    delete opponents[2];
    delete opponents[3];
    // Skipping opponents[4], which was already released by opponents[0]
}

// Used for main to check whether player quit prematurely or not
bool GameMaster::getGameFinished() { return gameFinished; }


////////////////////////////////////////////////////////////////////////////////
// Methods for handling screen and title prints
////////////////////////////////////////////////////////////////////////////////

// Typically to ensure that each dialogue nugget has level title at top
void GameMaster::printCurrentLevelTitle() {
    cout << endl << ((currLevel < 0) ? titles.back() : titles[currLevel]);
}

// Pathetic, but functional screen clearer -- big improvement to readability
void GameMaster::clearScreen() {
    const int BIG_NUMBER = 100;
    string clear = "";
    for (int i = 0; i < BIG_NUMBER; i++) {
        clear += "\n";
    }
    cout << clear;

    printCurrentLevelTitle();
}

// Assuming 80 char wide screen, centers a string automatically
void GameMaster::centerText(string title, string &toAdd) {
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

// Packages previous two helper functions assuming 80 char screen width
void GameMaster::createTitle(char ch, int x, string text,
                             string &toAdd, bool print) {

    toAdd += string(WIDTH, ch) + "\n";
    centerText(text, toAdd);
    toAdd += string(WIDTH, ch) + "\n\n";

    if (print) cout << toAdd;
}


////////////////////////////////////////////////////////////////////////////////
// All to do with starting the game and saving or loading saves
////////////////////////////////////////////////////////////////////////////////

// Saves player's current level for later loading
void GameMaster::savePlayerLevel() {
    if (currLevel < 0) return; // Let level stay if quit typed on start screen
    string level = to_string(currLevel);

    ofstream out;
    out.open(LEVEL_FILEPATH);
    out << level << endl; // Overwrite with the single integer-as-string
    out.close();
}

// Saves current state of player's inventory for later loading
void GameMaster::savePlayerInventory() {
    map<string, int>::iterator it;
    string item = "";
    string amountText = "";

    ofstream out;
    out.open(INV_FILEPATH);

    for (it = player->inventory.begin(); it != player->inventory.end(); it++) {
        item = it->first;
        amountText = to_string(it->second);
        out << item << DIVIDER << amountText << endl; // Saves each line
    }

    out.close();
}

// Check level.txt for change
int GameMaster::getStartingLevel() {
    string line = "";

    ifstream in(LEVEL_FILEPATH);               // Open file
    getline(in, line);
    in.close();                                // Close file
    return stoi(line);                         // Convert to int
}

// Only to be called if existing save detected
void GameMaster::loadPlayerSavedInventory() {
    string line = "";
    string invItem = "";
    int invItemAmount = 0;
    int dividerPos = 0;
    int fromDividerToEnd = 0;

    ifstream in(INV_FILEPATH);
    while(getline(in, line)) {
        dividerPos = line.find(DIVIDER);
        fromDividerToEnd = line.length() - dividerPos;

        invItem = line.substr(0, dividerPos);
        invItemAmount = stoi(line.substr(dividerPos + 1, fromDividerToEnd));

        player->inventory[invItem] = invItemAmount;
    }
    in.close();
}

// Announces start of game and asks player to load previous save
void GameMaster::startScreen() {
    string loadSave    = "load";
    string newGame     = "new";
    string savePrompt  = "Previous save detected. Type \"" + loadSave + "\" ";
           savePrompt += "to launch it or \"" + newGame + "\" to start fresh.";

    ///////////// First print game title /////////////
    clearScreen();


    int requestedLevel = getStartingLevel();         // Based on separate file

    ///////////// Next change behaviour if previous save detected /////////////
    if (requestedLevel > 0) {
        setUInput(savePrompt, loadSave, newGame);
        if (uInput == "quit") return; // For every new entry, check for quit

        // Previous save detected and loaded
        if (uInput == loadSave) {
            currLevel = requestedLevel;              // Change level
            loadPlayerSavedInventory();              // Adjusts player inv
        }
        // Previous save detected, but ignored
        else if (uInput == newGame) currLevel = 0;   // Start at beginning
        cout << endl;
    }
    ///////////// Or continue as normal if no save detected /////////////
    else {
        currLevel = 0;
        checkpoint();                                // Opportunity for skip
        if (uInput == "quit") return; // For every new entry, check for quit
    }

}


////////////////////////////////////////////////////////////////////////////////
// The crux of story mode and interpreting user inputs
////////////////////////////////////////////////////////////////////////////////

// Converts a string fully to lowercase (in place)
void GameMaster::lower(string &anyString) {
    int i = 0;
    while (anyString[i]) {
        anyString[i] = tolower(anyString[i]);
        i++;
    }
}

// Removes whitespaces in string (in place)
void GameMaster::removeWS(string &str) {
   str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

// Packages lower() & removeWS() with a getline call, using global uInput
void GameMaster::fillCleanUInput() {
    cout << ">>>> ";
    getline(cin, uInput);

    lower(uInput);
    removeWS(uInput);
}

// Gets simple input to lowercase; callable with specific prompt/target
void GameMaster::setUInput(string prompt, string goal1, string goal2) {

    // Simplify goals
    lower(goal1);
    lower(goal2);
    removeWS(goal1);
    removeWS(goal2);

    string output = "\n";
    createTitle('=', WIDTH, prompt, output);
    fillCleanUInput();
    if (uInput == "quit") return; // For every new entry, check for quit

    // Only worry about invalidity if goals not empty
    if (goal1 != "" || goal2 != "") {
        // Must remove situation of XOR, or else empty inputs will be accepted
        if (goal1 == "" && goal2 != "") goal1 = goal2;
        if (goal1 != "" && goal2 == "") goal2 = goal1;

        // Then if wrong input, note invalidity, and re-print prompt
        while (uInput != goal1 && uInput != goal2) {
            clearScreen();
            cout << "\nInvalid input. Try again.\n\n\n";
            output = ""; // Reset output
            createTitle('=', WIDTH, prompt, output);
            fillCleanUInput();
            if (uInput == "quit") return; // For every new entry, check for quit
        }
    }
    clearScreen();
}

// Require a specific input from user, as coded by text file
void GameMaster::handleSpecificInput(string line, string &output) {
    cout << output << endl << endl; // Flush output thus far

    const int ASTERISKS_NEEDED = 3;
    int newLength = line.length() - ASTERISKS_NEEDED - ASTERISKS_NEEDED;

    string targetInput = line.substr(ASTERISKS_NEEDED, newLength);
    string prompt = "Type \"" + targetInput + "\"";

    setUInput(prompt, targetInput);
    if (uInput == "quit") return; // For every new entry, check for quit

    // Once inventory typed, display it
    if (targetInput == "inventory") player->printInv();

    output = ""; // Reset output
}

// Ensures that line is of format ***SOME_KEYWORD***
bool GameMaster::isAsteriskEntry(string line) {
    // Must have three asterisks at beginning AND end of prompt

    const int ASTERISKS_NEEDED = 3;
    if (line.length() <= (ASTERISKS_NEEDED + ASTERISKS_NEEDED)) return false;

    for (int i = 0; i < ASTERISKS_NEEDED; i++) {
        if (line[i] != '*') return false; // Check for asterisks at beginning
        if (line[line.length() - i - 1] != '*') return false; // and at end
    }

    return true;
}

// Pauses text for user to read and get a breather
void GameMaster::checkpoint() {
    string toAdd = "\n\n";
    string anyInputText  = "<RETURN>: next line | \"skip\": next negotiation";

    createTitle('=', WIDTH, anyInputText, toAdd);
    fillCleanUInput();

    if (uInput == "quit") return; // For every new entry, check for quit
    if (uInput == "skip" ) doSkip = true; // Set global flag for future

    clearScreen();
}

// Prints txt script and replaces key symbols with frequent functions
void GameMaster::loadScript(string filename) {
    if (doSkip) return; // Skip all text until encounter

    string filepath = "txt/" + filename + ".txt";
    string line = "", output = "";

	ifstream in(filepath);

	while (getline(in, line)) {
        if (line == "***") {
            cout << output << endl;
            checkpoint();
            if (uInput == "quit") return; // For every new entry, check for quit
            output = "";

            // Handle "skip" typed within a script file
            if (doSkip) {
                in.close();
                return;
            }
        } else if (isAsteriskEntry(line)) {
            handleSpecificInput(line, output);
            if (uInput == "quit") return; // For every new entry, check for quit
        } else output += "\n" + line;
    }

    in.close();

    cout << output << endl << endl;
}


////////////////////////////////////////////////////////////////////////////////
// Actually running the game, managing level structure
////////////////////////////////////////////////////////////////////////////////

// Intro and prologue are unique (re:inventory), so handled separately
void GameMaster::runIntro() {

    player->fillInventory(); // Give player an inventory before the intro
    loadScript("Gen/Intro"); // Early exposition until discovery by Lepha
    if (uInput == "quit") return; // For every new entry, check for quit

    player->initInventory(); // Clear inventory before tutorial level

    bool isPrologue = true;
    runNextLevel(isPrologue);
    if (uInput == "quit") return; // For every new entry, check for quit

    player->fillInventory(); // Refill inventory before Level 1
}

// Loads final ending score + script, equivalent for loss or victory
void GameMaster::runEnding() {
    // Should give score at end of most recent level
    float finalScoreFloat = player->getInvValue();

    // Convert score to string for later use
    string finalScore = player->toPreciseString(finalScoreFloat);

    // Modify to output sentence
    string scoreText = "\n\nYou started with £100 in loot. You finished with £";
           scoreText += finalScore + ".\n";

    doSkip = false; // In order to print the final goodbye

    clearScreen(); // To switch to the THE END screen

    // Printouts
    cout << scoreText;
    loadScript("Gen/Ending"); // Generic text for victory or defeat
    cout << endl << endl;

}

// Runs text before level, runs encounter, and runs text after level
void GameMaster::runNextLevel(bool isPrologue) {
    startCurrentLevel(); // Intro text for level

    // Only create level at last possible moment
    levels[currLevel] = new Encounter(player,
                                      opponents[currLevel],
                                      turns[currLevel],
                                      keyPrices[currLevel],
                                      isPrologue);


    bool didNotQuit = levels[currLevel]->runEncounter(wonLevel);
    if (!didNotQuit) { // If quit came from within encounter
        uInput = "quit";
        return; // For every new entry, check for quit
    }

    endCurrentLevel(); // Ending text for level (+ game over ending)

    wonLevel = false; // Reset, even though it will be filled later
    currLevel++; // Advance level after complete

    // Doing this AFTER advancing currLevel
    if (uInput == "quit") return;
}

// After intros, these kick off each individual level
void GameMaster::startCurrentLevel() {
    string outText  = to_string(currLevel) + "/Level";
           outText += to_string(currLevel) + "Start";

    loadScript(outText);
}

// Called at the end of EACH level, with title change and skips in place
void GameMaster::endCurrentLevel() {
    string outText  = to_string(currLevel) + "/Level" + to_string(currLevel);
    // Load the correct script
    (wonLevel ? (outText += "Win") : (outText += "Lose"));

    doSkip = false; // Allow scripts after encounter
    loadScript(outText); // Text for victory or loss to be run first

    // If lost encounter and not in tutorial, end game
    if (!wonLevel && (currLevel > 0)) runEnding();

    return;
}


////////////////////////////////////////////////////////////////////////////////
// The primary mother function of the whole game
////////////////////////////////////////////////////////////////////////////////

// Core game function
void GameMaster::playGame() {
    // Ugly, but the cost of the GameMaster class are these frequent quit checks

    startScreen(); // Ask to skip intro
    if (uInput == "quit") return; // For every new entry, check for quit

    if (currLevel == 0) runIntro(); // Runs intro
    if (uInput == "quit") return; // For every new entry, check for quit

    while (currLevel < NUM_LEVELS) {
        runNextLevel();
        if (uInput == "quit") return; // For every new entry, check for quit
    }

    if (currLevel > 4) runEnding();
    if (uInput == "quit") return; // For every new entry, check for quit

    gameFinished = true;


}

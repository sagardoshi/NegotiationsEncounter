/*
METADATA
Name: Sagar Doshi
Date: 4 September 2018
Title: Negotiations Video Game
Advisors: Fariba Sadri & Anandha Gopalan
Course: MSc Computing Science
University: Imperial College London
*/

#include "../inc/GameMaster.h"

#include <csignal>


////////////////////////////////////////////////////////////////////////////////
// Disables/Re-enables signals and special characters
////////////////////////////////////////////////////////////////////////////////

// Re-set CTRL+D at end of game
void reEnableSpecialCharacters() {
    system("stty eof '^d'"  ); // re-enable CTRL+D
    system("stty intr '^c'" ); // re-enable CTRL+C
    system("stty quit '^\\'" ); // re-enable CTRL+backslash
    system("stty susp '^z'" ); // re-enable CTRL+Z
    system("stty dsusp '^y'"); // re-enable CTRL+Y
}

// Ignores signal if not already disabled by earlier function
void signalHandler(int signum) {}

// Register signals and signal handler
void registerSignals() {
    struct sigaction act;

    act.sa_handler = signalHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT,  &act, 0); // Ctrl-C
    sigaction(SIGQUIT, &act, 0); // Ctrl-backslash
    sigaction(SIGTSTP, &act, 0); // Ctrl-Z
    sigaction(SIGTERM, &act, 0); // Like SIGINT

}

// Re-direct CTRL+D during game
void disableSpecialCharacters() {
    system("stty eof undef"  ); // disable CTRL+D
    system("stty intr undef" ); // disable CTRL+C
    system("stty quit undef" ); // disable CTRL+backslash
    system("stty susp undef" ); // disable CTRL+Z
    system("stty dsusp undef"); // disable CTRL+Y
}


////////////////////////////////////////////////////////////////////////////////
// Setting up, quitting, and playing the game itself
////////////////////////////////////////////////////////////////////////////////

// Abstracts above three functions
void setUpGame() {
    disableSpecialCharacters();
    registerSignals();
}

// Helper to immediately end game if quit typed
void quitGame(GameMaster* gm) {
    reEnableSpecialCharacters();

    if (!gm->getGameFinished()) {
        gm->clearScreen();
        string gaveUp  = "You can't handle this anymore. This house, these ";
               gaveUp += "spirits, they are driving you\n";
               gaveUp += "mad! You've gotten this far, and that feels like an ";
               gaveUp += "achievement in itself.\n\n";
               gaveUp += "Perhaps it's the only achievement left to you. Yes, ";
               gaveUp += "perhaps it is.\n\n";
               gaveUp += "You lay back, allow the failure to fill you up, and ";
               gaveUp += "resign yourself to the\ncoming punishment.\n\n";
               gaveUp += "You have quit the game. Please come back and visit ";
               gaveUp += "the spirits again whenever\n";
               gaveUp += "you can! They'll be waiting...\n\n";

        cout << gaveUp;
    }
}

// Main function handles signals, kicks off playGame(), and quits
int main() {
    GameMaster* gm = new GameMaster();

    setUpGame();
    gm->playGame();
    quitGame(gm);

    delete gm;
    return 0;
}

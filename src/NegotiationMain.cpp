// Ref: http://patorjk.com/software/taag/#p=testall&f=Fun%20Face&t=O

/*
METADATA
Name: Sagar Doshi
Date: 24 July 2018
Title: Negotiations Video Game
Course: MSc Computing Science
University: Imperial College London
*/

#include "../inc/Negotiator.h"
#include "../inc/Encounter.h"
#include "../inc/PlayerCharacter.h"
#include "../inc/Globals.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

void createActions() {
    // Acceptable actions more generally
    genActions.push_back("help");
    genActions.push_back("quit");
    genActions.push_back("inventory");

    // Negotiation actions
    negoActions.push_back("turns");
    negoActions.push_back("issues");
    negoActions.push_back("propose offer");
}

void createEconomy() {
    // name: [default price, how many owned]
    economy["pomegranate"] = 10;
    economy["knuckle pads"] = 15;
    economy["silverback perfume"] = 10;
    economy["ginger cookie"] = 5;
    economy["coins"] = 1;
    economy["basket"] = 2;

    economy["porridge key"] = 30;


}


// Prints ASCII art
void printGorilla() {
    cout << "     .-.      " << endl;
    cout << "   c(O_O)c    " << endl;
    cout << "  ,'.---.`,   " << endl;
    cout << " / /|_|_|\\ \\ " << endl;
    cout << " | \\_____/ |  " << endl;
    cout << " '. `---' .`  " << endl;
    cout << "   `-...-'    " << endl << endl;
}

void printHelp() {
    cout << "******************* GENERAL HELP *******************\n";
    cout << "[help: see this menu]\n";
    cout << "[quit: exit the game]\n";
    cout << "[inventory: see what you currently hold]\n";
    cout << "****************************************************\n\n";


    if (inNego) {
        cout << "**************** DURING NEGOTIATION ****************\n";
        cout << "[turns: see how many turns you have left]\n";
        cout << "[issues: see what you're negotiating over]\n";
        cout << "[propose offer: use a turn to build your own offer]\n";
        cout << "****************************************************\n\n";
    }
}

void quitGame() {
    cout << "******************* QUIT *******************\n";
    cout << "As you whisper the word \"quit\", suddenly a powerful wind ";
    cout << "takes you up, up, and away, out of the House of Negotiations ";
    cout << "and back to your normal life. You wake up as if from a ";
    cout << "dream.\n\n";
    cout << "And yet... You shake your hair and notice as a single ";
    cout << "pomegranate seed falls to the ground.\n\n";
    cout << "Thank you for playing! Until next time.\n";
    cout << "********************************************\n\n\n\n";
    exit(0);
}

void printHowToNegotiate() {
    cout << "********************* HOW TO NEGOTIATE *********************\n";
    cout << "[For your first negotiation, your goal is to get a key from ";
    cout << "Porridge, the boy gorilla. But he probably won't just ";
    cout << "give you what you want for free... You have to find a trade.]\n\n";

    cout << "[Each opponent has a distinct personality. Porridge, for example, ";
    cout << "is a slightly impulsive, but friendly kid. He responds well if ";
    cout << "you offer him something he likes. Others, however, may not be ";
    cout << "quite as straightforward.]\n\n";

    cout << "[You have a limited number of rounds before your opponent gets ";
    cout << "impatient and gives up on you. At any time, you can type ";
    cout << "\"turns\" to check your deadline.]\n\n";

    cout << "[In every negotiation, there are one or more issues on the ";
    cout << "table. Each issue can take certain values, between a minimum and ";
    cout << "a maximum. During a negotiation, in order to see the issues on ";
    cout << "the table and their values, you need only type \"issues\".]\n\n";

    cout << "[The core of the negotiation is to propose tantalising offers ";
    cout << "to your opponent. ";
    cout << "In order to do that here, you can type \"propose offer\" to ";
    cout << "craft one by selecting items from your inventory to package ";
    cout << "together into an offer and send it.]\n\n";

    cout << "[The help menu now offers reminders of what commands to select ";
    cout << "during a negotiation.]\n";
    cout << "***********************************************************\n\n\n\n";
}


void checkpoint(bool allKeys = true) {

    uInput = "";
    cout << "\n\n----------------------------\n";
    if (allKeys) cout << "[Press any key to continue.]";
    else cout << ">>>> ";
    getline(cin, uInput);
    cout << "----------------------------\n\n\n\n";

}

// Rejects input not in list of acceptable verbs
bool isValidInput() {
    if (find(genActions.begin(), genActions.end(), uInput)
                                      != genActions.end()) {
        return true;
    }

    if (inNego) {
        if (find(negoActions.begin(), negoActions.end(), uInput)
                                            != genActions.end()) {
            return true;
        }
    }
    return false;
}

// Gets simple, unverified user input and converts to lowercase
string getStandardisedInput() {
    // Get standard input (including whitespaces)
    checkpoint(!anyKeyOK);

    // Convert any capitals to lowercase in place
    int i = 0;
    while (uInput[i]) {
        uInput[i] = tolower(uInput[i]);
        i++;
    }

    return uInput;
}


// Fully abstracted function to call to get user input
void getUserInput() {
    string uInput = getStandardisedInput();

    while(true) {
        if (inNego) {
            if (uInput == "quit") quitGame();
            else if (uInput == "help") printHelp();
            else if (uInput == "inventory") player->printInventory();
            else if (uInput == "turns" && inNego) currNego->printTurns();
            else if (uInput == "issues" && inNego) currNego->printIssues();
            else if (uInput == "propose offer" && inNego) {
                currNego->buildValidOffer(economy, bidAccepted);
                if (bidAccepted) return;
                currNego->useOneTurn();
            }
        } else {
            if (uInput == "quit") quitGame();
            else if (uInput == "help") printHelp();
            else if (uInput == "inventory") player->printInventory();
        }
        uInput = getStandardisedInput();
    }
}


void tutorialChoices(string whichChoice) {

    if (whichChoice == "firstTutorialQuestion") {
        cout << "[1: \"Uh, I'm not really sure...\"]\n";
        cout << "[2: \"To meet you!\"]\n";
        cout << "[3: \"It's a long story involving hot air balloons and lots of padding...\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2" && uInput != "3") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[1: \"Uh, I'm not really sure...\"]\n";
            cout << "[2: \"To meet you!\"]\n";
            cout << "[3: \"It's a long story involving hot air balloons and lots of padding...\"]\n\n";

            getStandardisedInput();
        }
    }

    if (whichChoice == "secondTutorialQuestion") {
        cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
        cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[1: \"Yeah, sure, sounds fun!\"]\n";
            cout << "[2: \"You're a bunch of weirdos! No way!\"]\n\n";

            getStandardisedInput();
        }
    }

}

void introStart() {
    cout << "\n\n\nCRASH!! You fall through a roof and land in a soft pile of ";
    cout << "of blankets. Ow. You shake your head as the dust settles and ";
    cout << "look up to see a large gorilla standing before you.\n\n";

    printGorilla();

    cout << "She has a long, rounded face, has a pair of thin glasses ";
    cout << "perched on her short nose, and is wearing a plain t-shirt and ";
    cout << "trousers. Her fur seems to be graying and fading a little around ";
    cout << "the edges. As the dust clears, it seems she was reading a ";
    cout << "magazine whilst sitting on her living room armchair.\n\n";

    checkpoint(anyKeyOK);

    cout << "She looks you over silently and purses her lips in confusion. ";
    cout << "You both freeze there for a few moments, considering the ";
    cout << "the situation. Suddenly, her lips widen into ";
    cout << "a broad, free smile.\n\n";

    cout << "\"Hello there, young human! How did you get in our house?\"\n\n";


    tutorialChoices("firstTutorialQuestion");
    uInput = ""; // Reset uInput

    cout << "\n\"Huh! Weird! Well, no matter. Welcome to the House of ";
    cout << "Negotiations! My name is Chamoy. Ook ook.\"\n\n";

    cout << "She stands up, holding her bulk back on two powerful legs. She ";
    cout << "extends a muscular arm to you for a handshake and daintily holds ";
    cout << " your ";
    cout << "hand as if it were a delicate teacup. She steps back again.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"Well, friend, the rules of the House are simple. Given the damage ";
    cout << "you have caused, you need to pay it off for us. Earn your way ";
    cout << "back to your normal life by winning keys from at least three members ";
    cout << "of my family.\"\n\n";

    cout << "\"How to gain the keys, you ask? Oooooook.\"\n\n";

    checkpoint(anyKeyOK);

    cout << "\"Simple! Follow our favourite pastime! Haggle with us! ";
    cout << "If you can convince three of us to give you a key, you can head ";
    cout << "back to your normal life, no harm, no foul. Ooook.\"\n\n";

    cout << "\"What do you say? Sounds like fun, right?\"\n\n";


    tutorialChoices("secondTutorialQuestion");

    if (uInput == "1") {
        cout << "Chamoy rubs her palms together in anticipation.\n\n";
        cout << "\"Excellent!! This will be great fun. Ook ook!\"\n\n";
    }
    if (uInput == "2") {
        cout << "Chamoy rubs her chin thoughtfully.\n\n";

        cout << "\"Okay, if you say so!\" she says, shrugging her shoulders. ";
        cout << "\"We have another way of making sure you learn your lesson.\"\n\n";

        checkpoint(anyKeyOK);

        cout << "She reaches one powerful arm back, and swings it rapidly ";
        cout << "towards your face. The last thing you see is her sorrowful ";
        cout << "expression. You feel a sharp pain, and then nothing. ";
        cout << "The blackness gulps you down.\n\n";

        cout << "You have been killed by Chamoy the Gorilla.\n\n";

        cout << "Perhaps you might do better in another life if you don't ";
        cout << "antagonise the very strong matriarch whose home you ";
        cout << "invaded.\n\n";

        cout << "Goodbye, and happy haggling!\n\n";
        exit(0);
    }

    uInput = ""; // Reset uInput


    cout << "She throws up her hands and starts rummaging on the far side of ";
    cout << "her armchair. She emerges shortly thereafter with a basket.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"This is all I can give you. You may use these as you wish ";
    cout << "for your negotiations. Ook. However, this is all you have. It ";
    cout << "must last you all three negotiations. Understand? Good.\"\n\n";

    cout << "She hands over the basket. You take it.\n\n";

    player->fillInventory();

    cout << "[You have an inventory! When negotiating, type \"inventory\" to see what you have.]\n\n";

    checkpoint(anyKeyOK);

    cout << "\"You look worried!\" She laughs. \"Don't look worried! What's ";
    cout << "the worst that can happen?\" Suddenly, Chamoy takes on a wild, ";
    cout << "terrifying aspect. \"After all, it's not like ";
    cout << "you're negotiating for your life...\" ";
    cout << "Without warning, a smile spreads over her face once more.\n\n";

    cout << "\"And you'll start easy, by negotiating with my young ";
    cout << "grandson, Porridge. He's still learning the family game, ";
    cout << "so it will be good practice for him too. Alright then, ";
    cout << "good luck! Ook ook!\"\n\n";

    cout << "Chamoy settles back onto her chair, opens her magazine, and pays ";
    cout << "you no heed.\n\n";

    checkpoint(anyKeyOK);

}

void l1Start() {
    cout << "You turn left and go down a hallway for a while. It's ";
    cout << "brightly lit, with colourful flowers everywhere. ";
    cout << "You see framed photos of the large Gorilla family. ";
    cout << "You keep moving down the hallway until you get to a door.\n\n";

    cout << "The door has a chalkboard taped to it with \"Porridge\" ";
    cout << "scribbled in childish, messy handwriting. You open the door.\n\n";

    checkpoint(anyKeyOK);

    cout << "Within, you see a tiny gorilla, wearing a baseball cap and ";
    cout << "tossing a ball against the wall. When you walk in, he pauses. ";
    cout << "\"Hi! You must be here for a negotiation! Grandma said someone ";
    cout << "would come knocking this morning. If you want my ";
    cout << "key, though, I'll warn you that I drive a hard bargain, and I'm ";
    cout << "the toughest negotiator in the family! You stand no chance!\"\n\n";

    checkpoint(anyKeyOK);

    cout << "He strikes a fierce pose, pouting his lips, but the effect is ";
    cout << "just too cute. You stifle a giggle. ";
    cout << "He grins too and points to a chair, gesturing you to sit down.\n\n";

    cout << "You take your seat. The negotiation is about to begin!\n\n\n\n";

    checkpoint(anyKeyOK);

    printHowToNegotiate();

    currNego = level1;

    checkpoint(anyKeyOK);
}

void l2Start() {
    cout << "Just as you step into the ";
    cout << "family room again, a rolling rumble fills the air. You ";
    cout << "start, but it quiets down and before emerging again. And ";
    cout << "again. And again. You quickly pinpoint the source of the ";
    cout << "noise.\n\n";

    checkpoint(anyKeyOK);

    cout << "Chamoy is ";
    cout << "still in her armchair, but she has fallen asleep, her ";
    cout << "magazine slipping off her belly. She is snoring.\n\n";

    cout << "You tiptoe through the room and on to the hallway on ";
    cout << "the other side. Again, you see more framed photos ";
    cout << "of the family, and a clutch of tulips in a vase. Toward ";
    cout << "the end of the hallway is another room, parallel to ";
    cout << "Porridge's.\n\n";

    checkpoint(anyKeyOK);

    cout << "";

    currNego = level2;
}


void l3Start() { currNego = level3; }
void endStart() { currNego = nullptr; }

void interact() {

    // Run introduction
    stage currentStage = intro;
    introStart();

    // Progress to Level 1
    currentStage = l1;
    l1Start();
    inNego = true;
    bidAccepted = false;
    level1->beginEncounter();
    getUserInput();
    inNego = false;


    // Progress to Level 2
    currentStage = l2;
    l2Start();
    inNego = true;
    bidAccepted = false;
    level2->beginEncounter();
    getUserInput();
    inNego = false;


    // Progress to Level 3
    currentStage = l3;
    // uInput = getUserInput();
    // l3Start();

    // Run ending
    currentStage = ending;
    endStart();


}

int main() {

    player = new PlayerCharacter("You", MODERATE);

    createActions();
    createEconomy();

    Negotiator* porridge = new Negotiator("Porridge", FRIENDLY);
    Negotiator* rhubarb = new Negotiator("Rhubarb", GRUFF);
    Negotiator* chamoy = new Negotiator("Chamoy", GRUFF);

    ////////// Level 1 //////////
    level1 = new Encounter(player, porridge, NEGO1, L1_TURNS);
    Issue<float> key1("porridge's key", 30.0, 15.0, 45.0);
    level1->loadFloatIssue(key1);

    ////////// Level 2 //////////
    level2 = new Encounter(player, rhubarb, NEGO2, L2_TURNS);
    Issue<float> key2("rhubarb's key", 40.0, 30.0, 50.0);
    level2->loadFloatIssue(key2);

    ////////// Level 3 //////////
    level3 = new Encounter(player, chamoy, NEGO3, L3_TURNS);
    Issue<float> key3("chamoy's key", 50.0, 40.0, 60.0);
    level3->loadFloatIssue(key3);

    interact();

    delete level1;
    delete level2;
    delete level3;
    delete currNego;

    delete porridge;
    delete rhubarb;
    delete chamoy;
    delete player;

    return 0;

}

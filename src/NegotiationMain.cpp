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
#include "../inc/Inventory.h"
#include "../inc/Globals.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include <random>

using namespace std;

void createActions() {
    // Acceptable actions more generally
    genActions.push_back("help");
    genActions.push_back("inventory");
    genActions.push_back("quit");
    genActions.push_back("negotiate");

    // Negotiation actions
    negoActions.push_back("see turns left");
    negoActions.push_back("see issues");
    negoActions.push_back("see current offer");
    negoActions.push_back("propose offer");
    negoActions.push_back("accept terms");
    negoActions.push_back("walk away");
}

void createEconomy() {
    // name: [default price, how many owned]



    economy["Pomegranate"] = 10;
    economy["Knuckle pads"] = 15;
    economy["Silverback perfume"] = 10;
    economy["Ginger cookie"] = 5;

    economy["Basket"] = 2;

    economy["Porridge's Key"] = 12;


}


// A trivial 0-1 random generator engine from time-based seed
float getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator)/100.0;
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
    cout << "[Type \"help\" to see this menu.]\n";
    cout << "[Type \"quit\" to quit the game.]\n";
    cout << "[Type \"inventory\" to see what you currently hold.]\n";
    cout << "[Type \"negotiate\" when prompted to start a level.]\n";
    cout << "****************************************************\n\n";


    if (inNegotiation) {
        cout << "***************** NEGOTIATION HELP *****************\n";
        cout << "[DURING NEGOTIATION: type \"see turns left\" to see how many ";
        cout << "rounds remain to conclude the negotiation.]\n";

        cout << "[DURING NEGOTIATION: type \"see issues\" to see all ";
        cout << "issues on the table, along with their min and max possible ";
        cout << "values.]\n";

        cout << "[DURING NEGOTIATION: type \"see current offer\" to see the ";
        cout << "current offer on the table.]\n";

        cout << "[DURING NEGOTIATION: type \"propose offer\" to make your own ";
        cout << "offer. You will be prompted for a value for each issue.]\n";

        cout << "[DURING NEGOTIATION: type \"accept terms\" to confirm that you ";
        cout << "are willing to take the offer currently on the table.]\n";

        cout << "[DURING NEGOTIATION: type \"walk away\" if you see no possible ";
        cout << "solution to this negotiation.]\n";
        cout << "****************************************************\n\n";
    }
}

void printQuit() {
    cout << "******************* QUIT *******************\n";
    cout << "As you whisper the word \"quit\", suddenly a powerful wind ";
    cout << "takes you up, up, and away, out of the House of Negotiations ";
    cout << "and back to your normal life. You wake up as if from a ";
    cout << "dream.\n\n";
    cout << "And yet... You shake your hair and notice as a single ";
    cout << "pomegranate seed falls to the ground.\n\n";
    cout << "Thank you for playing! Until next time.\n";
    cout << "********************************************\n\n\n\n";
}

void printHowToNegotiate() {
    cout << "********************* HOW TO NEGOTIATE *********************\n";
    cout << "[For your first negotiation, your goal is to get a key from ";
    cout << "Porridge, the boy gorilla. But he probably won't just ";
    cout << "give you what you want for free... You have to make a compromise.]\n\n";

    cout << "[Each opponent has a distinct personality. Porridge, for example, ";
    cout << "is a slightly impulsive, but friendly kid. He responds well if ";
    cout << "you offer him something he likes. Others, however, may not be ";
    cout << "quite as straightforward.]\n\n";

    cout << "[You have a limited number of rounds before your opponent gets ";
    cout << "impatient and gives up on you. At any time, you can type ";
    cout << "\"see turns left\" to check your deadline.]\n\n";

    cout << "[In every negotiation, there are one or more issues on the ";
    cout << "table. Each issue can take certain values, between a minimum and ";
    cout << "a maximum. During a negotiation, in order to see the issues on ";
    cout << "the table and their values, you need only type \"see issues\".]\n\n";

    cout << "[If there is an offer currently on the table, type \"see offer\" ";
    cout << "to see it.]\n\n";

    cout << "[The core of the negotiation is to propose and respond to offers. ";
    cout << "In order to do that here, you can type \"propose offer\" to craft ";
    cout << "and send one of your own. You can also type \"accept terms\" to ";
    cout << "take the current offer on the table or \"walk away\" if you ";
    cout << "do not think an agreement is possible.]\n\n";

    cout << "[The help menu now offers reminders of what commands to select ";
    cout << "during a negotiation.]\n";
    cout << "***********************************************************\n\n\n\n";
}


// Rejects input not in list of acceptable verbs
bool isValidInput() {
    if (inNegotiation) {
        if (find(negoActions.begin(), negoActions.end(), uInput)
                                            != genActions.end()) {
            return true;
        }
    }

    if (find(genActions.begin(), genActions.end(), uInput)
                                      != genActions.end()) {
        return true;
    }
    return false;
}

// Gets simple, unverified user input and converts to lowercase
string getStandardisedInput() {
    // Get standard input (including whitespaces)
    uInput = "";
    cout << "\n\n--------------------------\n";
    cout << ">>>> ";
    getline(cin, uInput);
    cout << "--------------------------\n\n\n\n";

    // Convert any capitals to lowercase in place
    int i = 0;
    while (uInput[i]) {
        uInput[i] = tolower(uInput[i]);
        i++;
    }

    return uInput;
}


// Fully abstracted function to call to get user input
string getUserInput() {
    string uInput = getStandardisedInput();

    // First check for interaction-ending keywords
    if (negotiationPossible && uInput == "negotiate") return uInput;
    if (uInput == "quit") {
        printQuit();
        exit(0);
    }

    // Keywords if in a negotiation
    if (inNegotiation) {
        if (uInput == "see turns left") {
            currentEncounter->printTurnsLeft();

        } else if (uInput == "see issues") {
            currentEncounter->printEncounterIssues();

        } else if (uInput == "see current offer") {
            currentEncounter->printOfferOnTable();

        } else if (uInput == "propose offer") {
            currentEncounter->buildValidOffer(economy);
            currentEncounter->useOneTurn();

        } else if (uInput == "accept terms") {
            // TODO: run player->acceptTerms()
            cout << "You have accepted the terms." << endl;
            currentEncounter->useOneTurn();
            return uInput;

        } else if (uInput == "walk away") {
            // TODO: run player->walkAway()
            cout << "You have walked away from the table." << endl;
            currentEncounter->useOneTurn();
            return uInput;
        }
    }

    // Remaining general keywords (except "negotiate" and "quit")
    if (uInput == "help") {
        printHelp();

    } else if (uInput == "inventory") {
        player->printInventory();

    } else if (!isValidInput()) {
        cout << "\n[Invalid input. Try again. Type \"help\" for ";
        cout << "instructions.]\n\n";
    }

    uInput = getUserInput(); // Recurse if not
    return uInput;
}


void tutorialChoices(string whichChoice) {

    if (whichChoice == "firstTutorialQuestion") {
        cout << "[Type \"1\" for \"Uh, I'm not really sure...\"]\n";
        cout << "[Type \"2\" for \"To meet you!\"]\n";
        cout << "[Type \"3\" for \"It's a long story involving hot air balloons and lots of padding...\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2" && uInput != "3") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[Type \"1\" for \"Uh, I'm not really sure...\"]\n";
            cout << "[Type \"2\" for \"To meet you!\"]\n";
            cout << "[Type \"3\" for \"It's a long story involving hot air balloons and lots of padding...\"]\n\n";

            getStandardisedInput();
        }
    }

    if (whichChoice == "secondTutorialQuestion") {
        cout << "[Type \"1\" for \"Yeah, sure, sounds fun!\"]\n";
        cout << "[Type \"2\" for \"You're a bunch of weirdos! No way!\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[Type \"1\" for \"Yeah, sure, sounds fun!\"]\n";
            cout << "[Type \"2\" for \"You're a bunch of weirdos! No way!\"]\n\n";

            getStandardisedInput();
        }
    }

}

void introStart() {
    cout << "CRASH!! You fall through a roof and land in a soft pile of ";
    cout << "of blankets. Ow. You shake your head as the dust settles and ";
    cout << "look up to see a large gorilla standing before you.\n\n";

    printGorilla();

    cout << "She has a long, rounded face, has a pair of thin glasses ";
    cout << "perched on her short nose, and is wearing a plain t-shirt and ";
    cout << "trousers. Her fur seems to be graying and fading a little around ";
    cout << "the edges. As the dust clears, it seems she was reading a ";
    cout << "magazine whilst sitting on her living room armchair.\n\n";

    cout << "She looks you over silently and purses her lips in confusion. ";
    cout << "You both freeze there for a few moments, considering the ";
    cout << "the situation. Suddenly, her lips widen into ";
    cout << "a broad, free smile.\n\n";

    cout << "--Hello there, young human! How did you get in our house?––\n\n";


    tutorialChoices("firstTutorialQuestion");
    uInput = ""; // Reset uInput

    cout << "\n--Huh! Weird! Well, no matter. Welcome to the House of ";
    cout << "Negotiations! My name is Chamoy. Ook ook.--\n\n";

    cout << "She stands up, holding her bulk back on two powerful legs. She ";
    cout << "extends a muscular arm to you for a handshake and daintily holds ";
    cout << " your ";
    cout << "hand as if it were a delicate teacup. She steps back again.\n\n";

    cout << "--Well, friend, the rules of the House are simple. Given the damage ";
    cout << "you have caused, you need to pay it off for us. Earn your way ";
    cout << "back to your normal life by winning keys from at least three members ";
    cout << "of my family.--\n\n";

    cout << "--How to gain the keys, you ask? Oooooook.--\n\n";

    cout << "--Simple! Follow our favourite pastime! Haggle with us! ";
    cout << "If you can convince three of us to give you a key, you can head ";
    cout << "back to your normal life, no harm, no foul. Ooook.--\n\n";

    cout << "--What do you say? Sounds like fun, right?--\n\n";


    tutorialChoices("secondTutorialQuestion");

    if (uInput == "1") {
        cout << "Chamoy rubs her palms together in anticipation.\n\n";
        cout << "--Excellent!! This will be great fun. Ook ook!--\n\n";
    }
    if (uInput == "2") {
        cout << "Chamoy rubs her chin thoughtfully.\n\n";

        cout << "--Okay, if you say so!-- she says, shrugging her shoulders. ";
        cout << "--We have another way of making sure you learn your lesson.--\n\n";

        cout << "She reaches one powerful arm back, and swings it rapidly ";
        cout << "towards your face. The last thing you see is her sorrowful ";
        cout << "expression.\n\n";

        cout << "You have been killed by Chamoy the Gorilla.\n\n";

        cout << "Perhaps you might do better in another life if you don't ";
        cout << "antagonise the very strong matriarch whose home you ";
        cout << "invaded.\n\n";

        cout << "Goodbye, and happy bananas!\n\n";
        exit(0);
    }

    uInput = ""; // Reset uInput

    cout << "She throws up her hands and starts rummaging on the far side of ";
    cout << "her armchair. She emerges shortly thereafter with a basket.\n\n";

    cout << "--This is all I can give you. You may use these as you wish ";
    cout << "for your negotiations. Ook. However, this is all you have. It ";
    cout << "must last you all three negotiations. Understand? Good.--\n\n";

    cout << "She hands over the basket. You take it.\n\n";

    player->fillInventory();

    cout << "[You have an inventory! At any time, type \"inventory\" in a ";
    cout << "command prompt to see what you have.]\n\n";

    cout << "--You look worried!-- She laughs. --Don't look worried! What's ";
    cout << "the worst that can happen?-- Suddenly, Chamoy takes on a wild, ";
    cout << "terrifying aspect. --After all, it's not like ";
    cout << "you're negotiating for your life...-- ";
    cout << "Without warning, a smile spreads over her face once more.\n\n";

    cout << "--And you'll start easy, by negotiating with my young ";
    cout << "grandson, Porridge. He's still learning the family game, ";
    cout << "so it will be good practice for him too. Alright then, ";
    cout << "good luck! Ook ook!--\n\n";

    cout << "Chamoy settles back onto her chair, opens her magazine, and pays ";
    cout << "you no heed.\n\n";

    cout << "[It's time to negotiate! Type \"negotiate\" to start. If you're ";
    cout << "stuck, you need only type \"help\".]" << endl << endl;

}

void l1Start() {
    cout << "You turn left and go down a hallway for a while. It's ";
    cout << "brightly lit, with colourful flowers everywhere. ";
    cout << "You see framed photos of the large Gorilla family. ";
    cout << "You keep moving down the hallway until you get to a door. ";

    cout << "The door has a chalkboard taped to it with \"Porridge\" ";
    cout << "scribbled in childish, messy handwriting. You open the door.\n\n";

    cout << "Within, you see a tiny gorilla, wearing a baseball cap and ";
    cout << "tossing a ball against the wall. When you walk in, he pauses. ";

    cout << "--Hi! You must be here for a negotiation! Grandma said someone ";
    cout << "would come knocking this morning. If you want my ";
    cout << "key, though, I'll warn you that I drive a hard bargain, and I'm ";
    cout << "the toughest negotiator in the family! You stand no chance!--\n\n";

    cout << "He strikes a fierce pose, pouting his lips, but the effect is ";
    cout << "just too cute. You stifle a giggle. ";
    cout << "He grins too and points to a chair, gesturing you to sit down.\n\n";

    cout << "You take your seat. The negotiation is about to begin!\n\n\n\n";

    inNegotiation = true;
    currentEncounter = level1;
    printHowToNegotiate();
}

void l2Start() { currentEncounter = level2; }
void l3Start() { currentEncounter = level3; }
void endStart() { currentEncounter = nullptr; }

void interact() {

    // Run introduction
    stage currentStage = intro;
    introStart();

    negotiationPossible = true;
    uInput = getUserInput();

    // Progress to Level 1
    currentStage = l1;
    l1Start();
    level1->beginEncounter(getRandWeight());



    uInput = getUserInput();



    // Progress to Level 2
    currentStage = l2;
    // uInput = getUserInput();
    // l2Start();

    // Progress to Level 3
    currentStage = l3;
    // uInput = getUserInput();
    // l3Start();

    // Run ending
    currentStage = ending;
    endStart();


}

int main() {
    cout << getRandWeight() << endl;
    cout << getRandWeight() << endl;
    cout << getRandWeight() << endl;
    cout << getRandWeight() << endl;
    cout << getRandWeight() << endl;

    player = new PlayerCharacter("You", MODERATE);

    createActions();
    createEconomy();

    Negotiator* porridge = new Negotiator("Porridge", FRIENDLY);
    Negotiator* rhubarb = new Negotiator("Rhubarb", GRUFF);
    Negotiator* chamoy = new Negotiator("Chamoy", GRUFF);

    ////////// Level 1 //////////
    level1 = new Encounter(player, porridge, NEGO1, L1_TURNS);
    Issue<float> key1("Porridge's Key", 10.0, 5.0, 15.0);
    // Issue<float> friendlifyPotion("Get Friendlify Potion", 15.0, 10.0, 20.0);
    // Add issues to encounter
    level1->loadFloatIssue(key1);
    // level1->loadFloatIssue(friendlifyPotion);

    ////////// Level 2 //////////
    level2 = new Encounter(player, rhubarb, NEGO2, L2_TURNS);

    ////////// Level 3 //////////
    level3 = new Encounter(player, chamoy, NEGO3, L3_TURNS);

    interact();

    delete porridge;
    delete rhubarb;
    delete chamoy;

    return 0;

}

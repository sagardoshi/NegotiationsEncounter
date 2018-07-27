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

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <random>

using namespace std;


const float FRIENDLY = 0.8;
const float GRUFF = 0.2;
const float MODERATE = 0.5;

enum dir {up, down, left, right};
enum stage {intro, l1, l2, l3, end};
string uInput = "";
stage current = intro;

PlayerCharacter player(MODERATE);

// A trivial 0-1 random generator engine from time-based seed
float getRandWeight() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator)/100.0;
}

// Prints ASCII art
void printGorilla() {
    cout << "          .-.      " << endl;
    cout << "        c(O_O)c    " << endl;
    cout << "       ,'.---.`,   " << endl;
    cout << "      / /|_|_|\\ \\ " << endl;
    cout << "      | \\_____/ |  " << endl;
    cout << "      '. `---' .`  " << endl;
    cout << "        `-...-'    " << endl;
}

void printInventory() {
    cout << "\n***************** INVENTORY *****************\n\n";
    if (!player.inventory.hasPomegranate && !player.inventory.hasKnucklePads &&
        !player.inventory.hasSilverbackPerfume && !player.inventory.hasBasket &&
        player.inventory.numGingerCookies == 0) {
        cout << "You have nothing in your inventory.\n\n";
        return;
    }

    cout << "In your inventory, you have: \n";
    if (player.inventory.hasPomegranate) cout << "--A pomegranate\n";
    if (player.inventory.hasKnucklePads) cout << "--Knuckle pads\n";
    if (player.inventory.hasSilverbackPerfume) cout << "--Silverback perfume\n";
    cout << "--" << player.inventory.numGingerCookies << " ginger cookies\n";
    if (player.inventory.hasBasket) cout << "All in a lovely basket.\n\n";

}

// Prints instructions
void printHelp() {
    cout << "\n******************* MENU *******************\n\n";
    cout << "[Type 'view' and hit return to read what you ";
    cout << "can see in front of you.]" << endl;

    cout << "[Type 'open' to try to open a door in front ";
    cout << "of you.]" << endl;

    cout << "[Type 'up', 'down', 'left', or 'right' to move ";
    cout << "forwards, backwards, left, or right.]" << endl;

    cout << "[Type 'quit' to quit the game.]" << endl;

    cout << "[Type 'help' to see these instructions.]" << endl;

    // cout << "[Type 'play' to return back to the game.]" << endl << endl;

    cout << "********************************************\n\n";
}

void handleMovement(string uInput) {
    string movement = uInput;
    if (uInput == "up") movement = "forward";
    if (uInput == "down") movement = "backward";

    cout << "You have moved " << movement << ".\n\n";
}

// Holds a vector of acceptable verb commands
vector<string> commands() {
    vector<string> acceptableCommands;

    acceptableCommands.push_back("view");
    acceptableCommands.push_back("open");
    acceptableCommands.push_back("up");
    acceptableCommands.push_back("down");
    acceptableCommands.push_back("left");
    acceptableCommands.push_back("right");
    acceptableCommands.push_back("quit");
    acceptableCommands.push_back("help");
    acceptableCommands.push_back("inventory");

    // acceptableCommands.push_back("play");

    return acceptableCommands;
}

// Rejects input not in list of acceptable verbs
bool isValidInput(string uInput) {
    vector<string> verbs = commands();

    for (int i = 0; i < verbs.size(); i++) {
        if (uInput == verbs[i]) return true;
    }

    return false;
}

// Gets simple, unverified user input and converts to lowercase
string getLowerCaseInput() {
    string uInput = "";
    cout << ">> ";
    cin >> uInput;
    ::transform(uInput.begin(), uInput.end(), uInput.begin(), ::tolower);
    return uInput;
}

// Fully abstracted function to call to get user input
string getUserInput() {
    string uInput = getLowerCaseInput();

    // Force fresh input until one of the valid options
    while(!isValidInput(uInput)) {
        cout << "Invalid input. Try again. Type 'help' for instructions.\n\n";
        uInput = getLowerCaseInput();
    }

    // If ever user input is quit, end game immediately
    if (uInput == "quit") exit(0);

    // If asking for help, allow a loop before returning
    if (uInput == "help") {
        printHelp();
        uInput = getUserInput();
    }

    // If asking for help, allow a loop before returning
    if (uInput == "inventory") {
        printInventory();
        uInput = getUserInput();
    }

    // If making a direction, note the movement, and return back
    if (uInput == "up" || uInput == "down" ||
        uInput == "left" || uInput == "right") {
        handleMovement(uInput);
        return uInput;
    }

    //
    // while (uInput != "play") uInput = getUserInput();

    return uInput;
}

void dialogueIntro() {
    cout << "CRASH!! You fall through a roof and land in a soft pile of ";
    cout << "of blankets. Ow. You shake your head as the dust settles and ";
    cout << "look up to see a large gorilla standing before you.\n\n";

    printGorilla();

    cout << "She has a long, rounded face, has a pair of thing glasses ";
    cout << "perched on her short nose, and is wearing a plain t-shirt and ";
    cout << "trousers. Her fur seems to be graying and fading a little around ";
    cout << "the edges. As the dust clears, it seems she was reading a ";
    cout << "magazine while sitting on her living room armchair.\n\n";

    cout << "She looks you over silently and purses her lips in confusion. ";
    cout << "You both freeze there for a few moments, considering the ";
    cout << "the situation. Suddenly, her lips widen into ";
    cout << "a broad, free smile.\n\n";

    cout << "--Hello there, young human! How did you get in our house?––\n\n";

    // TODO: Abstract this away later
    cout << "[Type \"1\" for \"Uh, I'm not really sure...\"]\n";
    cout << "[Type \"2\" for \"To meet you!\"]\n\n";
    //

    // TODO: Abstract this away later
    cout << ">> ";
    while (uInput != "1" && uInput != "2") cin >> uInput;
    uInput = "";
    //

    cout << "\n--Huh! Weird! Well, no matter. Welcome to the House of ";
    cout << "Negotiations! My name is Gori the Gorilla. Ook ook.--\n\n";

    cout << "She stands up, holding her bulk back on two powerful legs. She ";
    cout << "extends a muscular arm to you for a handshake and holds your ";
    cout << "hand ever so daintily and protectively. She steps back again.\n\n";

    cout << "--Well, friend, the rules of the House are simple. You may not ";
    cout << "leave and return to ";
    cout << "your normal life until you gain keys from at least three members ";
    cout << "of my family.--\n\n";

    cout << "--How to gain the keys, you ask? Oooooook.--\n\n";

    cout << "--Simple! Follow our favourite pastime! Haggle with us! ";
    cout << "If you can convince three of us to give you a key, you can head ";
    cout << "back to your normal life, no harm, no foul. Ooook.--\n\n";

    cout << "--What do you say? Sounds like fun, right?--\n\n";

    // TODO: Abstract this away later
    cout << "[Type \"1\" for \"Yeah, sure, sounds fun!\"]\n";
    cout << "[Type \"2\" for \"You're a bunch of weirdos! No way!\"]\n\n";

    cout << ">> ";
    while (uInput != "1" && uInput != "2") cin >> uInput;
    if (uInput == "1") {
        cout << "\nGori rubs her palms together in anticipation.\n\n";
        cout << "--Excellent!! This will be great fun. Ook ook!--\n\n";
    }
    if (uInput == "2") {
        cout << "\nGori rubs her chin thoughtfully.\n\n";
        cout << "--Okay, if you say so!-- she says, shrugging her shoulders. ";
        cout << "She reaches one powerful arm back, and swings it rapidly ";
        cout << "towards your face. The last thing you see is her sorrowful ";
        cout << "expression.\n\n";
        cout << "You have been killed by Gori the Gorilla.\n\n";
        exit(0);
    }
    uInput = "";
    //

    cout << "She throws up her hands and starts rummaging on the far side of ";
    cout << "her armchair. She emerges shortly thereafter with a basket.\n\n";

    cout << "--Here is your one and only tool. You may use these as you wish ";
    cout << "for your negotiations. Ook. However, this is all you have. It ";
    cout << "must last you all three negotiations. Understand? Good.--\n\n";

    cout << "She hands over the basket. You take it.\n\n";

    player.inventory.hasPomegranate = true;
    player.inventory.hasKnucklePads = true;
    player.inventory.hasSilverbackPerfume = true;
    player.inventory.hasBasket = true;
    player.inventory.numGingerCookies = 12;

    cout << "You have an inventory! At any time, type \"inventory\" in a ";
    cout << "command prompt to see what you have.\n\n";

    cout << "--You look worried!-- She laughs. --Don't look worried! What's ";
    cout << "the worst that can happen?-- Suddenly, Gori takes on a wild, ";
    cout << "terrifying aspect. --After all,-- she says, --It's not like ";
    cout << "you're negotiating for your life...--\n\n";

    cout << "She brightens, and the smile takes over her face once more.\n\n";

    cout << "--And you'll start easy, by negotiating with my young ";
    cout << "grandson, Porridge. He's still learning the family game, ";
    cout << "so it will be good practice for him too.--\n\n";

    cout << "She pauses for a moment and considers.\n\n";

    cout << "--Since you're new to this, why don't I give you a little boost ";
    cout << "to help you? I'll give you a basket of goodies. It? Ook.  Take this pomegranate. It's one of Porridge's ";
    cout << "favourites! Perhaps it will help you be more convincing.--\n\n";

    cout << "You've gained a Pomegranate!\n\n";

    cout << "--Alright, then! He's just down the hall. Go left here, and then ";
    cout << "just open his door. Good luck! Ook ook!--";

    cout << "Gori settles back onto her chair, opens her magazine, and pays ";
    cout << "you no heed.\n\n";

    cout << "(p.s. if you're stuck, type 'help')\n\n";

}

void l1Start() {
    cout << "The door has a chalkboard taped to it. On the chalkboard, ";
    cout << "you can see the name \"Porridge\" scribbled with childish, ";
    cout << "messy handwriting. You open the door.\n\n";

    cout << "Within, you see a tiny gorilla, wearing a baseball cap and ";
    cout << "throwing a ball against the wall. When you walk in, he pauses.\n\n";

    cout << "--Hi! You must be here for a negotiation! Grandma said someone ";
    cout << "would come knocking this morning. If you want me to give you my ";
    cout << "key, though, I'll warn you that I drive a hard bargain...--\n\n";

    cout << "He strikes a fierce pose, but the effect is just too cute. ";
    cout << " He points to a chair and gestures you to sit down. You do.\n\n";

    cout << "His eyes are bright. --Okay, friend! The way this works is that ";
    cout << "we lay out all the issues on the table, and we each decide ";
    cout << "how much each matters to us.";
}

void interact() {

    dialogueIntro();


    // Get user input and advance level if moved left
    uInput = getUserInput();
    if (current == intro && uInput == "left") {
        current = l1;
        cout << "You turn left and go down a hallway for a while. It's ";
        cout << "brightly lit, with colourful flowers everywhere. ";
        cout << "You see framed photos of the large Gorilla family everywhere.\n\n";
        cout << "You keep moving down the hallway until you get to a door.\n\n";
    }

    // Get user input and see if level 1 is ready to begin
    uInput = getUserInput();
    if (current == l1 && uInput == "open") l1Start();



}

int main() {
    // getRandWeight();

    interact();

    cout << "\n===============================" << endl;
    cout << "Testing the Negotiations Engine" << endl;
    cout << "===============================" << endl;
    cout << endl;


    Encounter* currentLevel = nullptr;

    Negotiator* friendlyNegotiator = new Negotiator(FRIENDLY);
    Negotiator* gruffNegotiator = new Negotiator(GRUFF);


    ////////// Level 1 //////////
    // Encounter, made up of two issues
    Encounter level1(friendlyNegotiator);
    // Issues
    Issue<float> keyPrice("Key Price", 10.0);
    Issue<bool> friendlifyPotion("Get Friendlify Potion", false);
    // Add issues to encounter
    level1.loadFloatIssue(keyPrice);
    level1.loadBoolIssue(friendlifyPotion);


    level1.printOfferOnTable();


    player.enterNegotiation();

    cout << "===============================" << endl;
    cout << "Closing the Negotiations Engine" << endl;
    cout << "===============================" << endl;
    cout << endl;

    delete friendlyNegotiator;
    delete gruffNegotiator;

    return 0;

}

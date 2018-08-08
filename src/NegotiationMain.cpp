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
#include <algorithm>

using namespace std;

void createActions() {
    // Acceptable actions more generally
    genActions.push_back("help");
    genActions.push_back("quit");
    genActions.push_back("inventory");

    // Negotiation actions
    negoActions.push_back("turns");
    negoActions.push_back("issues");
    negoActions.push_back("propose");
}

void createEconomy() {
    // name: [default price, how many owned]
    economy["pomegranate"] = 10;
    economy["knuckle pads"] = 15;
    economy["silverback perfume"] = 10;
    economy["ginger cookie"] = 5;
    economy["coins"] = 1;
    economy["basket"] = 2;
    economy["porridge's key"] = 30;
    economy["rhubarb's key"] = 40;
    economy["chamoy's key"] = 50;
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
        cout << "[propose: use a turn to build your own offer]\n";
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

    cout << "[Everyone has a distinct personality. Porridge, for example, ";
    cout << "is a slightly impulsive, but friendly kid. He's accommodating ";
    cout << " and easy to please, especially if you offer him something he ";
    cout << "likes. Others, however, may not be quite as straightforward.]\n\n";

    cout << "[You have a limited number of rounds before your opponent gets ";
    cout << "impatient and gives up on you. At any time, you can type ";
    cout << "\"turns\" to check your deadline.]\n\n";

    cout << "[To see what you're negotiating over, type \"issues\".]\n\n";

    cout << "[The core of the negotiation is to propose tantalising offers ";
    cout << "to your opponent. ";
    cout << "In order to do that here, you can type \"propose\" to ";
    cout << "craft one by selecting items from your inventory to package ";
    cout << "together into an offer and send it. Each proposal uses ";
    cout << "up one turn.]\n\n";

    cout << "[Remember: be careful with your inventory. It is your primary ";
    cout << "resource. The more you can keep, the higher your final score. ";
    cout << "So be cautious with your proposals.]\n\n";
    cout << "***********************************************************\n\n";
}


void checkpoint(bool allKeys = true) {

    uInput = "";
    cout << "\n\n---------------------------\n";
    if (allKeys) cout << "[Press return to continue.]  ";
    else cout << ">>>> ";
    getline(cin, uInput);
    cout << "---------------------------\n\n\n\n";

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
            else if (uInput == "propose" && inNego) {
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
        cout << "[3: \"It's a long story involving extreme sports and ";
        cout << "catapults...\"]\n\n";

        getStandardisedInput();
        while (uInput != "1" && uInput != "2" && uInput != "3") {
            cout << "[Invalid input. Try again.]\n\n";
            cout << "[1: \"Uh, I'm not really sure...\"]\n";
            cout << "[2: \"To meet you!\"]\n";
            cout << "[3: \"It's a long story involving extreme sports and ";
            cout << "catapults...\"]\n\n";

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
    cout << "look up to see a large gorilla reclining before you.\n\n";

    printGorilla();

    cout << "She has a long, rounded face, has a pair of thin glasses ";
    cout << "perched on her short nose. ";
    cout << "Her hair is graying a little around ";
    cout << "the edges. As the dust clears, it seems she was napping or ";
    cout << "reading a magazine whilst sitting on her living room ";
    cout << "armchair.\n\n";

    checkpoint(anyKeyOK);

    cout << "She looks you over silently and purses her lips in confusion. ";
    cout << "You freeze for a few moments, and she seems to be blinking the ";
    cout << "sleep out of her eyes.";
    cout << "Suddenly, she speaks in a rumble. \"Hello there, young human! How did you land in our nest?\"\n\n";


    tutorialChoices("firstTutorialQuestion");
    uInput = ""; // Reset uInput

    cout << "\n\"Hmm...\" she wipes her face. \"Well, no matter. You're ";
    cout << "here now, and you will have to deal with the consequences. ";
    cout << "But for now, it's good to meet you! My name is Chamoy. ";
    cout << "Welcome to the Tripgor Family Nest.\"\n\n";

    cout << "She stands up, an operation that sounds and looks like grand ";
    cout << "boulders grinding into a new position. She holds her bulk low ";
    cout << "on two powerful legs. Her lightening hair and age belies the ";
    cout << "metal power coiled in her form. She extends a meaty ";
    cout << "arm to you for a handshake. You reach, and she daintily grasps ";
    cout << "your hand, as if she were holding a teacup.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"Well, friend, you intruded ";
    cout << "on my family's peace and quiet. You'd be surprised how often ";
    cout << "this happens, and how little you humans seem to care. We can't ";
    cout << "run a household this way. And so, we have had to apply some ";
    cout << "serious rules to disincentivise projectile humans like you.\"\n\n";

    cout << "You gulp, still sore from your crash landing and silently ";
    cout << "cursing the trust you placed in the catapult technician.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"As of now,\" she says, while casually cleaning an ear with ";
    cout << "a thick finger, \"your life is forfeit. You left ";
    cout << " your human lands and ";
    cout << "arrived here in the Gorilla Nation. Here, our law applies. ";
    cout << "Our law states that if someone invades a nest, whatever the ";
    cout << "homeowners decide to do goes.\" She pauses here, checking to see ";
    cout << "how you take this. You keep still and try to avoid reacting.\n\n";

    cout << "\"However,\" she quips, \"we are not unreasonable people, ";
    cout << "here ";
    cout << "in the Tripgor family. We're a light-hearted bunch!\" She grins ";
    cout << "widely here, a terrifying sight that reveals her fangs. \"";
    cout << "Despite the damage you have caused, our rule is to allow ";
    cout << "intruders to win their freedom... but you need to earn it. ";
    cout << "If you can convince each one of my family members, well, then ";
    cout << " that's a good enough endorsement from me.\"\n\n";

    checkpoint(anyKeyOK);

    cout << "Chamoy leans closer to you, and the swiftness of her movement ";
    cout << "startles you. \"How to convince them, you ask?\"\n\n";

    cout << "\"Simple! Follow our favourite pastime! Negotiate with us! ";
    cout << "If you can earn a key from everyone in formal negotiation, ";
    cout << "you can head ";
    cout << "back to your normal life, no harm, no foul.\"\n\n";

    cout << "\"What do you say? Sounds like fun, right?\"\n\n";


    tutorialChoices("secondTutorialQuestion");

    if (uInput == "1") {
        cout << "Chamoy rubs her palms together in anticipation.\n\n";
        cout << "\"Excellent! Just what I was hoping to hear! Let's see ";
        cout << "if you can be the first human to make it through ";
        cout << "all of us.\"\n\n";
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

        cout << "You have been killed by Chamoy Tripgor.\n\n";

        cout << "Perhaps you might do better in another life if you don't ";
        cout << "antagonise the very strong matriarch whose home you ";
        cout << "invaded.\n\n";

        cout << "Goodbye, and happy haggling!\n\n\n\n";
        exit(0);
    }

    uInput = ""; // Reset uInput


    cout << "She scratches her elbows. \"Ah, and before you ";
    cout << "start, I have something for you.\" She begins rummaging ";
    cout << "behind her armchair, muttering to herself. She emerges shortly ";
    cout << "with a small wicker picnic basket.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"Take this. You may use the items within as you wish ";
    cout << "for your negotiations. You can trade these away for the keys, ";
    cout << "but I can't guarantee that everyone will want everything. ";
    cout << "However, this is all you get: it must last you all three ";
    cout << " negotiations. Whatever you end with is yours to keep. ";
    cout << "Understand? Good.\" ";
    cout << "She hands over the basket.\n\n";

    player->fillInventory();

    cout << "[You have an inventory! When negotiating, type \"inventory\" to see what you have.]\n\n";

    checkpoint(anyKeyOK);

    cout << "\"You look worried!\" She laughs. \"Don't! What's ";
    cout << "the worst that can happen?\" Suddenly, Chamoy reassumes her ";
    cout << "wild, terrifying aspect. \"After all, it's not like ";
    cout << "you're negotiating for your life...\"\n\n";

    checkpoint(anyKeyOK);

    cout << "Her face clears and she looks sleepy and calm again.\n\n";
    cout << "\"And you'll start easy, by negotiating with my young ";
    cout << "grandson, Porridge.\" She points down a hallway to your left. ";
    cout << "\"He's still learning the family game, ";
    cout << "so it will be good practice for both of you.\"\n\n";

    cout << "She looks pensive. \"Well, I guess he'll still be alright if ";
    cout << "he doesn't do well, whereas you...\" she trails off. \"Well, ";
    cout << "anyway, good luck, then!\n\n";

    cout << "Chamoy settles back onto her chair, and falls asleep before you ";
    cout << "even have time to move.\n\n";

    checkpoint(anyKeyOK);

}

void l1Start() {
    cout << "You turn left and go down a hallway for a while. It's ";
    cout << "brightly lit, with colourful flowers everywhere. ";
    cout << "You see framed photos of the Tripgor family. There seem to be ";
    cout << "three of them. You recognise Chamoy, and then someone who looks ";
    cout << "like her daughter, and someone who must be the grandson, ";
    cout << "Porridge.\n\n";

    cout << "You keep moving down the hallway until you get to a door. ";
    cout << "The door has a chalkboard taped to it with \"Porridge\" ";
    cout << "scribbled in childish, messy handwriting. You open the door.\n\n";

    checkpoint(anyKeyOK);

    cout << "Within, you see a tiny gorilla, wearing a baseball cap and ";
    cout << "tossing a ball against the wall. The room has a soft mattress ";
    cout << "with drawings pasted up on the walls. There's a toy closet, and ";
    cout << "a bright window shining a circle of light on the wall just ";
    cout << "where Porridge is aiming his ball.\n\n";

    cout << "When you walk in, he pauses. ";
    cout << "\"Hi! You must be here for a negotiation! Grandma said someone ";
    cout << "would likely come knocking this morning. If you want my ";
    cout << "key, though, I'll warn you that I drive a hard bargain, and I'm ";
    cout << "the toughest negotiator in the family! You stand no chance!\"\n\n";

    checkpoint(anyKeyOK);

    cout << "He strikes a fierce pose, pouting his lips, but the effect is ";
    cout << "just too cute. You stifle a giggle. ";
    cout << "He laughs too and points to a chair.\n\n";

    cout << "You take your seat. The negotiation is about to begin!\n\n";

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
    cout << "the end of the hallway is another door, parallel to ";
    cout << "Porridge's. This one has a cleanly etched name on it: ";
    cout << "\"Rhubarb.\"\n\n";

    checkpoint(anyKeyOK);

    cout << "The door is slightly ajar, and you knock lightly and poke your ";
    cout << "head around. The room is a studied contrast to Porridge's. When ";
    cout << "you step inside, you are greeted with an explosion of scent. ";
    cout << "At first glance, the room seems like an indoor forest, but it's ";
    cout << "far more organised than that.\n\n";

    cout << "The walls to the left and right have neat, white wooden shelves ";
    cout << "which are filled to the limit with plant cuttings. Some are ";
    cout << "wrapped with twine in bundles, some are living in small pots, ";
    cout << "but each one is labeled, catalogued, and placed precisely.\n\n";

    checkpoint(anyKeyOK);

    cout << "After you take all this in, you notice the gorilla in the back ";
    cout << "of the room. She is far larger than Porridge, looming behind ";
    cout << "a tiny, but delicately arranged small desk. She has a pen in ";
    cout << "hand and appears to be making a sketch of a sprig of rosemary. ";
    cout << "This must be Rhubarb, Porridge's mother and ";
    cout << "(presumably) Chamoy's daughter. And indeed, she shares a nose ";
    cout << "shape with both of them. She has a dignified, magisterial ";
    cout << "air. More serious than Porridge, less relaxed than Chamoy.\n\n";

    checkpoint(anyKeyOK);

    cout << "Rhubarb takes off her glasses and sighs, rubbing her temples. ";
    cout << "You're the latest intruder, then?\" she asks.\n\n";

    cout << "You nod, and then feel absurd for agreeing to name yourself an ";
    cout << "intruder, when it's the catapult technician's fault you are ";
    cout << "here anyway.\n\n";

    checkpoint(anyKeyOK);

    cout << "\"Alright. I don't know why Mother persists in playing these ";
    cout << "games. I would rather just do away with you all and send the ";
    cout << "message loud and clear that we won't stand for this home ";
    cout << "invasion. It would waste far less time. But oh, what do I know? ";
    cout << "I only keep this nest functioning.\" She sighs again, a heavy, ";
    cout << "deep affair, and gestures to the seat across from her.\n\n";

    cout << "\"Well,\" she says, \"let's get this over with. What do you ";
    cout << "have for me?\"\n\n";

    checkpoint(anyKeyOK);

    currNego = level2;
}


void l3Start() {

    checkpoint(anyKeyOK);

    cout << "[More flavour text to be filled out here, showing your ";
    cout << "triumphant return from Rhubarb, and your approach to Chamoy, ";
    cout << "weakened, with fewer items in your inventory, but nearly at the ";
    cout << "end.]\n\n";

    checkpoint(anyKeyOK);

    currNego = level3;

}

void endStart() { currNego = nullptr; }

void interact() {

    // Run introduction
    // stage currentStage = intro;
    introStart();

    // Progress to Level 1
    // currentStage = l1;
    l1Start();
    inNego = true;
    bidAccepted = false;
    level1->beginEncounter();
    getUserInput();
    inNego = false;


    // Progress to Level 2
    // currentStage = l2;
    l2Start();
    inNego = true;
    bidAccepted = false;
    level2->beginEncounter();
    getUserInput();
    inNego = false;


    // Progress to Level 3
    // currentStage = l3;
    l3Start();
    inNego = true;
    bidAccepted = false;
    level3->beginEncounter();
    getUserInput();
    inNego = false;

    // Run ending
    // currentStage = ending;
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

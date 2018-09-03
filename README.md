# Dealing with the Spirits

## Description

_Dealing with the Spirits_ is a text-based negotiations video game. In this world, powerful spirits dominate the world, and humans are considered barbaric, less than vermin. You are a desperate human thief trying to steal yourself a better life. Your plan is to steal whatever you can from the great spirit House of Healing.

Unfortunately, when you're caught, you need to use your wits -- and whatever items you had been able to grab -- to negotiate your way past a series of spirits, each of which might help you escape... if you offer them just the right things.

Can you make it back home with as many purloined goods as possible?

## Playing the Game
To install, follow the instructions below, and once you set it up once, you only need to type `make run` to play in the future. This is an interactive fiction game, so there's a fair bit of reading, broken up by interactive negotiations.

Don't worry if you need to take a break! Just type `quit` at any time, and the game will automatically save your inventory and the level where you are. You'll have the option to restart your save when you next play.

## Author
Hey! It's me, Sagar. Hope you like my game. If you want to see more of my stuff – including more traditional games – check out my website at https://www.hellosagar.com/

## Acknowledgments
This game is being made as a master's project for an MSc in Computing Science at Imperial College London, advised by Fariba Sadri and Anandha Gopalan. Thank you to everyone involved!

## Feedback

I am still updating and tweaking this game. If you have any ideas or feedback, or if you find any problems, please feel free to contact me at doshi "dot" sd "at" gmail "dot" com.

# Tech Stuff

## Installation
1. Clone this repository or download this package as a zip (extra help [here](https://help.github.com/articles/cloning-a-repository/)).
1. Open up a terminal window (on Mac and Linux, you can use the Terminal application for this).
1. Navigate to where you saved the package. (extra help [here](https://www.macworld.com/article/2042378/master-the-command-line-navigating-files-and-folders.html)).
1. Make your terminal window full screen, or at least 80 characters wide.
1. When in the root directory, type `make go` and hit return. The game should create and run itself here.
1. If you ever run into any trouble, clear the game with `make clean`, and then try running `make` and then `make run` once more.

## Uninstallation
Simply delete the folder you downloaded, wash your hands, and have a little party.

## Software Requirements

This game currently works directly through the command line, with a simple, C++ codebase. You should ideally have C++11 installed. The game is optimised for Mac and Linux usage -- it may work on Windows machines with a command line client like PuTTY, but this has not been tested. Beyond this, all reasonably modern hardware should suffice.

Because this is a command line-heavy game, it relies on `make` and `g++`. These are common functions used across many programs. However, many Macs don't install them from the beginning. If this is you, you may have to install Command Line Tools first.

The commands you get with Command Line Tools are quite useful more generally. However, it can be a heavy installation to get that going, because Apple bundles their big Xcode application with it. To install Command Line Tools much more quickly, without Xcode, follow these instructions (also listed on [this website](https://www.maketecheasier.com/install-command-line-tools-without-xcode/)):
1. Open up a Terminal window. If you're not sure how, click on the Spotlight (magnifying glass icon) search on the top right of your screen. Type ``terminal``, and hit return when the black icon with the ">\_" symbols show up.
1. In your terminal, type `xcode-select --install`.
1. If you already have Command Line Tools, it will say `xcode-select: error: command line tools are already installed, use "Software Update" to install updates`, and that means you're good to go!
1. If you don't already have it, you will get a prompt asking you to install command line developer tools. Tap the install button on the right. Agree to the license agreement, and wait for the download to complete.
1. Now you're good to go to play my game (and do a whole bunch of other things with Unix commands, if you wish!).

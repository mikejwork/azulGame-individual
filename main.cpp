#include <string>
#include <iostream>

#include "Settings.h"
#include "Menu.h"

void printError();
void runTestingMode(std::string fileName);

int main(int argc, char *argv[])
{
    // If the number of arguments passed is 1
    // we continue on as usual and no global settings are changed
    if (argc == 1)
    {
        // Create a new menu object and display the menu
        Menu* menu = new Menu();
        menu->displayMenu();
    } 
    // If the number of arguments pass is 3,
    // Assume that the user is trying to access testing mode / other setting arguments
    else if (argc == 2)
    {
        std::string parameter ( argv[1]);
        if (parameter == "--ai")
        {
            // Set the global variable to true, this will change the way our game is played
            aiEnabled = true;
            playerCount = 1;

            Menu* menu = new Menu();
            menu->displayMenu();
        }
    }

    else if (argc == 3)
    {
        // The passed parameter, "-t" or "-s", etc..
        std::string parameter ( argv[1]);
        // The passed option after the argument,
        // save file name, seed, etc..
        std::string argument (argv[2]);

        // If user has selected testing mode
        if (parameter == "-t")
        {
            runTestingMode(argument);
        }
        // If user has selected seed mode
        else if (parameter == "-s")
        {
            // Set the global setting to true, this will change different aspects of our main program
            seedRandom = true;
            // atoi will convert ASCII to int
            seed = atoi(argv[2]);

            // Create a new menu object and display the menu
            Menu* menu = new Menu();
            menu->displayMenu();
        }
        else { printError(); }
    }
    else { printError(); }

    return EXIT_SUCCESS;
}

// Prints the correct usage of the program when the user makes a mistake
void printError()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\tazul" << std::endl;
    std::cout << "\tOR" << std::endl;
    std::cout << "\tazul -t < filename" << std::endl;
}

void runTestingMode(std::string fileName)
{
    std::ifstream file(fileName);

    GameIO* gameIO = new GameIO(nullptr, &file, nullptr);
    GameManager* gameManager = gameIO->loadGame();
    delete gameIO;

    if (gameManager == nullptr)
    {
        std::cout << "ERROR - " << fileName << " is not a valid save file." << std::endl;
    }
    else
    {
        gameIO = new GameIO(gameManager, nullptr, &std::cout);
        gameIO->printGameState();
        delete gameIO;
        delete gameManager;
    }
    
}
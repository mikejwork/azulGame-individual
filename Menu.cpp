#include "Menu.h"

#define NEW_GAME 1
#define LOAD_GAME 2
#define CREDITS 3
#define QUIT 4

Menu::Menu()
{
    std::cout << "Welcome to Azul!" << std::endl << "--------------------\n";
    std::cout << "#Settings" << std::endl;
    std::cout << "Seed Randomness: ";
    if (seedRandom)
    {
        std::cout << "true \n";
    }
    else
    {
        std::cout << "false \n";
    }
    std::cout << "Versus AI: ";
    if (aiEnabled)
    {
        std::cout << "true \n";
    }
    else
    {
        std::cout << "false \n";
    }
    std::cout << "Advanced Mode: ";
    if (advanced)
    {
        std::cout << "true \n";
    }
    else
    {
        std::cout << "false \n";
    }
    std::cout << "\n";
}

int getInput()
{
    int input = 0;
    bool valid = true;

    do
    {
        std::cout << "> ", std::cin >> input;
        valid = true;
        if (std::cin.fail() || input > 4 || input < 1)
        {
            std::cin.clear();
            std::cin.ignore(9999999, '\n');
            std::cout << "Invalid input" << std::endl;
            valid = false;
        }

    } while (!valid);

    return input;
}

void displayCredits()
{
    std::cout << std::endl
              << "---------------------------------- \n"

              << "Name: Braden Smith \n"
              << "Student ID: s3787174 \n"
              << "Email: s3787174@student.rmit.edu.au \n \n"

              << "Name: Michael Jurie \n"
              << "Student ID: s3785631 \n"
              << "Email: s3785631@student.rmit.edu.au \n \n"

              << "Name: Issa Chanzi \n"
              << "Student ID: s3783615 \n"
              << "Email: s3783615@student.rmit.edu.au \n"

              << "---------------------------------- \n \n";
}

void Menu::displayMenu()
{
    std::cout << "Menu" << std::endl
              << "----" << std::endl
              << "1. New Game \n"
              << "2. Load Game \n"
              << "3. Credits \n"
              << "4. Quit \n \n";
    // Take int input and validate it
    int input = getInput();
    // handleInput will decide what to do with each of the options
    handleInput(input);
}

void Menu::handleInput(int input)
{
    if (input == NEW_GAME)
    {
        // Create a new gameManager object and start a fresh game
        gameManager = new GameManager();
        startGame();
    }
    else if (input == LOAD_GAME)
    {
        // Load game from save file and continue from the state at which it was saved at.
        if (loadGame()) {
            startGame();
        }
    }
    else if (input == CREDITS)
    {
        // Show the credits and send the user back to the menu
        displayCredits();
        displayMenu();
    }
    else if (input == QUIT)
    {
        // Exit the program.
        exit(0);
    }
}

void Menu::startGame()
{
    GameIO* gameIO = new GameIO(gameManager, &std::cin, &std::cout);

    while(gameManager->roundsPlayed < 5)
    {
        gameIO->executeRound();
        std::cout << "\n=== END OF ROUND === \n";
    }
    gameIO->printFinalScores();
}

// Load a game from a save file
bool Menu::loadGame()
{
    bool completed = true;

    // Get the filename from user
    std::cout << "Enter file name\n> ";
    std::string fileName;
    std::cin >> fileName;

    // Open a stream to the file and create a new gameIO with a nullptr OUT
    // This will silence any output whilst loading the game
    std::ifstream file(fileName);
    GameIO* gameIO = new GameIO(nullptr, &file, nullptr);

    gameManager = gameIO->loadGame();
    delete gameIO;

    if (gameManager == nullptr)
    {
        std::cout << "ERROR - " << fileName << " is not a valid save file.\n";
        completed = false;
    }
    return completed;
}
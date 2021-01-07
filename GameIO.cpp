#include <limits>

#include "GameIO.h"

#define IN (*in)   // Used for CIN
#define OUT (*out) // Used for COUT

/* ----- Null output for loadGame ----- */
class NullStream : public std::streambuf
{
public:
    int overflow(int ch)
    {
        return ch;
    }
    std::streamsize sputn(const char_type *s, std::streamsize count)
    {
        return count;
    }
    static NullStream instance;
};

NullStream NullStream::instance;
std::ostream *GameIO::nullOutput = new std::ostream(&NullStream::instance);
/* ----- Null output for loadGame ----- */

GameIO::GameIO(GameManager *gameManager, std::istream *in, std::ostream *out)
{
    this->gameManager = gameManager;
    this->in = in;
    this->out = out;

    // If no output stream is set, use the null stream we have created
    // This will output nothing so that we can load the game nicely
    if (this->out == nullptr)
    {
        this->out = nullOutput;
    }
}

void GameIO::executeRound()
{
    OUT << std::endl
        << "=== Start Round === " << std::endl;
    // Execute the turns while the factories are not empty, if they are the round will end.
    while (!gameManager->allFactoriesEmpty())
    {
        executeCommand();
    }
    gameManager->endRound();
}

void GameIO::executeCommand()
{
    Player *player = gameManager->getNextPlayer();
    std::string playerName = player->getName();

    OUT << "Turn for player: " << playerName << std::endl;

    printFactories();
    printMosaic(player);

    if (player->isAI())
    {
        executeTurn();
    }
    else
    {
        OUT << "> ";

        std::string command;
        IN >> command;

        if (command == "turn")
        {
            executeTurn();
        }
        else if (command == "save")
        {
            saveGame();
        }
        else if (IN.eof())
        {
            OUT << "Exiting program.." << std::endl;
            exit(0);
        }
        else
        {
            OUT << "Invalid command" << std::endl;
            OUT << "Valid commands are:" << std::endl;
            OUT << "\tturn <factory> <colour> <row>" << std::endl;
            OUT << "\tsave <filename>" << std::endl;

            // Flush buffer
            IN.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    IN.clear();
}

bool GameIO::executeTurn()
{
    bool success = true;
    Turn* turn;
    if (gameManager->getNextPlayer()->isAI())
    {
        turn = gameManager->getNextPlayer()->aiBrain->processAiTurn(gameManager->getNextPlayer()->getMosaic(), gameManager->factories);
    }
    else
    {
        turn = newTurn();
    }
    if (turn != nullptr)
    {
        // If the turn is valid and not a nullptr, execute the turn in the game manager
        gameManager->turn(turn);
    }
    else
    {
        success = false;
    }
    return success;
}

bool GameIO::executeLoadingTurn() // TODO AI Implementation
{
    bool success = true;
    Turn *turn = newTurn();
    if (turn != nullptr)
    {
        // If the turn is valid and not a nullptr, execute the turn in the game manager
        gameManager->turn(turn);
        if (gameManager->allFactoriesEmpty())
        {
            gameManager->endRound();
            gameManager->roundsPlayed++;
        }
    }
    else
    {
        success = false;
    }
    return success;
}

Turn *GameIO::newTurn()
{
    Turn *turn = nullptr;

    int factory;
    int row;
    unsigned char colour;

    IN >> factory;
    IN >> colour;
    IN >> row;

    colour = std::toupper(colour);

    if (colour == 0)
    {
        turn = nullptr;
    }
    else if (colour == 'F')
    {
        OUT << "you cannot move the 'F' tile! \n";
        turn = nullptr;
    }

    else if (!gameManager->validFactory(factory))
    {
        OUT << "Factory out of bounds." << std::endl;
        turn = nullptr;
    }

    else if (!gameManager->validRow(row))
    {
        OUT << "Row out of bounds." << std::endl;
        turn = nullptr;
    }
    else if (gameManager->getNextPlayer()->getMosaic()->checkPlace(colour, row))
    {
        OUT << "Player cannot place any more tiles, moving to broken" << std::endl;
        turn = new Turn(factory, row, colour);
    }

    //RECENTLY ADDED: THREE IF STATEMENTS AFTER MERGE
    // Checking if the mozaic tile has already been filled in that row
    else if (gameManager->getNextPlayer()->getMosaic()->checkLine(colour, row))
    {
        OUT << "Cannot place tile(s) there! \n";
        turn = nullptr;
    }

    // Checking if selected factory actually contains the tile the player is requesting
    else if (gameManager->factories[factory]->getAmountOf(colour) == 0)
    {
        OUT << "Factory does not contain that tile! \n";
        turn = nullptr;
    }

    // Checks if the selected row is already full
    else if (gameManager->getNextPlayer()->getMosaic()->isRowFull(row))
    {
        OUT << "Row is already full \n";
        turn = nullptr;
    }
    else
    {
        turn = new Turn(factory, row, colour);
    }

    return turn;
}

void GameIO::printFactories()
{
    OUT << "Factories: " << std::endl;
    gameManager->printFactories(OUT);
}

void GameIO::printMosaic(Player *player)
{
    std::string playerName = player->getName();
    Mosaic *mosaic = player->getMosaic();

    OUT << "Mosaic for: " << playerName << std::endl;
    OUT << *mosaic << std::endl;
}

void GameIO::printScore(Player *player)
{
    std::string playerName = player->getName();
    int score = player->getPoints();

    OUT << "Score for player " << playerName << ": " << score << std::endl;
}

void GameIO::printTurn()
{
    Player *player = gameManager->getNextPlayer();
    std::string playerName = player->getName();

    OUT << "Turn for player: " << playerName << "\n";

    printFactories();
    printMosaic(player);
}

void GameIO::printFinalScores()
{
    OUT << "=== GAME OVER ===\n\n";
    OUT << "Final Scores: \n";
    OUT << "Player " << gameManager->getNextPlayer()->getName() << ": " << gameManager->getNextPlayer()->getPoints() << std::endl;
    gameManager->cyclePlayers();
    OUT << "Player " << gameManager->getNextPlayer()->getName() << ": " << gameManager->getNextPlayer()->getPoints() << std::endl;
    gameManager->cyclePlayers(); //need to remove, this is just for testing
    OUT << gameManager->getWinningPlayerName();
}

void GameIO::printGameState()
{
    printFactories();

    std::vector<Player *> players = gameManager->getPlayers();

    for (int i = 0; i < (int)players.size(); i++)
    {
        Player *player = players[i];

        printScore(player);
        printMosaic(player);
    }
}

GameManager *GameIO::loadGame()
{
    delete this->gameManager;

    // Junk - just skipping a line in the save file
    std::string settingsHeader;
    IN >> settingsHeader;

    // Reads the saved seed
    // Read into the global variable
    IN >> seed;
    if (seed != 0)
        seedRandom = true;

    // Check if AI is enabled in the settings
    int AI;
    IN >> AI;
    if (AI != 0)
        aiEnabled = true;

    // Load the starting tile bag.
    std::string startingTileBag;
    IN >> startingTileBag;

    // Load the saved box lid.
    //std::string boxLid;
    //IN >> boxLid;

    this->gameManager = new GameManager(startingTileBag); // TODO: Change so that the starting tiles are loaded

    // Load the players
    loadPlayers();

    // Check if there are any stream errors or if the "end-of-file" character
    // if so it will return nullptr which will prompt the user that the save file
    // is invalid
    if (IN.fail() || IN.eof())
    {
        this->gameManager = nullptr;
    }
    else
    {
        bool running = true;
        while (running)
        {
            std::string test;
            IN >> test;
            if (!executeLoadingTurn())
            {
                running = false;
            }
        }
    }
    OUT << "Successfully loaded save" << std::endl;
    return this->gameManager;
}

void GameIO::saveGame()
{
    std::string fileName;
    IN >> fileName;
    std::ofstream file(fileName);
    file << *gameManager;
    OUT << "Game successfully save to " << fileName << "\n \n"; // Double newline to look a bit better
}

void GameIO::loadPlayers()
{
    if (aiEnabled)
    {
        // Add the initial first player
        std::string playerName;
        std::string dummyName;
        int playerNum = gameManager->countPlayers() + 1;

        OUT << "Enter a name for player " << playerNum << std::endl
            << "> ";
        IN >> playerName;
        IN >> dummyName; // need to take another input that isnt used, because the AI name is still saved
        OUT << std::endl;
        gameManager->addPlayer(new Player(playerName));

        // Add the AI player
        gameManager->addPlayer(new Player("AI", true));
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            std::string playerName;
            int playerNum = gameManager->countPlayers() + 1;

            OUT << "Enter a name for player " << playerNum << std::endl
                << "> ";
            IN >> playerName;
            OUT << std::endl;
            gameManager->addPlayer(new Player(playerName));
        }
    }
}
#include "GameManager.h"

#define NUM_PLAYERS 2
#define NUM_FACTORIES 6
#define MAX_TILES 4

GameManager::GameManager()
{
    this->tileBag = new TileBag();
    this->boxLid = new BoxLid();

    std::cout << "\nStarting a new game \n \n";
    setupPlayers();
    initializeFactories();
    std::cout << "Let's Play! \n";
}

GameManager::GameManager(std::string startingTiles)
{
    this->tileBag = new TileBag(startingTiles);
    this->boxLid = new BoxLid();
    initializeFactories();
}

void GameManager::endRound()
{
    updateMosaics();
    fillFactories();
    playerTurn = playerFirstNextRound;
    // reset the state of the first tile taken variable
    firstTileTaken = false;
    roundsPlayed++;
}

void GameManager::updateMosaics()
{
    for (int i = 0; i < 2; i++) // TODO AI Fix - 2 would be playerCount but its set to 1 when AI is enabled
    {
        Player* player = players[i];
        player->getMosaic()->updateMosaic(*boxLid, *tileBag);
        player->addPoints(player->getMosaic()->getPlayerPoints());

        if (seedRandom)
        {
            player->getMosaic()->brokenToBoxLid(*boxLid);
        } else {
            player->getMosaic()->returnBroken(tileBag);
        }
    }
    
    // Enable these to see the state of the tilebag / boxlid at the end of a round - Note: this shows the tilebag at the end of round BEFORE the new factories have been created
    /*
    std::cout << "[Debug] Current TileBag: " << this->tileBag->toString() << std::endl;
    std::cout << "[Debug] Current BoxLid: ";
    boxLid->dfs();
    std::cout << std::endl;
    */
}

/* Turns */
void GameManager::turn(Turn* turn)
{
    // Add the turn to the list of turns, so that it can be saved
    turns.push_back(turn);

    Player* player = getNextPlayer();
    Mosaic* mosaic = player->getMosaic();

    // Get the turn variables from the object
    int factory = turn->factory();
    int row = turn->row();
    char colour = turn->colour();

    // Get the amount of the chosen colour in the selected factory
    int amount = factories[factory]->getAmountOf(colour);

    // Add tiles to mosaic
    mosaic->addTiles(amount, row, new Tile(colour));

    if (factory == 0)
    {
        if (!firstTileTaken)
        {
            mosaic->firstTileTaken();
            factories[factory]->removeSpecific('F');
            playerFirstNextRound = playerTurn;
            firstTileTaken = true;
            // TODO: SET THE PLAYER WHO HAS TAKEN THE F TILE TO GO FIRST NEXT ROUND
        }
        factories[factory]->removeSpecific(colour);
    } else {
        leftoversToCenter(colour, factory);
        factories[factory]->clearFactory();
    }

    // Cycle to the next player
    cyclePlayers();
}

/* Factory */

void GameManager::initializeFactories()
{
    //Setup each factory with 4 tiles, taken from the front of the tilebag

    // Setup the centre factory
    factories.push_back(new Factory(0));
    // Setup factories 1-5
    for (int i = 1; i < NUM_FACTORIES; i++)
    {
        factories.push_back(new Factory(i));
    }
    fillFactories();
}

void GameManager::fillFactories()
{
    factories[0]->addTile(new Tile('F'));
    for (int i = 1; i < (int) factories.size (); i++)
    {
        for (int t = 0; t < MAX_TILES; t++)
        {
            // Check if the tilebag is empty - Milestone 3 boxLid implementation
            if (tileBag->getSize () != 0)
            {
                // If not empty continue as normal and take from the front and add to the factory
                factories[i]->addTile(tileBag->get(0));
                tileBag->removeFront();
            } 
            else {
                std::cout << "TileBag is empty! shuffling.." << std::endl;

                boxLid->addToTileBag(*tileBag);
                boxLid->clear();
                
                tileBag->shuffle();

                factories[i]->addTile(tileBag->get(0));
                tileBag->removeFront();
            }
        }
    }
}

void GameManager::printFactories(std::ostream& stream)
{
    for (std::string::size_type i = 0; i < factories.size(); i++)
    {
        stream << factories[i]->getNumber() << ": ";
        for (std::string::size_type e = 0; e < factories[i]->tiles.size(); e++)
        {
            stream << factories[i]->tiles[e]->getColour() << " ";
        }
        stream << std::endl;
    }
    stream << std::endl;
}

void GameManager::leftoversToCenter(char colour, int factory)
{
    for (std::string::size_type i = 0; i < factories[factory]->tiles.size(); i++)
    {
        if (factories[factory]->tiles[i]->getColour() != colour)
        {
            factories[0]->addTile(new Tile(*factories[factory]->tiles[i]));
        }
    }
}

bool GameManager::allFactoriesEmpty()
{
    bool empty = true;

    for (std::string::size_type i = 0; i < factories.size(); i++)
    {
        if (factories[i]->tiles.size() != 0)
        {
           empty = false;
        }
    }

    return empty;
}

bool GameManager::validFactory(int factory)
{
    bool valid = true;

    if (factory < 0)
    {
        valid = false;
    }
    else if (factory >= NUM_FACTORIES)
    {
        valid = false;
    }

    return valid;
}

bool GameManager::validRow(int row)
{
    bool valid = true;

    if (row < 0)
    {
        valid = false;
    }
    else if (row > NUM_ROWS)
    {
        valid = false;
    }

    return valid;
}

/* Player */

int GameManager::countPlayers()
{
    return (int) this->players.size();
}

void GameManager::setupPlayers()
{
    for (int i = 0; i < playerCount; i++)
    {
        std::string playerName;
        std::cout << "Enter a name for player " << i << std::endl << "> ";
        std::cin >> playerName;
        std::cout << "\n";

        addPlayer(new Player(playerName));
    }
    if (aiEnabled)
    {
        std::cout << "AI Player added \n";
        addPlayer(new Player("AI", true));
    }
}

void GameManager::addPlayer(Player* player)
{
    players.push_back(player);
}

void GameManager::cyclePlayers()
{
    playerTurn++;
    if (playerTurn >= (int)players.size())
    {
        playerTurn = 0;
    }
}

Player* GameManager::getNextPlayer()
{
    return players[playerTurn];
}

std::string GameManager::getWinningPlayerName()
{
    std::string returnString = "Player " + getNextPlayer()->getName() + "wins!\n";
    ;
    for (int i = 0; i < (int)players.size(); i++) //this is used just in case there are multiple users - future implement?
    {
        Player *temp = getNextPlayer();
        cyclePlayers();
        if (temp->getPoints() > getNextPlayer()->getPoints())
        {
            returnString = "Player " + temp->getName() + " wins!\n";
        }
        if (temp->getPoints() == getNextPlayer()->getPoints())
        {
            returnString = "It's a draw!\n";
        }
    }
    //delete temp; //need to delete temp
    return returnString;
}

std::vector<Player*> GameManager::getPlayers()
{
    return this->players;
}

/* Stream / Loadgame related */

std::ostream & operator<< (std::ostream & stream, GameManager& game)
{
    // Start of the settings
    stream << "#settings" << std::endl;
    stream << seed << std::endl;
    stream << aiEnabled << std::endl;

    // Print the starting tiles / tilebag
    // this will change when a seed is given.
    stream << game.tileBag->getStartingTiles() << std::endl;

    for (int i = 0; i < (int)game.players.size(); i++)
    {
        stream << game.players[i]->getName() << std::endl;
    }

    for (int i = 0; i < (int)game.turns.size(); i++)
    {
        stream << "turn " << game.turns[i] << std::endl;
    }
    return stream;
}
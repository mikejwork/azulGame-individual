#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "TileBag.h"
#include "Factory.h"
#include "Turn.h"
#include "Player.h"
// BoxLid
#include "BoxLid.h"

#include <vector>

class GameManager
{
public:
    GameManager();
    GameManager(std::string startingTiles);

    /* Turns */
    void turn(Turn* turn);
    void endRound();
    void updateMosaics();

    /* Factory */
    void initializeFactories();
    void fillFactories();
    void printFactories(std::ostream& stream);
    void leftoversToCenter(char colour, int factory);
    bool allFactoriesEmpty();
    bool validFactory(int factory);
    bool validRow(int row);
    std::vector<Factory*> factories;

    /* Player */
    int countPlayers();
    void setupPlayers();
    void addPlayer(Player* player);
    Player* getNextPlayer();
    void cyclePlayers();
    std::string getWinningPlayerName();
    std::vector<Player*> getPlayers();

    /* Convert game into stream for loadgame */
    friend std::ostream &operator<<(std::ostream &stream, GameManager &game);

    BoxLid* boxLid;
    TileBag *tileBag;

    int roundsPlayed = 0;
    bool firstTileTaken;
private:
    /* Player related */
    std::vector<Player*> players;
    int playerTurn;
    int playerFirstNextRound;

    /* Other */
    std::vector<Turn*> turns;
};

#endif // GAME_MANAGER_H
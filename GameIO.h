#ifndef GAME_IO_H
#define GAME_IO_H

#include <iostream>
#include <fstream>
#include <string>

#include "GameManager.h"

class GameIO {
public:
    GameIO(GameManager* gameManager, std::istream* in, std::ostream* out);

    void executeRound();
    void executeCommand();
    bool executeTurn();
    bool executeLoadingTurn();
    Turn* newTurn();

    void printFactories();
    void printMosaic(Player* player);
    void printScore(Player* player);
    void printTurn();
    void printFinalScores();
    void printGameState();

    void loadPlayers();

    GameManager* loadGame();
    void saveGame();

private:
    GameManager* gameManager;
    std::istream* in;
    std::ostream* out;
    static std::ostream* nullOutput;
};

#endif // GAME_IO_H
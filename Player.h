#ifndef PLAYER_H
#define PLAYER_H

#include "Mosaic.h"
#include "AiBrain.h"

#include <string>

class Player
{
public:
    Player(std::string name);
    Player(std::string name, bool isAi);
    ~Player();

    std::string getName();
    Mosaic* getMosaic();
    int getPoints();
    void addPoints(int points);
    bool isAI();

    AiBrain* aiBrain;
    
private:
    std::string name;
    Mosaic* mosaic;
    bool isAi;
    int points;
};

#endif // PLAYER_H
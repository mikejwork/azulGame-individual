#include "Player.h"

Player::Player(std::string name) :
    name(name),
    isAi(false)
{
    this->mosaic = new Mosaic();
}

Player::Player(std::string name, bool isAi) :
    name(name),
    isAi(isAi)
{
    this->mosaic = new Mosaic();
    this->aiBrain = new AiBrain();
}

Player::~Player()
{
    delete this->mosaic;
}

int Player::getPoints()
{
    return this->points;
}

bool Player::isAI()
{
    return this->isAi;
}

void Player::addPoints(int points)
{
    this->points += points;
}

std::string Player::getName()
{
    return this->name;
}

Mosaic *Player::getMosaic()
{
    return this->mosaic;
}
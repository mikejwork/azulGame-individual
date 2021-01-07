#include "Factory.h"

Factory::Factory(int factoryNumber) :
    factoryNumber(factoryNumber) {}

Factory::~Factory() {}

int Factory::getNumber()
{
    return this->factoryNumber;
}

void Factory::addTile(Tile* tile)
{
    tiles.push_back(new Tile(*tile));
    empty = false;
}

void Factory::clearFactory()
{
    tiles.clear();
    empty = true;
}

bool Factory::isEmpty()
{
    return this->empty;
}

int Factory::getAmountOf(char colour)
{
    int count = 0;
    for (std::string::size_type i = 0; i < tiles.size(); i++)
    {
        if (tiles[i]->getColour() == colour) {
            count++;
        }
    }
    return count;
}

void Factory::removeSpecific(char colour)
{
    std::vector<Tile*> tiles_cpy;
    for (std::string::size_type i = 0; i < tiles.size(); i++)
    {
        if (tiles[i]->getColour() != colour) {
            tiles_cpy.push_back(tiles[i]);
        }
    }
    tiles = tiles_cpy;
}
#include "Row.h"
#include "Settings.h"

Row::Row()
{
    size = 0;
}

Row::~Row()
{}

bool Row::canPlace(char colour)
{
    bool canPlace = false;
    if (this->empty)
    {
        canPlace = true;
    }
    else if (this->getIndex(0)->getColour() == colour)
    {
        canPlace = true;
    }

    return canPlace;
}

Tile* Row::getIndex(int index)
{
    return tiles.at(index);
}

int Row::getSize()
{
    return this->size;
}

void Row::add(Tile* tile)
{
   tiles.push_back(new Tile(*tile));
   size++;
   empty = false;
}

void Row::clearRow() {
    tiles.clear();
    empty = true;
    size = 0;
}

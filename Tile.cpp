#include "Tile.h"

Tile::Tile(char colour) :
    colour(colour)
{}

Tile::Tile(const Tile& other) :
    colour(other.colour)
{}

Tile::~Tile()
{}

char Tile::getColour()
{
    return this->colour;
}

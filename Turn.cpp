// This class represents the input information for a turn.
// It's meant for data storage, not to have extra functionality added.

#include "Turn.h"

Turn::Turn (
    int factory,
    int row,
    char colour)
{
    this->_factory = factory;
    this->_row = row;
    this->_colour = colour;
}

int Turn::factory ()
{
    return this->_factory;
}

int Turn::row ()
{
    return this->_row;
}

char Turn::colour()
{
    return this->_colour;
}

std::ostream & operator<< (std::ostream & stream, Turn * turn)
{
    stream << turn->_factory << " ";
    stream << turn->_colour << " ";
    stream << turn->_row;

    return stream;
}

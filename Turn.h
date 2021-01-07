#ifndef TURN_H
#define TURN_H

#include <string>
#include <iostream>

// This class represents the input information for a turn.
// It's meant for data storage, not to have extra functionality added.

class Turn
{
public:
    Turn (int factory, int row, char colour);

    int factory();
    int row ();
    char colour();

    friend std::ostream & operator<< (std::ostream & stream, Turn * turn);

private:
    int _factory;
    int _row;
    char _colour;
};

#endif

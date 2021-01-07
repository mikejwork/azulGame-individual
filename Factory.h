#ifndef FACTORY_H
#define FACTORY_H

#include <vector>
#include <string>

#include "Tile.h"

class Factory {
public:
    Factory(int factoryNumber);
    ~Factory();

    int getNumber();

    void addTile(Tile* tile);
    void removeSpecific(char colour);
    void clearFactory();
    bool isEmpty();
    int  getAmountOf(char c);

    std::vector<Tile*> tiles;
private:
    int factoryNumber;
    bool empty = false;
};

#endif // FACTORY_H
#ifndef ROW_H
#define ROW_H

#include <vector>

#include "Tile.h"

class Row
{
public:
    Row();
    ~Row();
    bool empty = true;

    int getSize();
    std::vector<Tile*> tiles;
    Tile* getIndex(int index);
    bool canPlace(char colour);

    void add(Tile* tile);
    void clearRow();

private:
    int size;
};

#endif //ROW_H

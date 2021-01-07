#ifndef BOXLIDNODE_H
#define BOXLIDNODE_H

#include <memory>
#include "Tile.h"

class BoxLidNode
{
public:
    BoxLidNode(Tile* value);
    BoxLidNode(const BoxLidNode &other); 
    ~BoxLidNode();
    Tile* tile;
    std::shared_ptr<BoxLidNode> left;
    std::shared_ptr<BoxLidNode> right;
private:
};

#endif
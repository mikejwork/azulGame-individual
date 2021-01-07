#ifndef BST_BL_H
#define BST_BL_H

#include "BoxLidNode.h"
#include "Tile.h"
#include "TileBag.h"
#include <memory>

class BoxLid 
{
public:

    BoxLid();
    ~BoxLid();

    // Clears the box lid
    void clear();
    // Adding new tile to the box lid
    void addTile(Tile* tile);

    // Returns the size of the box lid
    int getSize();

    void addToTileBag(TileBag &tileBag);
    void dfs() const;
private:
    std::shared_ptr<BoxLidNode> Root;
    std::shared_ptr<BoxLidNode> addTile(std::shared_ptr<BoxLidNode> node, Tile* tile);
    void addToTileBag(std::shared_ptr<BoxLidNode> node, TileBag &tileBag);
    void dfs(std::shared_ptr<BoxLidNode> node) const;
    int size;
};

#endif
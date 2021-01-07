#include "BoxLid.h"

#include <iostream>
#include <string>

BoxLid::BoxLid()
{
    Root = nullptr;
    size = 0;
}
BoxLid::~BoxLid()
{

}

/* Clears the box lid by setting the root to nullptr, this will automatically delete all nodes below */
void BoxLid::clear()
{
    Root = nullptr;
    size = 0;
}

// Add a tile to the boxLid
void BoxLid::addTile(Tile* tile)
{
    Root = addTile(Root, tile);
    size++;
}

std::shared_ptr<BoxLidNode> BoxLid::addTile(std::shared_ptr<BoxLidNode> node, Tile* tile)
{
    /* Node to return */
    std::shared_ptr<BoxLidNode> returnNode;

    if (node == nullptr)
    {
        returnNode = std::make_shared<BoxLidNode>(tile);
    }
    else
    {
        if (tile->getColour() < node->tile->getColour())
        {
            std::shared_ptr<BoxLidNode> subTree = addTile(node->left, tile);
            node->left = subTree;
            returnNode = node;
        }
        else 
        {
            std::shared_ptr<BoxLidNode> subTree = addTile(node->right, tile);
            node->right = subTree;
            returnNode = node;
        }
    }
    return returnNode;
}

// Returns an ordered string of all the tiles in the box lid. This is used when the tilebag is empty, it will return this string and then re-shuffle the bag
void BoxLid::addToTileBag(TileBag &tileBag)
{
    addToTileBag(Root, tileBag);
}

void BoxLid::addToTileBag(std::shared_ptr<BoxLidNode> node, TileBag &tileBag)
{
    if (node != nullptr)
    {
        addToTileBag(node->left, tileBag);

        tileBag.addBack(new Tile(*node->tile));

        addToTileBag(node->right, tileBag);
    }
}

void BoxLid::dfs() const
{
    dfs(Root);
}

void BoxLid::dfs(std::shared_ptr<BoxLidNode> node) const
{
    //go left
    if (node != nullptr)
    {
        dfs(node->left);

        //do me
        std::cout << node->tile->getColour();

        //go right
        dfs(node->right);
    }
}

// Returns the size of the box lid
int BoxLid::getSize()
{
    return this->size;
}


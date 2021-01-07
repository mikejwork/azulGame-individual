#include "BoxLidNode.h"

BoxLidNode::BoxLidNode(Tile* tile) :
    tile(tile),
    left(nullptr),
    right(nullptr)
{}

BoxLidNode::BoxLidNode(const BoxLidNode& other) : //Dont use
    tile(other.tile),
    left(other.left),
    right(other.right)
{}

BoxLidNode::~BoxLidNode() {
    left = nullptr;
    right = nullptr;
}
#ifndef TILEBAG_H
#define TILEBAG_H

#include <iostream>
#include <string>

#include "Tile.h"
#include "Settings.h"

class Node {
public:
   Node(Tile* tile, Node* next);

   Tile* tile;
   Node* next;
};

class TileBag {
public:
   TileBag();
   TileBag(std::string startingTiles);
   TileBag(int seed);
   ~TileBag();

   int getSize();
   Tile* get(int index);
   void addBack(Tile* tile);
   void removeFront();
   void shuffle();

   std::string getStartingTiles();

   std::string toString();

private:
   int size;
   Node* head;
   std::string startingTiles;

   void loadTiles(std::string startingTiles);
};

#endif // TILEBAG_H

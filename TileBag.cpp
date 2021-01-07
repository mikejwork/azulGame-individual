#include "TileBag.h"
#include <algorithm>
#include <random>

#define DEFAULT_TILES "YBBRRRLBULULRYBLBBLYRRUYUYUYUBBRLUBLRYLBLBUBUBRYRLLLRRUBYYBYYLRLRRYBURRLUYYYBUBLRUUBYLLBYYUULURYRUUL"

Node::Node(Tile* tile, Node* next) :
   tile(tile),
   next(next)
{}

TileBag::TileBag() {
   size = 0;
   if (!seedRandom)
   {
      this->startingTiles = DEFAULT_TILES;
   }
   else
   {
      std::string tileBag = DEFAULT_TILES;
      std::shuffle(tileBag.begin(), tileBag.end(), std::default_random_engine(seed));
      this->startingTiles = tileBag;
   }
   loadTiles (startingTiles);
}

TileBag::TileBag(std::string startingTiles) {
   size = 0;
   this->startingTiles = startingTiles;
   loadTiles (startingTiles);
}

TileBag::TileBag(int seed) {
   size = 0;
   std::string tileBag = DEFAULT_TILES;
   std::shuffle(tileBag.begin(), tileBag.end(), std::default_random_engine(seed));
   this->startingTiles = tileBag;
   loadTiles (tileBag);
}

TileBag::~TileBag()
{
   // TODO - memory leak here
}

void TileBag::loadTiles(std::string tiles)
{
    for (std::string::size_type i = 0; i < tiles.size(); i++) {
        Tile* new_tile = new Tile(tiles[i]);
        addBack(new_tile);
    }
}

int TileBag::getSize()
{
   return this->size;
}

Tile* TileBag::get(int index)
{
   int count = 0;
   Node* current = head;
   Tile* tile = nullptr;
   if (index >= 0 && index < size)
   {
       while (count < index) {
            ++count;
            current = current->next;
       }
       tile = new Tile(*current->tile);
   }
   return tile;
}

void TileBag::addBack(Tile* tile) {
   Node* toAdd = new Node(tile, nullptr);

   if (head == nullptr)
   {
      head = toAdd;
   }
   else
   {
      Node* current = head;
      while (current->next != nullptr) {
         current = current->next;
      }
      current->next = toAdd;
   }
   size++;
}

void TileBag::removeFront() {
   if (head != nullptr) {
      Node* temp = head;
      head = head->next;
      delete temp;
   }
   size--;
}

std::string TileBag::getStartingTiles ()
{
    return this->startingTiles;
}

// Used to shuffle the tileBag - Milestone 3 Boxlid implementation
void TileBag::shuffle()
{
   std::string currentTileBag  = TileBag::toString();
   // here we can use our saved seed which is a global variable, we can assume this is not
   // zero as the shuffle method is only called when the seedRandom setting is enabled
   std::shuffle(currentTileBag.begin(), currentTileBag.end(), std::default_random_engine(seed));

   // Make sure the tileBag is fully cleared
   this->head = nullptr;
   // Loop through our new shuffled string of tiles and populate the tilebag
   for (std::string::size_type i = 0; i < currentTileBag.size(); i++)
   {
      this->addBack(new Tile(currentTileBag[i]));
   }
}

std::string TileBag::toString()
{
   std::string str = "";

   Node* current = head;
   while (current != nullptr)
   {
      str += current->tile->getColour();
      current = current->next;
   }
   str += '\0'; // set null terminator char
   return str;
}

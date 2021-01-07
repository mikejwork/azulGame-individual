#ifndef MOSAIC_H
#define MOSAIC_H

#define NUM_ROWS 5
#define NUM_COLS 5
#define MIN_ROWS 0

#define BACK 1
#define TOP 2
#define LEFT 3
#define RIGHT 4

#define MOSAIC_WIDTH 5
#define MOSAIC_HEIGHT 5

#include "Tile.h"
#include "TileBag.h"
#include "Row.h"
#include "BoxLid.h"

#include <vector>
#include <iostream>
#include <string>

// TODO make members private
class Mosaic
{
public:
    Mosaic();
    ~Mosaic();

    friend std::ostream &operator<<(
        std::ostream &stream,
        Mosaic &mosaic);


    bool checkPlace(char colour, int row);


    void addTiles(int amount, int row, Tile *tile);
    void updatePoints(int row_num, int i);
    bool checkLine(char colour, int row);
    void returnBroken(TileBag *tilebag);
    void brokenToBoxLid(BoxLid &boxLid);
    void addToBoxLid(BoxLid &boxLid, char colour, int amount);
    void addToTileBag(TileBag &tileBag, char colour, int amount);
    int getPlayerPoints();
    void firstTileTaken();
    void updateMosaic(BoxLid &boxLid, TileBag &tileBag);
    bool isRowFull(int row);
    int brokenTilesScore();

    std::vector<Row*> getRows();


private:
    Tile *mosaic[MOSAIC_HEIGHT][MOSAIC_WIDTH];
    std::vector<Row *> rows;
    std::vector<Tile *> broken;

    char mask[MOSAIC_HEIGHT][MOSAIC_WIDTH] = {{'B', 'Y', 'R', 'U', 'L'},
                                              {'L', 'B', 'Y', 'R', 'U'},
                                              {'U', 'L', 'B', 'Y', 'R'},
                                              {'R', 'U', 'L', 'B', 'Y'},
                                              {'Y', 'R', 'U', 'L', 'B'}};
    bool returnCheck(int row_num, int i, int a);
    int count(int row_num, int i);
    int playerPoints;
};

#endif // MOSAIC_H

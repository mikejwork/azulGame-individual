#include "Mosaic.h"

Mosaic::Mosaic()
{
    // add the five rows to the Mosaic
    for (int i = 0; i < 5; i++)
    {
        rows.push_back(new Row());
    }
    playerPoints = 0;
}

Mosaic::~Mosaic()
{
}

// This is basically a toString for the mosaics, it may look complicated but its beautiful :)
// Prints the rows and mosaics and accounts for blank space nicely.
std::ostream &operator<<(std::ostream &stream, Mosaic &m)
{
    int row_num = 1;
    int blank_space = 4;

    std::vector<Row *> &rows = m.rows;
    std::vector<Tile *> &broken = m.broken;
    Tile *(&Mosaic)[MOSAIC_HEIGHT][MOSAIC_WIDTH] = m.mosaic;

    for (int row = 1; row < 6; row++)
    {
        stream << row << ":";
        for (int spacing = 0; spacing < blank_space; spacing++)
        {
            stream << "  ";
        }

        int empty = row_num - rows[row_num - 1]->getSize();
        for (int index = 0; index < empty; index++)
        {
            stream << " .";
        }

        for (int index = 0; index < rows[row_num - 1]->getSize(); index++)
        {
            char colour = rows[row_num - 1]->getIndex(index)->getColour();
            stream << " " << colour;
        }
        blank_space--;
        row_num++;

        stream << " ||";
        for (int i = 0; i < 5; i++)
        {
            if (Mosaic[row - 1][i] == nullptr)
            {
                stream << " .";
            }
            else
            {
                char colour = Mosaic[row - 1][i]->getColour();
                stream << " " << colour;
            }
        }

        stream << std::endl;
    }

    stream << "broken: ";

    for (std::string::size_type i = 0; i < broken.size(); i++)
    {
        stream << broken[i]->getColour() << " ";
    }

    stream << std::endl;

    return stream;
}

void Mosaic::updateMosaic(BoxLid &boxLid, TileBag &tileBag)
{
    int row_num = 1;
    for (int row = 0; row < NUM_ROWS; row++)
    {
        int size = row_num - rows[row_num - 1]->getSize(); //size will return the value of how many empty spaces are in the row
        if (size == 0)
        {
            for (int col = 0; col < NUM_COLS; col++)
            {
                if (rows[row_num - 1]->getIndex(row)->getColour() == mask[row][col])
                {
                    mosaic[row_num - 1][col] = new Tile(rows[row_num - 1]->getIndex(row)->getColour());
                    playerPoints += count(row_num - 1, col);
                }
            }
            // If seedRandom or "-s" was enabled by the user
            if (seedRandom)
            {
                // If the size of the row is greater than 0
                // we move the other tiles back to the boxLid
                if (rows[row_num - 1]->getSize() > 0)
                {
                    // Get the colour of the tiles in the row and add them to the boxlid
                    char colour = rows[row_num - 1]->getIndex(0)->getColour();
                    addToBoxLid(boxLid, colour, rows[row_num - 1]->getSize() - 1);
                }
            } else
            {
                // Explained above, works the same but adds to tilebag instead
                if (rows[row_num - 1]->getSize() > 0)
                {
                    char colour = rows[row_num - 1]->getIndex(0)->getColour();
                    addToTileBag(tileBag, colour, rows[row_num - 1]->getSize() - 1);
                }
            }
            // full clear the row to prepare for next round
            rows[row_num - 1]->clearRow();
        }
        // Change to next row, goes through from top to bottom just like the rules.
        row_num++;
    }
    // Calculate the broken tiles score and minus it from the players points
    playerPoints -= brokenTilesScore();
}

int Mosaic::getPlayerPoints()
{
    int temp = this->playerPoints;
    this->playerPoints = 0;
    return temp;
}

int Mosaic::count(int row_num, int col)
{
    int b = row_num;
    int f = row_num;
    int l = col;
    int r = col;

    int countRight = 0;
    int countTop = 0;
    int countLeft = 0;
    int countBot = 0;
    int result = 1;
    bool once;

    once = true;
    while (!returnCheck(row_num, r, RIGHT))
    {
        if (once == true)
        {
            countRight = 1;
            once = false;
        }
        countRight++;
        r++;
    }
    once = true;

    while (!returnCheck(row_num, l, LEFT))
    {
        if (once == true)
        {
            countLeft = 1;
            once = false;
        }
        countLeft++;
        l--;
    }
    once = true;

    while (!returnCheck(b, col, BACK))
    {
        if (once == true)
        {
            countBot = 1;
            once = false;
        }
        countBot++;
        b++;
    }
    once = true;

    while (!returnCheck(f, col, TOP))
    {
        if (once == true)
        {
            countTop = 1;
            once = false;
        }
        countTop++;
        f--;
    }
    if (!(countRight + countLeft + countTop + countBot) == 0)
    {
        result = countRight + countLeft + countTop + countBot;
    }

    return result;
}

bool Mosaic::returnCheck(int row_num, int i, int a)
{
    bool ans = true;
    if (a == 1)
    {
        //CHECKS DOWN
        if (row_num + 1 < 5) //This prevents checking outside of array
        {
            if (mosaic[row_num + 1][i] != nullptr)
            {
                ans = false;
            }
        }
    }
    else if (a == 2)
    {
        //CHECKS UP
        if (row_num - 1 >= 0)
        {
            if (mosaic[row_num - 1][i] != nullptr)
            {

                ans = false;
            }
        }
    }
    else if (a == 3)
    {
        //CHECKS LEFT
        if (i - 1 >= 0)
        {
            if (mosaic[row_num][i - 1] != nullptr)
            {
                ans = false;
            }
        }
    }
    else if (a == 4)
    {
        if (i + 1 <= 5)
        {
            //CHECKS RIGHT
            if (mosaic[row_num][i + 1] != nullptr)
            {
                ans = false;
            }
        }
    }
    return ans;
}

void Mosaic::addTiles(int amount, int row, Tile *tile)
{
    int tiles_filled = rows[row - 1]->getSize();
    int spaces_left = row - tiles_filled;
    int leftover = 0;

    if (checkPlace(tile->getColour(), row) == true)
    {
        for (int i = 0; i < amount; i++)
        {
            broken.push_back(new Tile(*tile));
        }
    } //END OF ADD

    else if (amount > spaces_left)
    {
        leftover = amount - spaces_left;
        for (int i = 0; i < spaces_left; i++)
        {
            rows[row - 1]->add(new Tile(*tile));
        }
        for (int i = 0; i < leftover; i++)
        {
            broken.push_back(new Tile(*tile));
        }
    }
    else
    {
        for (int i = 0; i < amount; i++)
        {
            rows[row - 1]->add(new Tile(*tile));
        }
    }
}

void Mosaic::firstTileTaken()
{
    broken.push_back(new Tile('F'));
}

bool Mosaic::checkPlace(char colour, int row) //returns true if: Mosaic has that colour already, or the tile already has other colours, and the tile cannot be placed.
{
    bool result = false;
    int count = 0;
    for (int i = 1; i <= NUM_ROWS; i++)
    {
        if (checkLine(colour, i) || isRowFull(i))
        {
            count++;
        }
    }
    if (count == NUM_ROWS)
    {
        result = true;
    }
    return result;
}

bool Mosaic::checkLine(char colour, int row) //THIS IS USED SEE IF THERE IS A TILE ALREADY IN A Mosaic - CALLED IN GAMEIO
{
    bool result = false;
    for (int col = 0; col < NUM_COLS; col++) //loops through the amount of COLS
    {
        if (mosaic[row - 1][col] != nullptr) //Safety check to prevent from crashing - could remove
        {
            if (mosaic[row - 1][col]->getColour() == colour) //Checks if that col is equal to the colour being placed.
            {
                result = true;
            }
        }
    }
    if (rows[row - 1]->getSize() != 0) //Used to check if the colour is already in the row
    {
        if (colour != rows[row - 1]->getIndex(0)->getColour())
        {
            result = true;
        }
    }
    return result;
}

void Mosaic::returnBroken(TileBag *tilebag)
{
    for (std::string::size_type i = 0; i < broken.size(); i++)
    {
        if (broken[i]->getColour() != 'F')
        {
            tilebag->addBack(new Tile(*broken[i]));
        }
    }
    broken.clear();
}

void Mosaic::brokenToBoxLid(BoxLid &boxLid)
{
    for (std::string::size_type i = 0; i < broken.size(); i++)
    {
        if (broken[i]->getColour() != 'F')
        {
            boxLid.addTile(new Tile(*broken[i]));
        }
    }
    broken.clear();
}

// Used to add specific tiles to the box lid, used at the end of round to move leftover tiles from the rows to the boxLid
void Mosaic::addToBoxLid(BoxLid &boxLid, char colour, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        boxLid.addTile(new Tile(colour));
    }
}

// Used to add specific tiles to the tileBag, used at the end of round to move leftover tiles from the rows to the tileBag - Alternative to addToBoxLid, if seeds are disabled.
void Mosaic::addToTileBag(TileBag &tileBag, char colour, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        tileBag.addBack(new Tile(colour));
    }
}

bool Mosaic::isRowFull(int row)
{
    if (rows[row - 1]->getSize() == row)
    {   
        return true;
    }
    return false;
}

int Mosaic::brokenTilesScore() {
    int count = 0;
    for (std::string::size_type i = 0; i < broken.size(); i++) {
        if (i >= 0 && i < 2) { //There are a total of 7 spots on the broken board, each part adds certain amount
            count += 1;
        }
        if (i >= 2 && i < 5) {
            count += 2;
        }
        if (i >= 5 && i < 7) {
            count += 3;
        }
    }
    return count;

}

std::vector<Row*> Mosaic::getRows()
{
    return this->rows;
}
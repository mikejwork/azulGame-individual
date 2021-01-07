#include "AiBrain.h"

AiBrain::AiBrain()
{
}

AiBrain::~AiBrain() {}

Turn *AiBrain::processAiTurn(Mosaic *mosaic, std::vector<Factory *> factories)
{
    this->mosaic = mosaic;
    std::vector<Row *> rows = mosaic->getRows();

    // Will store all possible turns
    std::vector<Turn *> possibleTurns;
    // Will store the turns with the highest amount of tiles / turns that will fill a row
    std::vector<Turn *> preferrableTurns;
    // Will store the turns with the less preferrable outcome, such as taking the F tile first
    std::vector<Turn *> nonPreferrableTurns;

    // Loop through each row
    for (std::string::size_type row = 0; row < rows.size(); row++)
    {
        // Get the empty space in the current row
        int emptySpace = (row + 1) - rows[row]->getSize();
        // If the row has space
        if (emptySpace > 0)
        {
            // Will loop through every factory
            for (std::string::size_type factory = 0; factory < factories.size(); factory++)
            {
                // Check if the factory is empty
                if (!factories[factory]->isEmpty())
                {
                    // Will get the colours avaliable to that row in specific, this will filter 
                    // out the turns that arent possible because there is already another colour in that row
                    std::string colours = coloursAvaliable(row);
                    // Loop through the avaliable colours
                    for (std::string::size_type i = 0; i < colours.size(); i++)
                    {
                        // Check if the colour is equal to zero, not sure why but sometimes it would crash due to passing through 0 instead of a char
                        if (colours[i] != 0)
                        {
                            // Check if the factory actually contains any of the colour
                            if (factories[factory]->getAmountOf(colours[i]) != 0)
                            {
                                // Check if the colour can be placed again, this stopped a specific crash, kinda bad that i have to call it twice but eh
                                if (rows[row]->canPlace(colours[i]))
                                {
                                    // Will check if the colour is already in the mozaic, if it is the turn will not be added
                                    if(!mosaic->checkLine(colours[i], row + 1))
                                    {
                                        // If the factory contains the 'F' tile, we move it to the "nonPreferrableTurns" vector, which we will
                                        // use if no other suitable turn is found. This is because the 'F' tile is a penalty and we want to minimise it
                                        if (factories[factory]->getAmountOf('F') > 0)
                                        {
                                            nonPreferrableTurns.push_back(new Turn(factory, row, colours[i]));
                                        }
                                        // Else if the turn will fill a row, add to the "preferrableTurns" vector, because we want to fill as many rows as possible
                                        else if (factories[factory]->getAmountOf(colours[i]) == emptySpace)
                                        {
                                            preferrableTurns.push_back(new Turn(factory, row, colours[i]));
                                        }
                                        // Else if the turn will fill a row but also have leftovers, add to "nonPreferrableTurns" vector as this will penalise us
                                        else if(factories[factory]->getAmountOf(colours[i]) > emptySpace)
                                        {
                                            nonPreferrableTurns.push_back(new Turn(factory, row, colours[i]));
                                        }
                                        // Else if the turn has an amount of tiles lower than the empty space, we add to a neutral vector "possibleTurns",
                                        // which will be used if no preferrableTurns are found
                                        else if (factories[factory]->getAmountOf(colours[i]) < emptySpace)
                                        {
                                            possibleTurns.push_back(new Turn(factory, row, colours[i]));
                                        }

                                        // TURN HIERARCHY
                                        /* this is the order in which the AI will check each vector
                                            <preferrableTurns>
                                                    \-----<possibleTurns>
                                                                   \-----<nonPreferrableTurns>
                                            
                                            Then by the end, if "bestTurn" is still nullptr, this is the case where no tile can be placed and must go straight to the broken tiles
                                            The AI will construct a "dummy" turn which will move the remaining tiles to a row, which will in turn let the game handle this normally,
                                            like it would for a player.
                                        */
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Turn* bestTurn = nullptr;
    // Check that there are some preferrable turns, if not we will go to the possible turns, then if that is empty we go to the nonpreferrableturns
    if (preferrableTurns.size() != 0)
    {
        int highestAmnt = 0;
        for (std::string::size_type turn = 0; turn < preferrableTurns.size(); turn++)
        {
            int factory = preferrableTurns[turn]->factory();
            char colour = preferrableTurns[turn]->colour();

            if (factories[factory]->getAmountOf(colour) > highestAmnt)
            {
                bestTurn = preferrableTurns[turn];
                highestAmnt = factories[factory]->getAmountOf(colour);
            }
        }
    }
    else
    {
        if (possibleTurns.size() != 0)
        {
            int highestNum = 0;

            for (std::string::size_type turn = 0; turn < possibleTurns.size(); turn++)
            {
                int factory = possibleTurns[turn]->factory();
                char colour = possibleTurns[turn]->colour();
                int amount = factories[factory]->getAmountOf(colour);
                if (amount > highestNum)
                {
                    bestTurn = possibleTurns[turn];
                    highestNum = amount;
                }
            }
        }
        else
        {
            if (nonPreferrableTurns.size() != 0)
            {
                // This int will hold the lowest number of tiles that will overflow into our broken
                // We want the smallest number because it is the least amount of points lost
                int lowestImpact = 999;
                bool bestFound = false;

                for (std::string::size_type turn = 0; turn < nonPreferrableTurns.size() && !bestFound; turn++)
                {
                    int row = nonPreferrableTurns[turn]->row();
                    int factory = nonPreferrableTurns[turn]->factory();
                    char colour = nonPreferrableTurns[turn]->colour();

                    int emptySpace = (row + 1) - rows[row]->getSize();

                    if (factories[factory]->getAmountOf(colour) == emptySpace)
                    {
                        bestTurn = nonPreferrableTurns[turn];
                        bestFound = true;
                    }
                    else if (factories[factory]->getAmountOf(colour) > emptySpace)
                    {
                        int amnt = factories[factory]->getAmountOf(colour) - emptySpace;
                        bestTurn = nonPreferrableTurns[turn];
                        if (amnt < lowestImpact)
                        {
                            lowestImpact = amnt;
                        }
                    }
                }
            }
        }
    }
    
    
    if (bestTurn == nullptr)
    {
        // If the turn is nullptr then no turn can be made, so we make the AI take any turn, so that our game automatically moves to broken
        bestTurn = allElseFails(factories);
    }

    // We remake the turn because the turn object actually uses 1 - 5 instead of 0 - 4
    bestTurn = new Turn(bestTurn->factory(), bestTurn->row() + 1, bestTurn->colour());
    // Output, will display the turn that the AI has chosen
    std::cout << "[AI] turn " << bestTurn->factory() << " " << bestTurn->colour() << " " << bestTurn->row() << "\n\n";

    /* Cleanup */
    possibleTurns.clear();
    preferrableTurns.clear();
    nonPreferrableTurns.clear();
    rows.clear();

    return bestTurn;
}

std::string AiBrain::coloursAvaliable(int row)
{
    std::vector<Row *> rows = mosaic->getRows();

    std::string retColours;
    std::string colours = "LURYB";
    for (std::string::size_type i = 0; i < colours.size(); i++)
    {
        if (rows[row]->canPlace(colours[i]))
        {
            retColours = retColours + colours[i];
        }
    }
    retColours = retColours + '\0';
    return retColours;
}

Turn* AiBrain::allElseFails(std::vector<Factory*> factories)
{
    Turn* turn = nullptr;
    for (std::string::size_type factory = 0; factory < factories.size(); factory++)
    {
        if (!factories[factory]->isEmpty())
        {
            std::string colours = "LURYB";
            for (std::string::size_type i = 0; i < colours.size(); i++)
            {
                // Safety check, sometimes the colour would be zero? not sure why
                if (colours[i] != 0)
                {
                    if (factories[factory]->getAmountOf(colours[i]) != 0)
                    {
                        turn = new Turn(factory, 1, colours[i]);
                    }
                }
            }   
        }
    }

    return turn;
}
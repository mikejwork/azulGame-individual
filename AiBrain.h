#ifndef AI_BRAIN_H
#define AI_BRAIN_H

#include "Mosaic.h"
#include "Factory.h"
#include "Turn.h"

class AiBrain
{
public:
    AiBrain();
    ~AiBrain();
    
    Turn* processAiTurn(Mosaic* mosaic, std::vector<Factory*> factories);
    Turn* allElseFails(std::vector<Factory*> factories);
    std::string coloursAvaliable(int row);
    Mosaic* mosaic;

private:

};

#endif
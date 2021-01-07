#ifndef MENU_H
#define MENU_H
#include <iostream>

#include "GameManager.h"
#include "GameIO.h"

#define MAX_ROUNDS 5;

class Menu {
public:
    Menu();

    /* Game Functionality */
    void startGame();
    bool loadGame();


    void displayMenu();
    void handleInput(int input);
private:
    GameManager* gameManager;
};

#endif // MENU_H
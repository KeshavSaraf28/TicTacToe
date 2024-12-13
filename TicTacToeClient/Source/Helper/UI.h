#ifndef UI_h
#define UI_h

#include <stdio.h>
#include <string>
#include "axmol.h"
#include "CommonHelper.h"
#include <string>
#include <iostream>
#include "constants.h"

USING_NS_AX;

class UI
{
public:
    UI(Layer *layer, int &gameState, std::string gameType);
    ~UI();

    void ShowGameOver(Layer *Layer, int gameType, std::string gameMode, int result);
    void GoToMainMenu(Object *pSender);
    void GoToGameScene(Object *pSender);
    void GoToMultiplayerGameScene(Object *pSender);
    void GoToOnlineGameScene(Object *pSender);
    void PauseGame(Object *pSender);
    int *gameState;
    Menu *pauseMenu;
    Sprite *pausebackground;
};

#endif /* UI_h */


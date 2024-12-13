#pragma once
#include "axmol.h"
#include "Helper/UI.h"
#include "Helper/CommonHelper.h"
#include "Helper/constants.h"
#include <iostream>

USING_NS_AX;

class GameSceneMultiplayer : public ax::Layer
{
public:
    static ax::Scene *createScene();

    virtual bool init();

    CREATE_FUNC(GameSceneMultiplayer);

protected:
    bool onTouchBegan(ax::Touch *touch, ax::Event *event);

    void InitGridRects();
    void InitGridPieces();

    void CheckAndPlacePiece(ax::Touch *touch);
    void CheckWin(int x, int y);
    void Check3PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int &result);
    ax::Rect GetRectSpaceFromTag(int tag);

    ax::Sprite *gridSprite;
    ax::Rect gridSpaces[3][3];
    ax::Sprite *gridPieces[3][3];

    int gridArray[3][3];

    int turn;
    int gameState;

    UI *ui;
};

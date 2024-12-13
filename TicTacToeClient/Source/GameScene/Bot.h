#pragma once

#include "axmol.h"
#include <vector>
#include <array>
#include "Helper/constants.h"
#include <iostream>
using namespace std;
USING_NS_AX;

class Bot
{
public:
    Bot(int playerPiece);
    void PlacePiece(int (*gridArray)[3][3], Sprite *gridPieces[3][3], int *gameState);

private:
    void CheckSection(int x1, int y1, int x2, int y2, int X, int Y, int pieceToCheck, int (*gridArray)[3][3], Sprite *gridPieces[3][3]);
    void CheckIfPieceIsEmpty(int X, int Y, int (*gridArray)[3][3], Sprite *gridPieces[3][3]);

    int botPiece;
    int playerPiece;

    vector<array<int, 6>> checkMatchVector;
};

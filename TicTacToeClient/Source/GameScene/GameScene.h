#include "axmol.h"
#include "Bot.h"
#include "Helper/UI.h"
#include "Helper/constants.h"
#include "Helper/CommonHelper.h"
USING_NS_AX;

class GameScene : public Layer
{
public:
    static Scene *createScene();
    virtual bool init();
//    virtual bool init() override; // Default init without parameters
//    virtual bool init(const std::string& url);  Parameterized init

    CREATE_FUNC(GameScene)
protected:
    bool onTouchBegan(Touch *touch, Event *event);
    void InitGridRects();
    void InitGridPieces();
//
    void CheckAndPlacePiece(Touch *touch);
    void CheckWin(int x, int y);
    void Check3PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int pieceToCheck);
    void CheckWinCondition(int x, int y, int pieceToCheck);
    Rect GetRectSpaceFromTag(int tag);
//
    Sprite *gridSprite;
    Rect gridSpaces[3][3];
    Sprite *gridPieces[3][3];
//
    int gridArray[3][3];
//
    int turn;
    int gameState;
//
    Bot *bot;
    UI *ui;
};


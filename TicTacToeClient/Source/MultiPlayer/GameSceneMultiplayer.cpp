#include "GameSceneMultiplayer.h"


Scene *GameSceneMultiplayer::createScene()
{
    auto scene = Scene::create();
    auto layer = GameSceneMultiplayer::create();
    scene->addChild(layer);
    return scene;
}
bool GameSceneMultiplayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    CommonHelper::AddCentredBackground(GAME_BACKGROUND_FILEPATH,this);
    gridSprite = Sprite::create(GRID_FILEPATH);
    gridSprite->setPosition(CommonHelper::getScreenCentre());
    this->addChild(gridSprite);

    InitGridRects();
    InitGridPieces();

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            gridArray[x][y] = EMPTY_PIECE;
        }
    }

    turn = X_PIECE;
    ui = new UI(this, gameState, "Multiplayer");
    gameState = STATE_PLAYING;

    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = AX_CALLBACK_2(GameSceneMultiplayer::onTouchBegan, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


bool GameSceneMultiplayer::onTouchBegan(ax::Touch *touch, ax::Event *event)
{
    if (STATE_PLAYING == gameState)
    {
        CheckAndPlacePiece(touch);
    }
    return true;
}

void GameSceneMultiplayer::CheckAndPlacePiece(ax::Touch *touch)
{
    Rect rect1 = gridSprite->getBoundingBox();
    Point touchPoint = touch->getLocation();

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (gridSpaces[x][y].containsPoint(touchPoint))
            {
                if (gridArray[x][y] == EMPTY_PIECE)
                {
                    gameState = STATE_PLACING_PIECE;

                    gridArray[x][y] = turn;

                    // check who's current turn it is and switch
                    if (X_PIECE == turn)
                    {
                        gridPieces[x][y]->setTexture(X_PIECE_FILEPATH);
                    }
                    else
                    {
                        gridPieces[x][y]->setTexture(O_PIECE_FILEPATH);
                    }

                    gridPieces[x][y]->setVisible(true);
                    // gridPieces[x][y]->setScale(.5,.5);
                    gridPieces[x][y]->runAction(Sequence::create(FadeIn::create(PIECE_FADE_IN_TIME), CallFunc::create(std::bind(&GameSceneMultiplayer::CheckWin, this, x, y)), NULL));
                }
            }
        }
    }
}
void GameSceneMultiplayer::CheckWin(int x, int y)
{
    int result = -1;
    Check3PiecesForMatch(0, 0, 1, 0, 2, 0, result);
    Check3PiecesForMatch(0, 1, 1, 1, 2, 1, result);
    Check3PiecesForMatch(0, 2, 1, 2, 2, 2, result);
    Check3PiecesForMatch(0, 0, 0, 1, 0, 2, result);
    Check3PiecesForMatch(1, 0, 1, 1, 1, 2, result);
    Check3PiecesForMatch(2, 0, 2, 1, 2, 2, result);
    Check3PiecesForMatch(0, 0, 1, 1, 2, 2, result);
    Check3PiecesForMatch(0, 2, 1, 1, 2, 0, result);

    // check who's current turn it is and switch
    if (X_PIECE == turn)
    {
        turn = O_PIECE;
    }
    else
    {
        turn = X_PIECE;
    }
    if (STATE_PLACING_PIECE == gameState)
    {
        gameState = STATE_PLAYING;
    }
    int emptyNum = 9;

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (EMPTY_PIECE != gridArray[x][y])
            {
                emptyNum--;
            }
        }
    }

    // check if the game is a draw
    if (0 == emptyNum)
    {
        gameState = STATE_DRAW;
    }

    if (STATE_DRAW == gameState || STATE_LOSE == gameState || STATE_WON == gameState)
    {
        ui->ShowGameOver(this, GAME_TYPE_OFFLINE, "Multiplayer", result);
    }

    std::cout << gameState << std::endl;
}


void GameSceneMultiplayer::Check3PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int &result)
{
    if (turn == gridArray[x1][y1] && turn == gridArray[x2][y2] && turn == gridArray[x3][y3])
    {
        std::string winningPieceStr;

        if (O_PIECE == turn)
        {
            winningPieceStr = O_WINNING_PIECE_FILEPATH;
            result = STATE_O_WIN;
        }
        else
        {
            winningPieceStr = X_WINNING_PIECE_FILEPATH;
            result = STATE_X_WIN;
        }

        Sprite *winningPieces[3];

        winningPieces[0] = Sprite::create(winningPieceStr);
        winningPieces[0]->setPosition(gridPieces[x1][y1]->getPosition());
        //        winningPieces[0]->setOpacity( 0 );
        this->addChild(winningPieces[0]);
        winningPieces[1] = Sprite::create(winningPieceStr);
        winningPieces[1]->setPosition(gridPieces[x2][y2]->getPosition());
        //        winningPieces[1]->setOpacity( 0 );
        this->addChild(winningPieces[1]);
        winningPieces[2] = Sprite::create(winningPieceStr);
        winningPieces[2]->setPosition(gridPieces[x3][y3]->getPosition());
        //        winningPieces[2]->setOpacity( 0 );
        this->addChild(winningPieces[2]);

        winningPieces[0]->runAction(Blink::create(1.0f, 3));
        winningPieces[1]->runAction(Blink::create(1.0f, 3));
        winningPieces[2]->runAction(Blink::create(1.0f, 3));

        gameState = STATE_WON;
    }
}


void GameSceneMultiplayer::InitGridRects()
{
    gridSpaces[0][0] = Rect(gridSprite->getBoundingBox().getMinX(),
                            gridSprite->getBoundingBox().getMinY(),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[1][0] = Rect(gridSprite->getBoundingBox().getMinX() + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().getMinY(),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[2][0] = Rect(gridSprite->getBoundingBox().getMinX() + ((gridSprite->getBoundingBox().size.width / 3) * 2),
                            gridSprite->getBoundingBox().getMinY(),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[0][1] = Rect(gridSprite->getBoundingBox().getMinX(),
                            gridSprite->getBoundingBox().getMinY() + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[1][1] = Rect(gridSprite->getBoundingBox().getMinX() + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().getMinY() + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[2][1] = Rect(gridSprite->getBoundingBox().getMinX() + ((gridSprite->getBoundingBox().size.width / 3) * 2),
                            gridSprite->getBoundingBox().getMinY() + gridSprite->getBoundingBox().size.height / 3,
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[0][2] = Rect(gridSprite->getBoundingBox().getMinX(),
                            gridSprite->getBoundingBox().getMinY() + ((gridSprite->getBoundingBox().size.height / 3) * 2),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[1][2] = Rect(gridSprite->getBoundingBox().getMinX() + gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().getMinY() + ((gridSprite->getBoundingBox().size.height / 3) * 2),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);

    gridSpaces[2][2] = Rect(gridSprite->getBoundingBox().getMinX() + ((gridSprite->getBoundingBox().size.width / 3) * 2),
                            gridSprite->getBoundingBox().getMinY() + ((gridSprite->getBoundingBox().size.height / 3) * 2),
                            gridSprite->getBoundingBox().size.width / 3,
                            gridSprite->getBoundingBox().size.height / 3);
}
void GameSceneMultiplayer::InitGridPieces()
{
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            gridPieces[x][y] = Sprite::create(X_PIECE_FILEPATH);
            gridPieces[x][y]->setPosition(Vec2(gridSprite->getPositionX() + (gridPieces[x][y]->getContentSize().width * (x - 1)), gridSprite->getPositionY() + (gridPieces[x][y]->getContentSize().height * (y - 1))));
            gridPieces[x][y]->setVisible(false);
            gridPieces[x][y]->setOpacity(0);
            gridPieces[x][y]->setTag(x * 3 + y);
            this->addChild(gridPieces[x][y]);
        }
    }
}

ax::Rect GameSceneMultiplayer::GetRectSpaceFromTag(int tag)
{
    int row = tag / 3;
    int col = tag % 3;
    return gridSpaces[row][col];
}

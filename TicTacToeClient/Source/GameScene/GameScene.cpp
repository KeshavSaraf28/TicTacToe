#include "GameScene.h"

Scene *GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}
//bool GameScene::init(const std::string& url)
//{
//    if (!Scene::init())
//    {
//        return false;
//    }
//
//    // Initialization code with URL
//    return true;
//}
bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    CommonHelper::AddCentredBackground(GAME_BACKGROUND_FILEPATH, this);
    
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
    bot =new Bot(turn);
    ui = new UI(this, gameState, "SinglePlayer");
    gameState = STATE_PLAYING;
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = AX_CALLBACK_2(GameScene::onTouchBegan, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
    
    
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    if (STATE_PLAYING == gameState)
    {
        CheckAndPlacePiece(touch);
    }
    return true;
}

void GameScene::CheckAndPlacePiece(Touch *touch)
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
                    gridPieces[x][y]->runAction(Sequence::create(FadeIn::create(PIECE_FADE_IN_TIME), CallFunc::create(std::bind(&GameScene::CheckWin, this, x, y)), NULL));
                }
            }
        }
    }
}
void GameScene::CheckWin(int x, int y)
{
    CheckWinCondition(x, y, PLAYER_PIECE);

    if (STATE_WON != gameState)
    {
        gameState = STATE_BOT_PLAYING;
        bot->PlacePiece(&gridArray, gridPieces, &gameState);

        CheckWinCondition(x, y, BOT_PIECE);
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
        ui->ShowGameOver(this, GAME_TYPE_OFFLINE, "SinglePlayer", -1);
    }

    std::cout << gameState << std::endl;
}

void GameScene::CheckWinCondition(int x, int y, int pieceToCheck)
{
    // Check Row
    Check3PiecesForMatch(0, 0, 1, 0, 2, 0, pieceToCheck);
    Check3PiecesForMatch(0, 1, 1, 1, 2, 1, pieceToCheck);
    Check3PiecesForMatch(0, 2, 1, 2, 2, 2, pieceToCheck);

    // Check Column
    Check3PiecesForMatch(0, 0, 0, 1, 0, 2, pieceToCheck);
    Check3PiecesForMatch(1, 0, 1, 1, 1, 2, pieceToCheck);
    Check3PiecesForMatch(2, 0, 2, 1, 2, 2, pieceToCheck);

    // Check Diagonal
    Check3PiecesForMatch(0, 0, 1, 1, 2, 2, pieceToCheck);
    Check3PiecesForMatch(0, 2, 1, 1, 2, 0, pieceToCheck);
}

void GameScene::Check3PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int pieceToCheck)
{
    if (pieceToCheck == gridArray[x1][y1] && pieceToCheck == gridArray[x2][y2] && pieceToCheck == gridArray[x3][y3])
    {
        std::string winningPieceStr;

        if (O_PIECE == pieceToCheck)
        {
            winningPieceStr = O_WINNING_PIECE_FILEPATH;
        }
        else
        {
            winningPieceStr = X_WINNING_PIECE_FILEPATH;
        }

        Sprite *winningPieces[3];

        winningPieces[0] = Sprite::create(winningPieceStr);
        winningPieces[0]->setPosition(gridPieces[x1][y1]->getPosition());
        // winningPieces[0]->setOpacity( 0 );
        //  winningPieces[0]->setScale(.5, .5);
        this->addChild(winningPieces[0]);
        winningPieces[1] = Sprite::create(winningPieceStr);
        winningPieces[1]->setPosition(gridPieces[x2][y2]->getPosition());
        // winningPieces[1]->setScale(.5, .5);
        // winningPieces[1]->setOpacity( 0 );
        this->addChild(winningPieces[1]);
        winningPieces[2] = Sprite::create(winningPieceStr);
        winningPieces[2]->setPosition(gridPieces[x3][y3]->getPosition());
        // winningPieces[2]->setScale(.5, .5);
        // winningPieces[2]->setOpacity( 0 );
        this->addChild(winningPieces[2]);

        winningPieces[0]->runAction(Blink::create(1.0f, 3));
        winningPieces[1]->runAction(Blink::create(1.0f, 3));
        winningPieces[2]->runAction(Blink::create(1.0f, 3));

        if (PLAYER_PIECE == pieceToCheck)
        {
            gameState = STATE_WON;
        }
        else
        {
            gameState = STATE_LOSE;
        }
    }
}


void GameScene::InitGridRects()
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
void GameScene::InitGridPieces()
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

Rect GameScene::GetRectSpaceFromTag(int tag)
{
    int row = tag / 3;
    int col = tag % 3;
    return gridSpaces[row][col];
}

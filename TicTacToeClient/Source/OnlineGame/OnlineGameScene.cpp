#include "OnlineGameScene.h"


USING_NS_AX;
OnlineGameScene* OnlineGameScene::instance=nullptr;
Scene *OnlineGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OnlineGameScene::create();
    scene->addChild(layer);
    instance=layer;
    return scene;
}
OnlineGameScene* OnlineGameScene::getInstance()
{
    return OnlineGameScene::instance;
}
//OnlineGameScene* OnlineGameScene::create(const std::string& url)
//{
//    OnlineGameScene *pRet = new(std::nothrow) OnlineGameScene();
//    if (pRet && pRet->init(url)) // Call init with URL
//    {
//        pRet->autorelease();
//        return pRet;
//    }
//    else
//    {
//        delete pRet;
//        return nullptr;
//    }
//}
bool OnlineGameScene::init()
{
    if (!Layer::init()) // Call base class init with URL
    {
        return false;
    }
    CommonHelper::AddCentredBackground(GAME_BACKGROUND_FILEPATH, this);
    waitingSprite = Sprite::create(WAITING_FILEPATH);
    waitingSprite->setPosition(CommonHelper::getScreenCentre());
    this->addChild(waitingSprite);

    
    gridSprite= Sprite::create(GRID_FILEPATH);
    gridSprite->setPosition(CommonHelper::getScreenCentre());
    gridSprite->setVisible(false);
    this->addChild(gridSprite);
    yourTurnLabel = Label::createWithTTF("Your Turn", "fonts/Marker Felt.ttf", 40);
    yourTurnLabel->setTextColor(Color4B(54, 57, 67, 255));
    yourTurnLabel->setPosition(CommonHelper::getScreenCentre().x, CommonHelper::getScreenCentre().y * 1.75);
    this->addChild(yourTurnLabel);
    yourTurnLabel->setVisible(false);

    opponentTurnLabel = Label::createWithTTF("Opponent's Turn", "fonts/Marker Felt.ttf", 40);
    opponentTurnLabel->setTextColor(Color4B(54, 57, 67, 255));
    opponentTurnLabel->setPosition(CommonHelper::getScreenCentre().x, CommonHelper::getScreenCentre().y * 1.75);
    this->addChild(opponentTurnLabel);
    opponentTurnLabel->setVisible(false);

    InitGridRects();
    InitGridPieces();

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            gridArray[x][y] = EMPTY_PIECE;
        }
    }
    mySymbol="";
    gameState = STATE_PLAYING;
    ui = new UI(this, gameState, "OnlineGame");
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    AXLOG("websocket working");
    wsManager=WebSocketManager::getInstance(false);


    listener->onTouchBegan = AX_CALLBACK_2(OnlineGameScene::onTouchBegan, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

bool OnlineGameScene::onTouchBegan(Touch *touch, Event *event)
{
    AXLOG("Touch not working.1");
    if (turn == 1)
    {
        AXLOG("Touch not working.2");
        CheckAndPlacePiece(touch);
    }
    return true;
}
void OnlineGameScene::CheckAndPlacePiece(Touch *touch)
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
                    sendMove(wsManager->getWebSocket(),x,y);
                    turn = 0;
                }
            }
        }
    }
}
//
std::vector<int> OnlineGameScene::getWinningCoordinates()
{
    std::vector<std::vector<int>> pos{
        {0, 0, 0, 1, 0, 2},
        {1, 0, 1, 1, 1, 2},
        {2, 0, 2, 1, 2, 2},
        {0, 0, 1, 0, 2, 0},
        {0, 1, 1, 1, 2, 1},
        {0, 2, 1, 2, 2, 2},
        {0, 0, 1, 1, 2, 2},
        {2, 0, 1, 1, 0, 2}};
    for (int i = 0; i < pos.size(); i++)
    {
        if ((gridArray[pos[i][0]][pos[i][1]] == gridArray[pos[i][2]][pos[i][3]]) && (gridArray[pos[i][2]][pos[i][3]] == gridArray[pos[i][4]][pos[i][5]]) && (gridArray[pos[i][0]][pos[i][1]] == O_PIECE || gridArray[pos[i][0]][pos[i][1]] == X_PIECE))
        {
            return pos[i];
        }
    }
    return pos[0];
}
//
void OnlineGameScene::CheckWin(int state)
{
    if (STATE_DRAW == state || STATE_LOSE == state || STATE_WON == state)
    {
        gameState = state;
        if (gameState != STATE_DRAW)
        {
            string winningPieceStr;

            if (gameState == STATE_LOSE)
            {
                if (mySymbol == "X")
                    winningPieceStr = O_WINNING_PIECE_FILEPATH;
                else
                    winningPieceStr = X_WINNING_PIECE_FILEPATH;
            }
            else
            {
                if (mySymbol == "O")
                    winningPieceStr = O_WINNING_PIECE_FILEPATH;
                else
                    winningPieceStr = X_WINNING_PIECE_FILEPATH;
            }

            std::vector<int> v = OnlineGameScene::getWinningCoordinates();

            Sprite *winningPieces[3];

            winningPieces[0] = Sprite::create(winningPieceStr);
            winningPieces[0]->setPosition(gridPieces[v[0]][v[1]]->getPosition());
            winningPieces[0]->setOpacity(0);
            this->addChild(winningPieces[0]);
            winningPieces[1] = Sprite::create(winningPieceStr);
            winningPieces[1]->setPosition(gridPieces[v[2]][v[3]]->getPosition());
            winningPieces[1]->setOpacity(0);
            this->addChild(winningPieces[1]);
            winningPieces[2] = Sprite::create(winningPieceStr);
            winningPieces[2]->setPosition(gridPieces[v[4]][v[5]]->getPosition());
            winningPieces[2]->setOpacity(0);
            this->addChild(winningPieces[2]);

            winningPieces[0]->runAction(FadeIn::create(PIECE_FADE_IN_TIME));
            winningPieces[1]->runAction(Sequence::create(DelayTime::create(PIECE_FADE_IN_TIME * 0.5), FadeIn::create(PIECE_FADE_IN_TIME), NULL));
            winningPieces[2]->runAction(Sequence::create(DelayTime::create(PIECE_FADE_IN_TIME * 1.5), FadeIn::create(PIECE_FADE_IN_TIME), NULL));
        }
    }
}

void OnlineGameScene::FillGameData(GameDataParser gameData)
{
    turn=gameData.turn;
    state=gameData.state;
    int count=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(gameData.matrix[i][j]!="") count++;
        }
    }
    if(count==0 && turn==1){
        mySymbol="X";
    }
    else if(count==0 && turn==0)
    {
        mySymbol="O";
    }
    
}
void OnlineGameScene::FillGrid(vector<vector<string>>& matrix)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(gridArray[i][j]!=SymbolToPiece[matrix[i][j]])
            {
                gridArray[i][j]=SymbolToPiece[matrix[i][j]];
                if (X_PIECE == gridArray[i][j])
                {
                    gridPieces[i][j]->setTexture(X_PIECE_FILEPATH);
                }
                else
                {
                    gridPieces[i][j]->setTexture(O_PIECE_FILEPATH);
                }

                gridPieces[i][j]->setVisible(true);
                gridPieces[i][j]->runAction(FadeIn::create(PIECE_FADE_IN_TIME));
                
            }
        }
    }
}

void OnlineGameScene::onMessage(GameDataParser gameData)
{
    
    
    if(gameData.state=="In Progress")
    {
        waitingSprite->setVisible(false);
        OnlineGameScene::FillGameData(gameData);
        OnlineGameScene::FillGrid(gameData.matrix);
        gridSprite->setVisible(true);
        if(turn==1)
        {
            opponentTurnLabel->setVisible(false);
            yourTurnLabel->setVisible(true);
        }
        else
        {
            opponentTurnLabel->setVisible(true);
            yourTurnLabel->setVisible(false);
        }
        
        
    }
    else if(gameData.state=="Win")
    {
        OnlineGameScene::FillGrid(gameData.matrix);
        gameState=STATE_WON;
        OnlineGameScene::CheckWin(STATE_WON);
    }
    else if(gameData.state=="Lose")
    {
        OnlineGameScene::FillGrid(gameData.matrix);
        gameState=STATE_LOSE;
        OnlineGameScene::CheckWin(STATE_LOSE);
    }
    else if(gameData.state=="Draw")
    {
        OnlineGameScene::FillGrid(gameData.matrix);
        gameState=STATE_DRAW;
        OnlineGameScene::CheckWin(STATE_DRAW);
    }
    if(gameState==STATE_DRAW || gameState==STATE_WON || gameState==STATE_LOSE)
        ui->ShowGameOver(this, GAME_TYPE_ONLINE, "OnlineGame", gameState);
}


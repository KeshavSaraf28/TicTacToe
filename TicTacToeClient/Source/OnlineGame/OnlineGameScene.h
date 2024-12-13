#pragma once
#include "axmol.h"
#include "GameScene/GameScene.h"
#include <vector>
#include <string>
#include "GameDataParser.h"
#include "Helper/CommonHelper.h"
#include "Helper/constants.h"
#include <iostream>
#include <map>
#include "WebSocketManager.h"
#include "Helper/UI.h"
USING_NS_AX;
using namespace std;

class OnlineGameScene : public GameScene
{
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(OnlineGameScene);
//    static OnlineGameScene* create(const std::string& url);

private:
    bool onTouchBegan(Touch *touch, Event *event);

    void CheckAndPlacePiece(Touch *touch);
    void CheckWin(int state);
    WebSocketManager* wsManager;
    Sprite *waitingSprite;
    Label *yourTurnLabel;
    Label *opponentTurnLabel;
    string url;

//    NetworkController *ws;
    UI *ui;
    string state;
    int turn;
    string mySymbol;
    
public:
    static OnlineGameScene* instance;
    static OnlineGameScene* getInstance();
    void FillGameData(GameDataParser gameData);
    void FillGrid(vector<vector<string>>& matrix);
    void onMessage(GameDataParser gameData);
    unordered_map<string,int> SymbolToPiece={
        {"X",X_PIECE},
        {"O",O_PIECE},
        {"",EMPTY_PIECE}
    };
    

//    void OnlineGameScene::getGameData(GameDataParser gameData)
    vector<int> getWinningCoordinates();
};

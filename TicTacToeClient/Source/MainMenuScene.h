#pragma once

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "Helper/constants.h"
#include "MultiPlayer/GameSceneMultiplayer.h"
#include "OnlineGame/OnlineGameScene.h"
#include "Helper/CommonHelper.h"
#include "OnlineGame/WebSocketManager.h"
#include <fstream>   // For std::ofstream
#include <string> 

USING_NS_AX;

using namespace ui;

//USING_NS_AX_EXT;

class MainMenuScene : public ax::Layer
{
public:
    static ax::Scene *createScene();
    virtual bool init();
    CREATE_FUNC(MainMenuScene);

private:
    void touchEvent(Object *sender, Widget::TouchEventType type);
    void editBoxReturn(ui::EditBox* editBox);
};

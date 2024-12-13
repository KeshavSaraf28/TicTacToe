#include "UI.h"
#include "MainMenuScene.h"
#include "MultiPlayer/GameSceneMultiplayer.h"
#include "OnlineGame/OnlineGameScene.h"


UI::UI(Layer *layer, int &gameState, std::string gameType)
{
    this->gameState = &gameState;
//
    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
    MenuItemSprite *pauseItem = MenuItemSprite::create(Sprite::create(PAUSE_BUTTON), Sprite::create(PAUSE_BUTTON_PRESSED), AX_CALLBACK_1(UI::PauseGame, this));
    pauseItem->setPosition(Vec2(screenSize.width - (pauseItem->getContentSize().width / 2) + origin.x, pauseItem->getContentSize().height / 2 + origin.y));
//
    Menu *pauseButtons = Menu::create(pauseItem, NULL);
    pauseButtons->setPosition(Vec2::ZERO);
    layer->addChild(pauseButtons);
////
    pausebackground = Sprite::create(GAME_BACKGROUND_FILEPATH);
    pausebackground->setPosition(CommonHelper::getScreenCentre());
    pausebackground->setOpacity(0);
    layer->addChild(pausebackground);
//
    MenuItemImage *overlayPauseWindowItem = MenuItemImage::create(PAUSE_WINDOW, PAUSE_WINDOW, PAUSE_WINDOW, NULL);
    overlayPauseWindowItem->setEnabled(false);
    MenuItemSprite *resumeItem = MenuItemSprite::create(Sprite::create(RESUME_BUTTON), Sprite::create(RESUME_BUTTON_PRESSED), Sprite::create(RESUME_BUTTON), AX_CALLBACK_1(UI::PauseGame, this));
    resumeItem->setPosition(Vec2(-1.2 * overlayPauseWindowItem->getContentSize().width / 4, resumeItem->getPositionY()));
    MenuItemSprite *mainMenuItem = MenuItemSprite::create(Sprite::create(HOME_BUTTON), Sprite::create(HOME_BUTTON_PRESSED), Sprite::create(HOME_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToMainMenu, this));
    mainMenuItem->setPosition(Vec2(1.2 * overlayPauseWindowItem->getContentSize().width / 4, mainMenuItem->getPositionY()));
//
    MenuItemSprite *restartButton;
    if (gameType.compare("SinglePlayer") == 0)
    {
        restartButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToGameScene, this));
    }
    else if (gameType.compare("Multiplayer") == 0)
    {
        restartButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToMultiplayerGameScene, this));
    }
    else
    {
        restartButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToOnlineGameScene, this));
    }
    restartButton->setPosition(Vec2(0, restartButton->getPositionY()));
//
    pauseMenu = Menu::create(overlayPauseWindowItem, resumeItem, mainMenuItem, restartButton, NULL);
    pauseMenu->setPosition(Vec2(CommonHelper::getScreenCentre().x, CommonHelper::getScreenCentre().y + screenSize.height));
    layer->addChild(pauseMenu);
}

UI::~UI()
{
}

void UI::PauseGame(Object *pSender)
{
    Size screenSize = Director::getInstance()->getVisibleSize();
//
    if (STATE_PLAYING == *gameState)
    {
        *gameState = STATE_PAUSED;
        pausebackground->runAction(FadeIn::create(PAUSE_BACKGROUND_FADE_IN_TIME));
//
        EaseBounceOut *menuActionEasing = EaseBounceOut::create(MoveTo::create(MENU_MOVE_BOUNCE_DURATION, CommonHelper::getScreenCentre()));
        pauseMenu->runAction(menuActionEasing);
    }
    else if (STATE_PAUSED == *gameState)
    {
        *gameState = STATE_PLAYING;
        pausebackground->runAction(FadeOut::create(PAUSE_BACKGROUND_FADE_IN_TIME));

        EaseBounceOut *menuActionEasing = EaseBounceOut::create(MoveTo::create(MENU_MOVE_BOUNCE_DURATION, Vec2(CommonHelper::getScreenCentre().x, CommonHelper::getScreenCentre().y+ screenSize.height)));
        pauseMenu->runAction(menuActionEasing);
    }
}

void UI::ShowGameOver(Layer *layer, int gameType, std::string gameMode, int result)
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
    auto background = Sprite::create(MAIN_MENU_BACKGROUND_PATH);
    background->setPosition(CommonHelper::getScreenCentre());
    background->setOpacity(0);
    background->runAction(Sequence::create(DelayTime::create(1.0f * .5), FadeIn::create(GAME_OVER_BACKGROUND_FADE_IN_TIME), NULL));
    layer->addChild(background);
    MenuItemImage *overlayWindowItem;
//
    if (gameMode == "SinglePlayer" || gameMode=="OnlineGame")
    {
        if (*gameState == STATE_WON)
        {
            overlayWindowItem = MenuItemImage::create(VICTORY_WINDOW_FILEPATH, VICTORY_WINDOW_FILEPATH);
        }
        else if (*gameState == STATE_LOSE)
        {
            overlayWindowItem = MenuItemImage::create(LOSE_WINDOW_FILEPATH, LOSE_WINDOW_FILEPATH);
        }
        else
        {
            overlayWindowItem = MenuItemImage::create(OVERLAY_WINDOW_FILEPATH, OVERLAY_WINDOW_FILEPATH);
            auto label = Label::createWithTTF("It's a Draw", "fonts/Marker Felt.ttf", 40);
            label->setTextColor(Color4B(128, 0, 128, 255));
            label->setPosition(Vec2(CommonHelper::getScreenCentre().x*0.6, CommonHelper::getScreenCentre().y));
            // label->setScale(.5,.5);
            overlayWindowItem->addChild(label);
        }
        
    }
    else
    {
        overlayWindowItem = MenuItemImage::create(OVERLAY_WINDOW_FILEPATH, OVERLAY_WINDOW_FILEPATH);
        Label* label=Label::createWithTTF("It's a Draw", "fonts/Marker Felt.ttf", 40);
        if (result == STATE_X_WIN)
        {
            label = Label::createWithTTF("X is the Winner", "fonts/Marker Felt.ttf", 40);
        }
        else if (result == STATE_O_WIN)
        {
            label = Label::createWithTTF("O is the Winner", "fonts/Marker Felt.ttf", 40);
        }
        label->setTextColor(Color4B(128, 0, 128, 255));
        label->setPosition(Vec2(CommonHelper::getScreenCentre().x*0.6, CommonHelper::getScreenCentre().y));
        // label->setScale(.5,.5);
        overlayWindowItem->addChild(label);
    }
    
//        Overlay window was acting as a button, so disabled it.
    overlayWindowItem->MenuItem::setEnabled(false);
//
    MenuItemSprite *retryButton;
    if (gameType == GAME_TYPE_OFFLINE && gameMode=="SinglePlayer")
    {
        retryButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToGameScene, this));
        
    }
    if (gameType == GAME_TYPE_OFFLINE && gameMode=="Multiplayer")
    {
        retryButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToMultiplayerGameScene, this));
    }
    else if (gameType == GAME_TYPE_ONLINE)
    {
        retryButton = MenuItemSprite::create(Sprite::create(RETRY_BUTTON), Sprite::create(RETRY_BUTTON_PRESSED), Sprite::create(RETRY_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToOnlineGameScene, this));
    }
    retryButton->setPosition(Vec2(-.75 * overlayWindowItem->getContentSize().width / 4, retryButton->getPositionY()));
//
    auto homeButton = MenuItemSprite::create(Sprite::create(HOME_BUTTON), Sprite::create(HOME_BUTTON_PRESSED), Sprite::create(HOME_BUTTON_PRESSED), AX_CALLBACK_1(UI::GoToMainMenu, this));
    homeButton->setPosition(Vec2(.75 * overlayWindowItem->getContentSize().width / 4, homeButton->getPositionY()));
//
    auto menu = Menu::create(overlayWindowItem, retryButton, homeButton, NULL);
    menu->setPosition(Vec2(CommonHelper::getScreenCentre().x, CommonHelper::getScreenCentre().y+screenSize.height));
    // menu->setScale(.5,.5);
    layer->addChild(menu);
//
    EaseBounceOut *menuActionEasing = EaseBounceOut::create(MoveTo::create(MENU_MOVE_BOUNCE_DURATION,CommonHelper::getScreenCentre()));
    Sequence *menuShowSeq = Sequence::create(DelayTime::create(1.0f * 0.5), menuActionEasing, NULL);
    menu->runAction(menuShowSeq);
//    std::cout<<"Game over\n";
}

void UI::GoToGameScene(Object *pSender)
{
//    std::cout<<"go back to new game\n";
    Scene *scene = GameScene::createScene();
    TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
    Director::getInstance()->replaceScene(transition);
}

void UI::GoToMultiplayerGameScene(Object *pSender)
{
    Scene *scene = GameSceneMultiplayer::createScene();
    TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
    Director::getInstance()->replaceScene(transition);
}
void UI::GoToOnlineGameScene(Object *pSender)
{
//        // Pass the room code to the OnlineGameScene
        auto scene = OnlineGameScene::createScene();
        TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
        Director::getInstance()->replaceScene(transition);
//    });
}
void UI::GoToMainMenu(Object *pSender)
{
    Scene *scene = MainMenuScene::createScene();
    TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
    Director::getInstance()->replaceScene(transition);
}


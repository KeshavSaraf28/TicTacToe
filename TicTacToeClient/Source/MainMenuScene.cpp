#include "MainMenuScene.h"


Scene *MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

//    if i come home from game,delete that instance
    WebSocketManager* wsManager=WebSocketManager::getInstance(true);
    if(wsManager)
        wsManager->DeleteInstance();


    Size screenSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto layer = this;

    auto background = Sprite::create(MAIN_MENU_BACKGROUND_PATH);
    background->setPosition(CommonHelper::getScreenCentre());
    layer->addChild(background);

    auto title = Sprite::create("res/tictactoe logo.png");
    title->setScale(0.5, 0.5);
    title->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.75 + origin.y));
    layer->addChild(title);

    auto playButton = Button::create(MAIN_MENU_PLAY_BUTTON, MAIN_MENU_PLAY_BUTTON_PRESSED);
    Vec2 playPostion = Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.45 + origin.y);
    playButton->setPosition(playPostion);
    layer->addChild(playButton);

    playButton->addTouchEventListener(AX_CALLBACK_2( MainMenuScene::touchEvent, this));
    playButton->setTag(TAG_MAIN_MENU_SINGLEPLAYER_BUTTON);

    // Multiplayer PlayButton
    auto multiplayerPlayButton = Button::create(MAIN_MENU_MULTI_PLAY_BUTTON, MAIN_MENU_MULTI_PLAY_BUTTON_PRESSED);
    Vec2 multiplayerPlayPostion = Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.3 + origin.y);
    multiplayerPlayButton->setPosition(multiplayerPlayPostion);
    layer->addChild(multiplayerPlayButton);

    multiplayerPlayButton->setTag(TAG_MAIN_MENU_MULTIPLAYER_BUTTON);
    multiplayerPlayButton->addTouchEventListener(AX_CALLBACK_2(MainMenuScene::touchEvent, this));
    // PlayOnline PlayButton
    auto playOnlineButton = Button::create("res/Play Online.png");
    Vec2 playOnlinePostion = Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.15 + origin.y);
    playOnlineButton->setPosition(playOnlinePostion);
    layer->addChild(playOnlineButton);
    playOnlineButton->setTag(TAG_MAIN_MENU_ONLINE_BUTTON);
    playOnlineButton->addTouchEventListener(AX_CALLBACK_2(MainMenuScene::touchEvent, this));

    return true;
}

void MainMenuScene::touchEvent(Object *sender, Widget::TouchEventType type)
{
    Node *node = (Node *)sender;
    switch (type)
    {
    case Widget::TouchEventType::BEGAN:
        break;
    case Widget::TouchEventType::MOVED:
        break;
    case Widget::TouchEventType::ENDED:
        if (TAG_MAIN_MENU_SINGLEPLAYER_BUTTON == node->getTag())
        {
            std::cout<<"new game"<<std::endl;
            Scene *scene = GameScene::createScene();
            TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
            Director::getInstance()->replaceScene(transition);
        }
        else if (TAG_MAIN_MENU_MULTIPLAYER_BUTTON == node->getTag())
        {
            Scene *scene = GameSceneMultiplayer::createScene();
            TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
            Director::getInstance()->replaceScene(transition);
        }
        else if (TAG_MAIN_MENU_ONLINE_BUTTON == node->getTag())
        {
            auto popupLayer = LayerColor::create(Color4B(0, 0, 0, 255)); // Fully opaque black color

            this->addChild(popupLayer, 10);
            auto touchListener = EventListenerTouchOneByOne::create();
           touchListener->setSwallowTouches(true);
           touchListener->onTouchBegan = [](Touch* touch, Event* event) {
               return true; // Swallow all touches to prevent background interaction
           };
//            Director::getInstance()::EventDispatcher()::addEventListenerWithSceneGraphPriority(touchListener,popupLayer);
           _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, popupLayer);

            auto inputBox = ui::EditBox::create(Size(450,150), ui::Scale9Sprite::create(PAUSE_WINDOW)); // Assuming "input_bg.png" is a valid image file
            inputBox->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                                       Director::getInstance()->getVisibleSize().height / 2 + 30));
            inputBox->setFontName("fonts/arial.ttf");
            inputBox->setFontSize(30);
            inputBox->setFontColor(Color3B::BLACK);
            inputBox->setPlaceHolder("Enter text");
            inputBox->setPlaceholderFontColor(Color3B::BLACK);
            inputBox->setMaxLength(100);
            inputBox->setInputMode(ui::EditBox::InputMode::ANY); // Text-only input mode
            popupLayer->addChild(inputBox);


            // Create a confirm button to proceed
//            auto confirmButton = Button::create("res/Play Online.png");
            auto confirmButton = ui::Button::create();
            confirmButton->setTitleText("Confirm");
            confirmButton->setTitleColor(Color3B::WHITE);
            confirmButton->setTitleFontSize(24);
            confirmButton->setColor(Color3B::WHITE);
            confirmButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                                            Director::getInstance()->getVisibleSize().height / 2 - 80));
            popupLayer->addChild(confirmButton);
//
            confirmButton->addClickEventListener([=](Object *){
                std::string url = inputBox->getText();
//                AXLOG(url.c_str());
                string filePath=FileUtils::getInstance()->getWritablePath() + "/saved_url.txt";
                std::ofstream file(filePath, std::ios::out | std::ios::trunc);
                if (file.is_open())
                {
                    file << url; // Write the URL to the file
                    file.close();
                    AXLOG("URL saved to file: %s", filePath.c_str());
                }
                else
                {
                    AXLOG("Failed to open file for writing: %s", filePath.c_str());
                }

                // Pass the room code to the OnlineGameScene
                auto scene = OnlineGameScene::createScene();
                TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
                Director::getInstance()->replaceScene(transition);
            });
        }
        break;
    case Widget::TouchEventType::CANCELED:
        break;
    default:
        break;
    }
}

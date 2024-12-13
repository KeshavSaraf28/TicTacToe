#include "SplashScene.h"



Scene *SplashScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SplashScene::create();
    scene->addChild(layer);
    return scene;
}

bool SplashScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite *sprite = Sprite::create("res/gameberry labs.png");
    sprite->setPosition(CommonHelper::getScreenCentre());
    Layer *layer = this;
    sprite->setScale(0.8, 0.8);
    layer->addChild(sprite);
//
    Sprite *loadingAnimation = Sprite::create(std::string(LOADING_SCREEN_PATH) + "loading.png");
    loadingAnimation->setScale(0.7, 0.7);
    loadingAnimation->setPosition(Vec2(CommonHelper::getScreenCentre().x, Director::getInstance()->getVisibleSize().height / 4 + Director::getInstance()->getVisibleOrigin().y));
    this->addChild(loadingAnimation);
//
    Vector<SpriteFrame *> frames;
    for (int i = 1; i <= 3; i++)
    {
        std::string frameName = std::string(LOADING_SCREEN_PATH) + "loading_" + std::to_string(i) + ".png";
        SpriteFrame *frame = SpriteFrame::create(frameName, Rect(0, 0, 721, 64));
        frames.pushBack(frame);
    }
//
//    // Create the animation from the frames
    Animation *animation = Animation::createWithSpriteFrames(frames, 0.6f);
    Animate *animate = Animate::create(animation);
//    loadingAnimation->runAction(animate);
//
    loadingAnimation->runAction(Sequence::create(animate, CallFunc::create(std::bind(&SplashScene::SwitchToMainMenu, this, 0.2f)), NULL));
//
    return true;
}

void SplashScene::SwitchToMainMenu(float dt)
{
    Scene *scene = MainMenuScene::createScene();
    TransitionFade *transition = TransitionFade::create(SCENE_TRANSITION_TIME, scene);
    Director::getInstance()->replaceScene(transition);
}


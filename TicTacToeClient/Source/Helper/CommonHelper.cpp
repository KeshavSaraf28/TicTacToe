#include "CommonHelper.h"

namespace CommonHelper{

Vec2 getScreenCentre()
{
    return Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y);
}
void AddCentredBackground(const std::string &background, ax::Layer *layer)
{
    Sprite *sprite = Sprite::create(background);
    sprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x, Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y));
    layer->addChild(sprite);
}

}

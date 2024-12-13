
#ifndef SplashScene_h
#define SplashScene_h

#pragma once
#include "axmol.h"
#include "Helper/constants.h"
#include "MainMenuScene.h"
#include "Helper/CommonHelper.h"
USING_NS_AX;

class SplashScene : public ax::Layer
{
public:
    static ax::Scene *createScene();
    virtual bool init();
    CREATE_FUNC(SplashScene);

private:
    void SwitchToMainMenu(float dt);
};



#endif

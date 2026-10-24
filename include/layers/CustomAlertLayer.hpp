#pragma once

#include <cocos2d.h>
#include "constants.hpp"
using namespace cocos2d;

// Lots of bullshit in cocos2dx 2.2.3
class CustomAlertLayer : public CCLayerColor
{
public:
    CCLayer* m_mainLayer;
    CCNode* m_scene;

#if GAME_VERSION > GV_1_6
    CC_RGBA_PROTOCOL_VTABLE
#endif

    virtual ~CustomAlertLayer() {}

    static CustomAlertLayer* create();
    virtual bool init();
    virtual void runAnimation();
    virtual void show();

    virtual void onClose()
    {
        this->removeFromParentAndCleanup(true);
    }
    virtual void showFinished(){}

private:
    virtual void registerWithTouchDispatcher()
    {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        dispatcher->addTargetedDelegate(this, 0x80000003, true);
    }

    virtual void keyBackClicked()
    {
        onClose();
    }

    virtual bool ccTouchBegan(CCTouch*, CCEvent*) { return true; }
};
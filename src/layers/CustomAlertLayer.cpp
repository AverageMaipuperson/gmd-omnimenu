#include "CustomAlertLayer.hpp"
#include "AppDelegate.hpp"

CustomAlertLayer* CustomAlertLayer::create()
{
    auto ret = new CustomAlertLayer();
    if (ret->init())
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CustomAlertLayer::init()
{
    CCLayerColor::initWithColor(ccc4(0, 0, 0, 105));
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    auto director = CCDirector::sharedDirector();
    auto dispatcher = director->getTouchDispatcher();

    dispatcher->setForcePrio(true);
    dispatcher->setTargetPrio(0x80000002);

    #if GAME_VERSION < GV_1_7
        m_scene = CCDirector::sharedDirector()->getRunningScene();
    #else
        m_scene = AppDelegate::get()->getScenePointer();
    #endif

    return true;
}

void CustomAlertLayer::runAnimation()
{

}

void CustomAlertLayer::show()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto pivot = CCNode::create();
    pivot->setPosition(ccp(winSize.width/2, winSize.height/2));
    pivot->setScale(.1);
    this->addChild(pivot);

    m_mainLayer->setPosition(ccp(-winSize.width / 2, -winSize.height / 2));
    pivot->addChild(m_mainLayer);

    auto scaleUp = CCScaleTo::create(0.5f, 1.0f);
    auto ease = CCEaseElasticOut::create(scaleUp, 0.5f);
    pivot->runAction(ease);
    
    m_scene->addChild(this, INT_MAX);

    showFinished();
}
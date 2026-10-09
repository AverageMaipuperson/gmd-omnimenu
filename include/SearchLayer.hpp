// TODO: add a global SearchLayer so i dont have to create the same layer multiple times?

#pragma once

#include <cocos2d.h>
#include "CCTextInputNode.hpp"
#include "LevelBrowserLayer.hpp"
#include "ButtonSprite.hpp"
#include "CCMenuItemSpriteExtra.hpp"
#include "VersionUtils.hpp"
#include <algorithm>
#include "MyLevelsLayer.hpp"
#include "SavedLevelsLayer.hpp"

static std::string g_searchFilter = "";
static CCArray* g_filterArr = nullptr;
static CCMenuItemSpriteExtra* g_exitSearchBtn = nullptr;

CCArray* (*GameLevelManager_getSavedLevels)(GameLevelManager*);
#if GAME_VERSION > GV_1_0
CCArray* (*LocalLevelManager_getLocalLevels)(LocalLevelManager*);
#else
CCArray* (*GameLevelManager_getLocalLevels)(GameLevelManager*);
#endif

#if GAME_VERSION > GV_1_3

class SearchLayer : public CCLayerColor, public TextInputDelegate 
{
public:
    LevelBrowserLayer* m_levelBrowserLayer;
    CCTextInputNode* m_input;
    CCLayer* m_mainLayer;
    CCMenu* m_buttonMenu;
    
    static SearchLayer* create(LevelBrowserLayer* lbl)
    {
        auto ret = new SearchLayer();
        if (ret && ret->init(lbl)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setupTouchPriority() {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        
        dispatcher->removeDelegate(this);
        dispatcher->removeDelegate(m_buttonMenu);
        dispatcher->removeDelegate(m_input);

        m_input->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_input, -130, true);
        
        m_buttonMenu->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_buttonMenu, -129, true);
        
        dispatcher->addTargetedDelegate(this, -128, true);
    }

    bool init(LevelBrowserLayer* lbl)
    {
        m_levelBrowserLayer = lbl;
        CCLayerColor::initWithColor({0, 0, 0, 105});
/*
#if GAME_VERSION > GV_1_6
        auto shader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
        if (shader)
            this->setShaderProgram(shader);
        else 
            this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor));
#endif
*/

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setKeypadEnabled(true);
        this->setTouchEnabled(true);

        m_mainLayer = CCLayer::create();
        this->addChild(m_mainLayer);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
        bg->setContentSize(CCSize(280, 160));
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_mainLayer->addChild(bg);

        auto title = CCLabelBMFont::create("Find Level", "goldFont.fnt");
        title->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 55.f));
        title->setScale(0.8f);
        m_mainLayer->addChild(title);

        auto inputBg = extension::CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({200.f, 40.f});
        inputBg->setColor({123,60,31});
        inputBg->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_mainLayer->addChild(inputBg);

        m_input = CCTextInputNode::create(180.f, 30.f, "Search", "Thonburi", 24, "bigFont.fnt");
        m_input->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
        m_input->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");
        m_input->setDelegate(this);
        m_input->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 10));
        m_mainLayer->addChild(m_input);

        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition(CCPointZero /* ccp(0,0) */);
        m_mainLayer->addChild(m_buttonMenu);

        auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSpr,
            closeSpr,
            this,
            menu_selector(SearchLayer::onClose)
        );
        closeBtn->setPosition(ccp(winSize.width / 2 - 130, winSize.height / 2 + 70));
        m_buttonMenu->addChild(closeBtn);

        auto goSpr = ButtonSprite::create("Search", 60, 0, 0.6f, true, "goldFont.fnt", "GJ_button_01.png");
        auto goBtn = CCMenuItemSpriteExtra::create(
            goSpr,
            goSpr,
            this,
            menu_selector(SearchLayer::onSearch)
        );
        goBtn->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 45));
        m_buttonMenu->addChild(goBtn);

        setupTouchPriority();
        return true;
    }

    void onClose(CCObject*)
    {
        this->removeFromParentAndCleanup(true);
    }

    void onSearch(CCObject*)
    {
        std::string inputStr = "";

        auto children = m_input->getChildren();
        if (children)
        {
            for (auto i = 0; i < children->count(); i++)
            {
                auto obj = children->objectAtIndex(i);
                
                if (auto childLabel = dynamic_cast<CCLabelBMFont*>(obj))
                {
                    const char* labelStr = childLabel->getString();
                    if (labelStr && strlen(labelStr) > 0)
                    {
                        inputStr = labelStr;
                        break;
                    }
                }
            }
        }
        std::transform(inputStr.begin(), inputStr.end(), inputStr.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        g_searchFilter = inputStr;

        auto obj = getSearchObject(m_levelBrowserLayer);
        auto arr = CCArray::create();

        if (getSearchType(obj) == 99)
            arr = GameLevelManager_getSavedLevels(GameLevelManager::sharedState());

        else if(getSearchType(obj) == 98)
            arr = LocalLevelManager_getLocalLevels(LocalLevelManager::sharedState());

        auto newArr = CCArray::create();

        for (auto i = 0 ; i < arr->count() ; i++)
        {
            auto lvl = (GJGameLevel*)arr->objectAtIndex(i);
            auto name = lvl->m_sLevelName;
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (name.find(inputStr) != std::string::npos) newArr->addObject(lvl);
        }

        this->m_levelBrowserLayer->setupLevelBrowser(newArr->count() == 0 ? arr : newArr);
        this->m_levelBrowserLayer->loadPage(obj);
        this->m_levelBrowserLayer->updateLevelsLabel();
        onClose(nullptr);
    }

    void show()
    {
#if GAME_VERSION < GV_1_7
        auto scene = CCDirector::sharedDirector()->getRunningScene();
#else
        auto scene = MEMBER_BY_OFFSET(CCScene*, CCDirector::sharedDirector(), 0x98);
#endif
        if (scene) scene->addChild(this, 999);
    }

    virtual void registerWithTouchDispatcher() override {}
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true;
    }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {}
    virtual void keyBackClicked() override {
        onClose(nullptr);
    }
};

#else

class MLLSearchLayer : public CCLayerColor, public TextInputDelegate 
{
public:
    MyLevelsLayer* m_myLevelsLayer;
    CCTextInputNode* m_input;
    CCLayer* m_mainLayer;
    CCMenu* m_buttonMenu;
    
    static MLLSearchLayer* create(MyLevelsLayer* lbl)
    {
        auto ret = new MLLSearchLayer();
        if (ret && ret->init(lbl)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setupTouchPriority() {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        
        dispatcher->removeDelegate(this);
        dispatcher->removeDelegate(m_buttonMenu);
        dispatcher->removeDelegate(m_input);

        m_input->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_input, -130, true);
        
        m_buttonMenu->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_buttonMenu, -129, true);
        
        dispatcher->addTargetedDelegate(this, -128, true);
    }

    bool init(MyLevelsLayer* lbl)
    {
        m_myLevelsLayer = lbl;
        CCLayerColor::initWithColor({0, 0, 0, 105});

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setKeypadEnabled(true);
        this->setTouchEnabled(true);

        m_mainLayer = CCLayer::create();
        this->addChild(m_mainLayer);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
        bg->setContentSize(CCSize(280, 160));
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_mainLayer->addChild(bg);

        auto title = CCLabelBMFont::create("Find Level", "goldFont.fnt");
        title->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 55.f));
        title->setScale(0.8f);
        m_mainLayer->addChild(title);

        auto inputBg = extension::CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({200.f, 40.f});
        inputBg->setColor({123,60,31});
        inputBg->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_mainLayer->addChild(inputBg);

        m_input = CCTextInputNode::create(180.f, 30.f, "Search", "Thonburi", 24, "bigFont.fnt");
        m_input->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
        m_input->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");
        m_input->setDelegate(this);
        m_input->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 10));
        m_mainLayer->addChild(m_input);

        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition(CCPointZero /* ccp(0,0) */);
        m_mainLayer->addChild(m_buttonMenu);

        auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSpr,
            closeSpr,
            this,
            menu_selector(MLLSearchLayer::onClose)
        );
        closeBtn->setPosition(ccp(winSize.width / 2 - 130, winSize.height / 2 + 70));
        m_buttonMenu->addChild(closeBtn);

        auto goSpr = ButtonSprite::create("Search", 60, 0, 0.6f, true, "goldFont.fnt", "GJ_button_01.png");
        auto goBtn = CCMenuItemSpriteExtra::create(
            goSpr,
            goSpr,
            this,
            menu_selector(MLLSearchLayer::onSearch)
        );
        goBtn->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 45));
        m_buttonMenu->addChild(goBtn);

        setupTouchPriority();
        return true;
    }

    void onClose(CCObject*)
    {
        this->removeFromParentAndCleanup(true);
    }

    void onSearch(CCObject*)
    {
        std::string inputStr = "";

        auto children = m_input->getChildren();
        if (children)
        {
            for (auto i = 0; i < children->count(); i++)
            {
                auto obj = children->objectAtIndex(i);
                
                if (auto childLabel = dynamic_cast<CCLabelBMFont*>(obj))
                {
                    const char* labelStr = childLabel->getString();
                    if (labelStr && strlen(labelStr) > 0)
                    {
                        inputStr = labelStr;
                        break;
                    }
                }
            }
        }
        std::transform(inputStr.begin(), inputStr.end(), inputStr.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        g_searchFilter = inputStr;

        auto arr = CCArray::create();
        #if GAME_VERSION > GV_1_0
        arr = LocalLevelManager_getLocalLevels(LocalLevelManager::sharedState());
        #else
        arr = GameLevelManager_getLocalLevels(GameLevelManager::sharedState());
        #endif

        auto newArr = CCArray::create();

        for (auto i = 0 ; i < arr->count() ; i++)
        {
            auto lvl = (GJGameLevel*)arr->objectAtIndex(i);
            auto name = lvl->m_sLevelName;
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (name.find(inputStr) != std::string::npos) newArr->addObject(lvl);
        }
        
        g_filterArr = newArr;

        if (g_exitSearchBtn)
        {
            g_exitSearchBtn->setVisible(true);
            g_exitSearchBtn->setEnabled(true);
        }

        this->m_myLevelsLayer->setupLevelBrowser();
        onClose(nullptr);
    }

    void show()
    {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) scene->addChild(this, 999);
    }

    virtual void registerWithTouchDispatcher() override {}
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true;
    }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {}
    virtual void keyBackClicked() override {
        onClose(nullptr);
    }

};

class SLLSearchLayer : public CCLayerColor, public TextInputDelegate 
{
public:
    SavedLevelsLayer* m_myLevelsLayer;
    CCTextInputNode* m_input;
    CCLayer* m_mainLayer;
    CCMenu* m_buttonMenu;
    
    static SLLSearchLayer* create(SavedLevelsLayer* lbl)
    {
        auto ret = new SLLSearchLayer();
        if (ret && ret->init(lbl)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setupTouchPriority() {
        auto dispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        
        dispatcher->removeDelegate(this);
        dispatcher->removeDelegate(m_buttonMenu);
        dispatcher->removeDelegate(m_input);

        m_input->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_input, -130, true);
        
        m_buttonMenu->setTouchEnabled(true);
        dispatcher->addTargetedDelegate(m_buttonMenu, -129, true);
        
        dispatcher->addTargetedDelegate(this, -128, true);
    }

    bool init(SavedLevelsLayer* lbl)
    {
        m_myLevelsLayer = lbl;
        CCLayerColor::initWithColor({0, 0, 0, 105});

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        this->setKeypadEnabled(true);
        this->setTouchEnabled(true);

        m_mainLayer = CCLayer::create();
        this->addChild(m_mainLayer);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
        bg->setContentSize(CCSize(280, 160));
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_mainLayer->addChild(bg);

        auto title = CCLabelBMFont::create("Find Level", "goldFont.fnt");
        title->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 55.f));
        title->setScale(0.8f);
        m_mainLayer->addChild(title);

        auto inputBg = extension::CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({200.f, 40.f});
        inputBg->setColor({123,60,31});
        inputBg->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_mainLayer->addChild(inputBg);

        m_input = CCTextInputNode::create(180.f, 30.f, "Search", "Thonburi", 24, "bigFont.fnt");
        m_input->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
        m_input->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");
        m_input->setDelegate(this);
        m_input->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 10));
        m_mainLayer->addChild(m_input);

        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition(CCPointZero /* ccp(0,0) */);
        m_mainLayer->addChild(m_buttonMenu);

        auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(
            closeSpr,
            closeSpr,
            this,
            menu_selector(SLLSearchLayer::onClose)
        );
        closeBtn->setPosition(ccp(winSize.width / 2 - 130, winSize.height / 2 + 70));
        m_buttonMenu->addChild(closeBtn);

        auto goSpr = ButtonSprite::create("Search", 60, 0, 0.6f, true, "goldFont.fnt", "GJ_button_01.png");
        auto goBtn = CCMenuItemSpriteExtra::create(
            goSpr,
            goSpr,
            this,
            menu_selector(SLLSearchLayer::onSearch)
        );
        goBtn->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 45));
        m_buttonMenu->addChild(goBtn);

        setupTouchPriority();
        return true;
    }

    void onClose(CCObject*)
    {
        this->removeFromParentAndCleanup(true);
    }

    void onSearch(CCObject*)
    {
        std::string inputStr = "";

        auto children = m_input->getChildren();
        if (children)
        {
            for (auto i = 0; i < children->count(); i++)
            {
                auto obj = children->objectAtIndex(i);
                
                if (auto childLabel = dynamic_cast<CCLabelBMFont*>(obj))
                {
                    const char* labelStr = childLabel->getString();
                    if (labelStr && strlen(labelStr) > 0)
                    {
                        inputStr = labelStr;
                        break;
                    }
                }
            }
        }
        std::transform(inputStr.begin(), inputStr.end(), inputStr.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        g_searchFilter = inputStr;

        auto arr = CCArray::create();
        arr = GameLevelManager_getSavedLevels(GameLevelManager::sharedState());

        auto newArr = CCArray::create();

        for (auto i = 0 ; i < arr->count() ; i++)
        {
            auto lvl = (GJGameLevel*)arr->objectAtIndex(i);
            auto name = lvl->m_sLevelName;
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (name.find(inputStr) != std::string::npos) newArr->addObject(lvl);
        }
        
        g_filterArr = newArr;

        if (g_exitSearchBtn)
        {
            g_exitSearchBtn->setVisible(true);
            g_exitSearchBtn->setEnabled(true);
        }

        this->m_myLevelsLayer->setupLevelBrowser();
        onClose(nullptr);
    }

    void show()
    {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) scene->addChild(this, 999);
    }

    virtual void registerWithTouchDispatcher() override {}
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        return true;
    }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {}
    virtual void keyBackClicked() override {
        onClose(nullptr);
    }

};
#endif
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
#include "CustomAlertLayer.hpp"
#include "AppDelegate.hpp"

static std::string g_searchFilter = "";
static CCArray* g_filterArr = nullptr;
static CCMenuItemSpriteExtra* g_exitSearchBtn = nullptr;

CCArray* (*GameLevelManager_getSavedLevels)(GameLevelManager*);
#if GAME_VERSION > GV_1_0
CCArray* (*LocalLevelManager_getLocalLevels)(LocalLevelManager*);
#else
CCArray* (*GameLevelManager_getLocalLevels)(GameLevelManager*);
#endif

class SearchLayer : public CustomAlertLayer, public TextInputDelegate
{
public:
    LevelBrowserLayer* m_levelBrowserLayer;
    CCTextInputNode* m_input;
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

    bool init(LevelBrowserLayer* lbl)
    {
        m_levelBrowserLayer = lbl;
        CustomAlertLayer::init();

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_mainLayer = CCLayer::create();

        auto background = CCSprite::create("square02_001.png");
        background->setScale(100);
        background->setOpacity(105);
        m_mainLayer->addChild(background, 10000);

        auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
        bg->setContentSize(CCSize(280, 160));
        bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_mainLayer->addChild(bg, 10001);

        auto title = CCLabelBMFont::create("Find Level", "goldFont.fnt");
        title->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 55.f));
        title->setScale(0.8f);
        m_mainLayer->addChild(title, 10002);

        auto inputBg = extension::CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
        inputBg->setContentSize({200.f, 40.f});
        inputBg->setColor({123,60,31});
        inputBg->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
        m_mainLayer->addChild(inputBg, 10001);

        m_input = CCTextInputNode::create(180.f, 30.f, "Search", "Thonburi", 24, "bigFont.fnt");
        m_input->setLabelPlaceholderColor({0x75, 0xAA, 0xF0});
        m_input->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");
        m_input->setDelegate(this);
        m_input->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 10));
        m_mainLayer->addChild(m_input, 10002);

        m_buttonMenu = CCMenu::create();
        m_buttonMenu->setPosition(CCPointZero);
        m_mainLayer->addChild(m_buttonMenu, 10002);

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

        return true;
    }

#if GAME_VERSION > GV_1_3
    virtual void onSearch(CCObject*)
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
        onClose();
    }
#else 
    virtual void onSearch(CCObject*){}
#endif
};

class MLLSearchLayer : public SearchLayer
{
public:
    MyLevelsLayer* m_myLevelsLayer;
    
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

    bool init(MyLevelsLayer* lbl)
    {
        m_myLevelsLayer = lbl;
        return SearchLayer::init(nullptr);
    }

    virtual void onSearch(CCObject*) override
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
        onClose();
    }
};

class SLLSearchLayer : public SearchLayer
{
public:
    SavedLevelsLayer* m_myLevelsLayer;
    
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

    bool init(SavedLevelsLayer* lbl)
    {
        m_myLevelsLayer = lbl;
        return SearchLayer::init(nullptr);
    }

    virtual void onSearch(CCObject*) override
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
        onClose();
    }
};
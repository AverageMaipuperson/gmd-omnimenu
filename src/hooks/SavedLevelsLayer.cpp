#include "hook.hpp"
#include "GJGameLevel.hpp"
#include "SavedLevelsLayer.hpp"
#include "EditLevelLayer.hpp"
#include "SearchLayer.hpp"

bool (*SavedLevelsLayer_init)(MyLevelsLayer* self);
bool SavedLevelsLayer_init_H(MyLevelsLayer* self) {
    if (!SavedLevelsLayer_init(self)) return false;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    if (hax.getModuleEnabled(ModuleID::PAGE_CONTROLS))
    {
        auto menu = CCMenu::create();
        menu->setPosition(ccp(0,0));
        self->addChild(menu);

        auto spr = CCSprite::create("gj_findBtn_001.png");
        auto btn = CCMenuItemSpriteExtra::create(
            spr,
            spr,
            self,
            menu_selector(SavedLevelsLayer::onSearch)
        );
        btn->setPosition(ccp(25, winSize.height - 65));
        menu->addChild(btn);

        spr = CCSprite::create("gj_findBtnOff_001.png");
        btn = CCMenuItemSpriteExtra::create(
            spr,
            spr,
            self,
            menu_selector(SavedLevelsLayer::exitSearch)
        );
        btn->setPosition(ccp(25, winSize.height - 105));
        btn->setVisible(false);
        btn->setEnabled(false);
        g_exitSearchBtn = btn;
        menu->addChild(btn);
    }

    return true;
}

void SavedLevelsLayer::onSearch(CCObject*)
{
    SLLSearchLayer::create(this)->show();
}

void SavedLevelsLayer::exitSearch()
{
    g_searchFilter = "";
    g_filterArr = nullptr;
    this->setupLevelBrowser();

    g_exitSearchBtn->setVisible(false);
    g_exitSearchBtn->setEnabled(false);
}

void (*SavedLevelsLayer_destructor)(MyLevelsLayer*);
void SavedLevelsLayer_destructor_H(MyLevelsLayer* self)
{
    SavedLevelsLayer_destructor(self);
    g_searchFilter = "";
    g_filterArr = nullptr;
}

void SavedLevelsLayer_om() {
    Omni::hook("_ZN16SavedLevelsLayer4initEv",
        reinterpret_cast<void*>(SavedLevelsLayer_init_H),
        reinterpret_cast<void**>(&SavedLevelsLayer_init));
    Omni::hook("_ZN16SavedLevelsLayerD1Ev",
        reinterpret_cast<void*>(SavedLevelsLayer_destructor_H),
        reinterpret_cast<void**>(&SavedLevelsLayer_destructor));
}
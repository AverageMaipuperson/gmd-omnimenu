#include "hook.hpp"
#include "EditorPauseLayer.hpp"
#include "ButtonSprite.hpp"

LevelEditorLayer* g_lel = nullptr;

void EditorPauseLayer::onOpenMenu() {
    auto haxOverlay = HaxMenu::create(this);
    this->addChild(haxOverlay, 1000);
    this->setTouchEnabled(false);
}

bool (*TRAM_EditorPauseLayer_init)(EditorPauseLayer* self, LevelEditorLayer* editLayer);
bool EditorPauseLayer_init(EditorPauseLayer* self, LevelEditorLayer* editLayer) {
    if (!TRAM_EditorPauseLayer_init(self, editLayer)) return false;
    g_lel = editLayer;
    HaxManager& hax = HaxManager::sharedState();
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    if (hax.getModuleEnabled(ModuleID::HIDE_ICON_ON_EDITOR) && hax.getModuleEnabled(ModuleID::FLOATING_ICON) && hax.omniMenu) {
        hax.omniMenu->m_sMenu->setVisible(true);
        hax.omniMenu->m_sMenu->setEnabled(true);
    }
#if GAME_VERSION >= GV_1_6
#if GAME_VERSION < GV_1_8
    CCObject* obj = self->getChildren()->objectAtIndex(2);
#else
    CCObject* obj = self->getChildren()->objectAtIndex(5);
#endif
    auto label = dynamic_cast<CCLabelBMFont*>(obj);
    if (label) {
        if (hax.getModuleEnabled(ModuleID::OBJECT_LIMIT_BYPASS)) {
            int objectCount = getObjectCount(editLayer);
            label->setString(fmt::format("{} objects", objectCount).c_str(), "goldFont.fnt");
        }
    }
#endif
    if (hax.getModuleEnabled(ModuleID::OBJECT_COUNTER)) {
        bool objHack = hax.getModuleEnabled(ModuleID::OBJECT_LIMIT_BYPASS);
        auto objectLimit = OBJECT_LIMIT + 1;
        if (objHack) objectLimit = INCREASED_OBJECT_LIMIT;
        int objectCount = getObjectCount(editLayer);
    
        CCLabelBMFont* counterLabel;

        fmt::internal::MemoryBuffer<char, 500> buf;
        fmt::BasicWriter<char> writer(buf);

        writer.write("{}", objectCount);
#if GAME_VERSION >= GV_1_5
        if (!objHack) 
#else
        if (!objHack || !hax.getModuleEnabled(ModuleID::_16K_FIX)) 
#endif
            writer.write("/{}", objectLimit);

        float maxX = 0;
        
        if (editLayer->getSectionCount() > 0 && getObjectCount(editLayer) > 0) {
            CCArray* lastSection;
            for (int j = editLayer->getSectionCount() - 1; j > 0; j--) {
                lastSection = static_cast<CCArray*>(getEditorSections(editLayer)->objectAtIndex(j));
                if (lastSection && lastSection->count() > 0) break;
            }
            if (lastSection && lastSection->count() > 0) {
                for (int i = 0; i < lastSection->count(); i++) {
                    auto obj = static_cast<GameObject*>(lastSection->objectAtIndex(i));
                    if (obj->getPosition().x > maxX) {
                        maxX = obj->getPosition().x;
                    }
                }
            }
        }

#if GAME_VERSION < GV_1_7
        // This should be 311.58, but editor code uses 311 flat, and dropping the .58 only causes 6-7 seconds of discrepancy on an hour long level
        int dist = floorf((maxX + 340) / 311.0f);
#else
        auto speeds = getSpeedObjects(getGridLayer(editLayer));
        auto speedsSorted = CCArray::create();
        speedsSorted->initWithArray(speeds);

        // sort by x position
        std::sort(speedsSorted->data->arr, speedsSorted->data->arr + speedsSorted->data->num, compareXes2);

        float mult = 0.9;
        switch (getStartSpeed(getEditorSettingsObject(editLayer))) {
            case 1: mult = 0.7; break;
            case 2: mult = 1.1; break;
            case 3: mult = 1.3; break;
        }
        float time = 340 / (346.2 * mult);
        float lastSpeedX = 0;
        for (int i = 0; i < speedsSorted->count(); i++) {
            auto obj = static_cast<GameObject*>(speedsSorted->objectAtIndex(i));
            time += (obj->getPositionX() - lastSpeedX) / (346.2 * mult);
            lastSpeedX = obj->getPositionX();
            switch (getObjectKey(obj)) {
                case 200: mult = 0.7; break;
                case 201: mult = 0.9; break;
                case 202: mult = 1.1; break;
                case 203: mult = 1.3; break;
            }
        }
        time += (maxX - lastSpeedX) / (346.2 * mult);
        int dist = floorf(time);
#endif
        int seconds = dist % 60;
        int minutes = dist / 60;

        writer.write(" objects\nLength: ");
        if (minutes > 0) {
#if GDPS == GDPS_NEOPOINTFOUR
            writer.write("{}min {}s ({})", minutes, seconds,
                GJGameLevel::lengthKeyToString(GJGameLevel::getLengthKey(maxX)));
#else
            writer.write("{}min {}s (Long)", minutes, seconds);
#endif
        } else {
            writer.write("{} second", seconds);
            if (seconds != 1) writer.write("s");
            writer.write(" ({})",
                GJGameLevel::lengthKeyToString(GJGameLevel::getLengthKey(maxX)));
        }
        
#if GAME_VERSION < GV_1_6
        counterLabel = CCLabelBMFont::create(writer.c_str(), "goldFont.fnt");

        counterLabel->setScale(0.5f);
        counterLabel->setAnchorPoint({0.f, 0.5f});
        counterLabel->setPosition(ccp(10, winSize.height - 20));
        self->addChild(counterLabel, 1000);
#else
        label->setString(writer.c_str());
#endif
    }
    CCMenu* btnMenu = CCMenu::create();
    self->addChild(btnMenu, 999);
    btnMenu->setPosition(ccp(0, winSize.height));

    CCSprite* menuSpr = CCSprite::create("OMNImenu_btn.png");
    CCMenuItemSpriteExtra* menuBtn = CCMenuItemSpriteExtra::create(menuSpr, menuSpr, self, menu_selector(PauseLayer::onOpenMenu));
    if(!hax.getModuleEnabled(ModuleID::FLOATING_ICON)) btnMenu->addChild(menuBtn, 999);
    menuBtn->setPosition(ccp(winSize.width - 50.f, -50.f));
#ifdef STEALTH_MODE
    menuBtn->setVisible(false);
#endif

    if (hax.getModuleEnabled(ModuleID::SAVE_LEVEL))
    {
        auto menu = (CCMenu*)self->getChildren()->objectAtIndex(0);
        auto exitBtn = (CCMenuItemSpriteExtra*)menu->getChildren()->lastObject();
        exitBtn->removeFromParentAndCleanup(true);

        auto spr = ButtonSprite::create("Save", 0, 220, 1, false);
        auto btn = (CCMenuItemSpriteExtra*)CCMenuItemExt::createWithSpriteExtra(
            spr,
            [self](CCObject*)
            {
                struct Delegate : public FLAlertLayerProtocol
                {
                    EditorPauseLayer* m_layer = nullptr;

                    void FLAlert_Clicked(FLAlertLayer*, bool btn2)
                    {
                        if (btn2) m_layer->saveLevel();
                        delete this;
                    }
                };

                auto delegate = new Delegate();
                delegate->m_layer = self;

                auto alert = FLAlertLayer::create(
                    delegate,
                    "Save",
                    "Do you want to <cy>save</c> the level?",
                    "NO",
                    "YES",
                    300.f
                );
                alert->setTag(2);
                alert->show();
            }
        );

        menu->addChild(btn);

        spr = ButtonSprite::create("Exit", 0, 220, 1, false);
        btn = (CCMenuItemSpriteExtra*)CCMenuItemExt::createWithSpriteExtra(
            spr,
            [self](CCObject*)
            {
                #if GAME_VERSION < GV_1_7
                self->onExitNoSave();
                #else
                self->onExitNoSave(nullptr);
                #endif
            }
        );

        menu->addChild(btn);
        menu->alignItemsVerticallyWithPadding(10);
    }

    return true;
}

void (*TRAM_EditorPauseLayer_saveLevel)(EditorPauseLayer* self);
void EditorPauseLayer_saveLevel(EditorPauseLayer* self) {
    HaxManager& hax = HaxManager::sharedState();
    TRAM_EditorPauseLayer_saveLevel(self);
    if (hax.getModuleEnabled(ModuleID::AUTO_BACKUP)) {
        hax.createBackup();
        hax.saveSettingsToFile();
    }
}

void (*EditorPauseLayer_onResume)(EditorPauseLayer* self SEL_MenuHandler_1_7_compat2);
void EditorPauseLayer_onResume_H(EditorPauseLayer* self SEL_MenuHandler_1_7_compat2) {
    auto& hax = HaxManager::sharedState();
    EditorPauseLayer_onResume(self sender_param_1_7);
    if (hax.getModuleEnabled(ModuleID::HIDE_ICON_ON_EDITOR) && hax.getModuleEnabled(ModuleID::FLOATING_ICON) && hax.omniMenu) {
        hax.omniMenu->m_sMenu->setVisible(false);
        hax.omniMenu->m_sMenu->setEnabled(false);
    }
}

void (*EditorPauseLayer_FLAlert_Clicked)(EditorPauseLayer* self, FLAlertLayer* alert, bool btn2);
void EditorPauseLayer_FLAlert_Clicked_H(EditorPauseLayer* self, FLAlertLayer* alert, bool btn2)
{
    // CCLog("%i", alert->getTag());
    if (btn2)
    {
        EditorPauseLayer_FLAlert_Clicked(self, alert, true);
        #if GAME_VERSION < GV_1_7
        if (alert->getTag() == 1)
        self->onExitEditor();
        else self->saveLevel();
        #else
        if (alert->getTag() == 2)
        self->saveLevel();
        else
        {
            auto gman = GameManager::sharedState();
            gman->returnToLastScene(g_lel->getLevel());
        }
        #endif
    }
}

void EditorPauseLayer_om() {
    Omni::hook("_ZN16EditorPauseLayer4initEP16LevelEditorLayer",
        reinterpret_cast<void*>(EditorPauseLayer_init),
        reinterpret_cast<void**>(&TRAM_EditorPauseLayer_init));
    Omni::hook("_ZN16EditorPauseLayer9saveLevelEv",
        reinterpret_cast<void*>(EditorPauseLayer_saveLevel),
        reinterpret_cast<void**>(&TRAM_EditorPauseLayer_saveLevel));
#if GAME_VERSION < GV_1_7
    Omni::hook("_ZN16EditorPauseLayer8onResumeEv",
        reinterpret_cast<void*>(EditorPauseLayer_onResume_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onResume));
#else
    Omni::hook("_ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE",
        reinterpret_cast<void*>(EditorPauseLayer_onResume_H),
        reinterpret_cast<void**>(&EditorPauseLayer_onResume));
#endif
    Omni::hook("_ZN16EditorPauseLayer15FLAlert_ClickedEP12FLAlertLayerb",
        reinterpret_cast<void*>(EditorPauseLayer_FLAlert_Clicked_H),
        reinterpret_cast<void**>(&EditorPauseLayer_FLAlert_Clicked));
}
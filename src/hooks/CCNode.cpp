#include "hook.hpp"
#include "LevelEditorLayer.hpp"

void (*TRAM_CCNode_setVisible)(CCNode* self, bool toggle);
void CCNode_setVisible(CCNode* self, bool toggle) {
    HaxManager& hax = HaxManager::sharedState();
    if (hax.getModuleEnabled(ModuleID::FORCE_VISIBILITY)) {
        TRAM_CCNode_setVisible(self, true);
    } else TRAM_CCNode_setVisible(self, toggle);
}

void (*CCNode_scheduleUpdate)(CCNode*);
void CCNode_scheduleUpdate_H(CCNode* self)
{
    if (self == LevelEditorLayer::get()) 
        return;
    
    CCNode_scheduleUpdate(self);
}

void (*CCNode_unscheduleUpdate)(CCNode*);
void CCNode_unscheduleUpdate_H(CCNode* self)
{
    if (self == LevelEditorLayer::get()) 
        return;
    
    CCNode_unscheduleUpdate(self);
}

void CCNode_om() {
    Omni::hook("_ZN7cocos2d6CCNode10setVisibleEb",
        reinterpret_cast<void*>(CCNode_setVisible),
        reinterpret_cast<void**>(&TRAM_CCNode_setVisible));
/*
    Omni::hook("_ZN7cocos2d6CCNode14scheduleUpdateEv",
        reinterpret_cast<void*>(CCNode_scheduleUpdate_H),
        reinterpret_cast<void**>(&CCNode_scheduleUpdate));

    Omni::hook("_ZN7cocos2d6CCNode16unscheduleUpdateEv",
        reinterpret_cast<void*>(CCNode_unscheduleUpdate_H),
        reinterpret_cast<void**>(&CCNode_unscheduleUpdate));
*/
}
#pragma once

#include <cocos2d.h>
#include "LevelEditorLayer.hpp"
#include "FLAlertLayer.hpp"

class EditorPauseLayer : public cocos2d::CCLayer, public FLAlertLayerProtocol {
public:
    static EditorPauseLayer* create(LevelEditorLayer*);
    void onOpenMenu();
    #if GAME_VERSION < GV_1_7
    void onExitEditor();
    void onExitNoSave();
    #else
    void onExitEditor(CCObject*);
    void onExitNoSave(CCObject*);
    #endif
    void saveLevel();
};

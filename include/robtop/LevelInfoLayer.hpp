#pragma once

#include <cocos2d.h>
#include "GJGameLevel.hpp"

class LevelInfoLayer : public cocos2d::CCLayer {
public:
    static LevelInfoLayer* create(GJGameLevel*);
    bool init(GJGameLevel*);
    void onClone();

    void onViewLevelInfo();
    void onExport();
    void onComment();
    // secret robtop layers :eyes:
    void onStars(); // >1.3
    #if GAME_VERSION < GV_1_7
    void onFeatured(); // >1.4
    #else
    void onFeatured(CCObject*); // >1.4
    #endif

    void onTrack(CCObject*);
};

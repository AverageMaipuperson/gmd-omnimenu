#pragma once

#include <cocos2d.h>
// #include "PlayLayer.hpp"

class AchievementManager : public cocos2d::CCNode {
public:
    static AchievementManager* sharedState();

    void notifyAchievementWithID(const char* ach);
    #if GAME_VERSION > GV_1_2
    void reportAchievementWithID(const char* ach, int, bool);
    #else
    void reportAchievementWithID(const char* ach, int);
    #endif
};

class AchievementNotifier : public cocos2d::CCNode {
public:
    static AchievementNotifier* sharedState();

    void notifyAchievement(const char*, const char*, const char*);
};
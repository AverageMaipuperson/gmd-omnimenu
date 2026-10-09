#pragma once

#include <cocos2d.h>
#include "GameObject.hpp"
using namespace cocos2d;

enum GhostType {
    None = 0,
    Ghost = 1
};

class PlayerObject : public GameObject {
public:
    static PlayerObject* create(int, cocos2d::CCLayer*);
    bool init(void*);
    void lockPlayer();
    void gravityDown();
    void gravityUp();
    void speedDown();
    void speedUp();
    void yStartDown();
    void yStartUp();
    void logValues();

    void deactivateParticle();
    void flipMod();
    void stopRotation();
    void toggleGhostEffect(GhostType type);
    void touchedObject(GameObject* object);
    void setColor(_ccColor3B const&);
    void setSecondColor(_ccColor3B const&);
    void updatePlayerFrame(int);
#if GAME_VERSION > GV_1_0
    void updatePlayerRollFrame(int id);
    bool getRollMode() const;
#endif
#if GAME_VERSION > GV_1_3
    void updatePlayerShipFrame(int id);
    void updatePlayerScale(); // exists since 1.5, but reimplemented by OMNImenu in 1.4 for Ship Gravity Bug Fix
    void togglePlayerScale(bool);
#endif

    void flipGravity(bool);
    void toggleFlyMode(bool);
    void spawnPortalCircle(_ccColor3B, float);

#if GAME_VERSION > GV_1_0
    void toggleRollMode(bool);
#endif

#if GAME_VERSION > GV_1_4
    void toggleBirdMode(bool);
    bool getBirdMode() const;
    void updatePlayerBirdFrame(int id);
#endif

#if GAME_VERSION > GV_1_6
    void updateTimeMod(float);
#endif
};

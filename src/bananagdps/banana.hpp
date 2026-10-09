#include "cocos2d.h"

static cocos2d::CCSprite* g_banana = nullptr;
static cocos2d::CCNode* g_bananaContainer = nullptr;
static cocos2d::CCMenu* g_bananaMenu = nullptr;

bool assert_texture_sizes(CCSprite* spr1, CCSprite* spr2)
{
    auto rect1 = spr1->getTextureRect();
    auto rect2 = spr2->getTextureRect();

    return (rect1.origin.x == rect2.origin.x &&
            rect1.origin.y == rect2.origin.y &&
            rect1.size.width == rect2.size.width &&
            rect1.size.height == rect2.size.height);
}
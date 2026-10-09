#include "InfoLayer.hpp"
#include "GJGameLevel.hpp"
#include "ButtonSprite.hpp"
#include "CCMenuItemExt.h"
#include "hook.hpp"

bool (*InfoLayer_init)(InfoLayer*, GJGameLevel*);
bool InfoLayer_init_H(InfoLayer* self, GJGameLevel* lvl)
{
    InfoLayer_init(self, lvl);
    auto spr = ButtonSprite::create(CCSprite::create("GJ_sLikeIcon_001.pngs"), 0, 0, 1, 1, false, "GJ_button_01.png");
    auto spr2 = ButtonSprite::create(CCSprite::create("GJ_sLikeIcon_001.pngs"), 0, 0, 1, 1, false, "GJ_button_02.png");

    auto btn = CCMenuItemExt::createWithToggler(
        spr,
        spr2,
        [self](CCObject*)
        {

        }
    );
    return true;
}

void InfoLayer_om()
{
    Omni::hook("_ZN9InfoLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(InfoLayer_init_H),
        reinterpret_cast<void**>(&InfoLayer_init));
}
#include "hook.hpp"
#include "MenuLayer.hpp"
#include "GJGameLevel.hpp"

void (*t_CCSprite_setColor)(CCSprite*, _ccColor3B const&);
void CCSprite_setColor(CCSprite* spr, _ccColor3B const& color)
{
    auto texture = CCTextureCache::sharedTextureCache()->textureForKey("GJ_gradientBG.png");
    if (spr->getTexture() != texture) t_CCSprite_setColor(spr, color);
}

void (*t_OptionsLayer_customSetup)(CCLayer*);
void OptionsLayer_customSetup(CCLayer* self)
{
    t_OptionsLayer_customSetup(self);
    auto menu = MEMBER_BY_OFFSET(CCMenu*, self, 0x208);
    menu->getChildren()->removeObjectAtIndex(0);
    menu->getChildren()->removeObjectAtIndex(0);
    menu->alignItemsHorizontallyWithPadding(10);
    menu->setPosition(ccp(menu->getPositionX(), menu->getPositionY() - 40));
}

void banana_om()
{
    Omni::hook(
        "_ZN7cocos2d8CCSprite8setColorERKNS_10_ccColor3BE",
        reinterpret_cast<void*>(CCSprite_setColor),
        reinterpret_cast<void**>(&t_CCSprite_setColor));

    Omni::hook(
        "_ZN12OptionsLayer11customSetupEv",
        reinterpret_cast<void*>(OptionsLayer_customSetup),
        reinterpret_cast<void**>(&t_OptionsLayer_customSetup));

    /* Omni::hook(
        "_ZN14LevelInfoLayer4initEP11GJGameLevel",
        reinterpret_cast<void*>(LevelInfoLayer_init),
        reinterpret_cast<void**>(&t_LevelInfoLayer_init));

    Omni::hook(
        "_ZN9MenuLayer4initEv",
        reinterpret_cast<void*>(MenuLayer_init),
        reinterpret_cast<void**>(&t_MenuLayer_init)); */
}
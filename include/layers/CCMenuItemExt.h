#ifndef __CCMENUITEMEXT_H__
#define __CCMENUITEMEXT_H__

#include <cocos2d.h>
#include <functional>
#include "CCMenuItemToggler.hpp"
#include "CCMenuItemSpriteExtra.hpp"

class CCMenuItemExt : public CCNode {
public:
    using Callback = std::function<void(CCObject*)>;

    static CCMenuItem* create(CCMenuItem* wrapper, const Callback& callback);
    static CCMenuItem* createWithSprite(CCNode*, CCNode*, CCNode*, const Callback&);
    static CCMenuItem* createWithSprite(CCNode*, CCNode*, const Callback&);
    static CCMenuItem* createWithSprite(CCNode*, const Callback&);
    static CCMenuItem* createWithLabel(CCNode*, const Callback&);
    static CCMenuItem* createWithSpriteExtra(CCNode*, CCNode*, const Callback&);
    static CCMenuItem* createWithSpriteExtra(CCNode*, const Callback&);
    static CCMenuItem* createWithToggler(CCNode*, CCNode*, const Callback&);

    void callback(CCObject*);

private:
    Callback _callback = nullptr;
};

#endif

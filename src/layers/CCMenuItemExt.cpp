#include "CCMenuItemExt.h"

CCMenuItem* CCMenuItemExt::create(CCMenuItem* wrapper, const Callback& callback)
{
    auto listener = new CCMenuItemExt();
    listener->_callback = callback;

    wrapper->setTarget(listener, menu_selector(CCMenuItemExt::callback));
    wrapper->addChild(listener); 
    listener->release(); 

    return wrapper;
}

CCMenuItem* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, CCNode* disabled, const Callback& callback)
{
    return create(CCMenuItemSprite::create(normal, selected, disabled), callback);
}

CCMenuItem* CCMenuItemExt::createWithSprite(CCNode* normal, CCNode* selected, const Callback& callback)
{
    return createWithSprite(normal, selected, nullptr, callback);
}

CCMenuItem* CCMenuItemExt::createWithSprite(CCNode* normal, const Callback& callback)
{
    return createWithSprite(normal, nullptr, nullptr, callback);
}

CCMenuItem* CCMenuItemExt::createWithLabel(CCNode* label, const Callback& callback)
{
    return create(CCMenuItemLabel::create(label), callback);
}

CCMenuItem* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, CCNode* selected, const Callback& callback)
{
    auto item = CCMenuItemSpriteExtra::create(normal, selected, nullptr, nullptr);
    return create(item, callback);
}

CCMenuItem* CCMenuItemExt::createWithSpriteExtra(CCNode* normal, const Callback& callback)
{
    return createWithSpriteExtra(normal, nullptr, callback);
}

CCMenuItem* CCMenuItemExt::createWithToggler(CCNode* off, CCNode* on, const Callback& callback)
{
    auto item = CCMenuItemToggler::create(off, on, nullptr, nullptr);
    return create(item, callback);
}

void CCMenuItemExt::callback(CCObject* sender)
{
    if (_callback) _callback(sender);
}

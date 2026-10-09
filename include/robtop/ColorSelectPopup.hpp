#pragma once

#include <cocos2d.h>
#include "GameObject.hpp"
#include "FLAlertLayer.hpp"
#include "CCTextInputNode.hpp"

class ColorSelectPopup : public FLAlertLayer {
public:
    static ColorSelectPopup* create(GameObject* object);
    bool init(GameObject* object);
    _ccColor3B getColorValue();
    void selectColor(_ccColor3B color);

    void onCopy();
    void onPaste();
    void updateDurLabel();
};

class ColorPickerPopup;
class ColorPickerDelegate {
public:
    virtual ColorPickerPopup* colorPickerClosed(ColorPickerPopup*);
};

class ColorPickerPopup : public FLAlertLayer {
public:
    #if GAME_VERSION < GV_1_7
    static ColorPickerPopup* create();
    #else
    static ColorPickerPopup* create(int, int, int);
    #endif
    bool init();
    _ccColor3B getColorValue();
    void selectColor(_ccColor3B color);
    void setDelegate(ColorPickerDelegate*);

    void onCopy();
    void onPaste();
};
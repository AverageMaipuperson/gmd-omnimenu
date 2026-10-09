#pragma once

#include "constants.hpp"
#include <cocos2d.h>
#include <Module.hpp>
#include "CCMenuItemSpriteExtra.hpp"
#include "../layers/CustomScrollLayer.hpp"
#include "FLAlertLayer.hpp"
#if GAME_VERSION >= GV_1_7
#include "cocos2d-2.2.3/cocos2dx/layers_scenes_transitions_nodes/CCLayer.h"
#endif
#include "CCMenuItemExt.h"
#include "ColorSelectPopup.hpp"

#define CHECKBOX_PREFIX 100000
#define LABEL_PREFIX 200000
#define INFO_PREFIX 300000

// #if GAME_VERSION < GV_1_7
// #else
// class HaxMenu : public CCLayer
// #endif


class Branch : public CCNode {
public:
    struct Segment {
        CCPoint start;
        CCPoint end;
    };

    std::vector<Segment> lines;
    bool m_show = false;

    static Branch* create() {
        auto ret = new Branch();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init()
    {
        return true;
    }

    virtual void draw() override {
        CCNode::draw();
        if (!this->isVisible() || lines.empty()) return;

        ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        
        ccDrawColor4B(100, 100, 100, 150); 
        glLineWidth(2.0f);

        for (const auto& line : lines)
        {
            ccDrawLine(line.start, line.end);
        }
    }
};

class HaxMenu : public CCLayerColor, public ColorPickerDelegate, public TextInputDelegate, public FLAlertLayerProtocol
{
public:
    static HaxMenu* create(CCLayer* referrer);
    static cocos2d::CCScene* scene();

protected:
    bool init(CCLayer* referrer);
    void onClose(CCObject*);
    void keyBackClicked();

    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void addButton(const char* label, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector);
    // void addButtonRight(const char* label, float fontSize, float yOffset, CCObject* target, SEL_MenuHandler selector);
    void onCategory(ModuleCategory category);
    void onPlayer(SEL_MenuHandler_1_7_compat);
    void onVisual(SEL_MenuHandler_1_7_compat);
    void onEditor(SEL_MenuHandler_1_7_compat);
    void onBypass(SEL_MenuHandler_1_7_compat);
    void onInformational(SEL_MenuHandler_1_7_compat);
    void onUniversal(SEL_MenuHandler_1_7_compat);
    void onLabel(SEL_MenuHandler_1_7_compat);
    void onParticles(SEL_MenuHandler_1_7_compat);
    void onSaveData(SEL_MenuHandler_1_7_compat);

    void cleanupMembers();

    void toggler(CCObject*);
    void modInfo(CCObject*);
    float getDuration();
    void setColorAtIndex(int index);
    void onPih(CCObject*);
    void onUDID();
    void onThree(CCObject*);
    void onExportData(SEL_MenuHandler_1_7_compat);
    void onImportData(CCObject* sender);
    void toggleDropdown(ModuleSubcategory);
    void drawBranches(Module& owner, const std::vector<Module*>& modules, bool visible);
    void redrawBranches();
    void moveDown(Module& pivot, bool down);
    void setNoclipTintColor(_ccColor3B color);
    CCTextInputNode* createTheInput(Module&, CCPoint);
    
    virtual ColorPickerPopup* colorPickerClosed(ColorPickerPopup*);
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    CCNode* leftParent;
    CCNode* rightParent;
    CCLayer* referrer;
    CCArray* catButtons;
    CCMenu* catMenu;
    CCMenu* modMenu;
    CCMenuItemSpriteExtra* udidBtn;
    CCMenuItemSpriteExtra* crashBtn;
    CCSprite* leftPanel;
    CCSprite* rightPanel;
    bool canSpoofPih;
    std::map<ModuleSubcategory, Branch*> branches;
    std::map<ModuleSubcategory, float> yPos;
    ColorPickerPopup* popup;
};
#pragma once

#include <cocos2d.h>
#include "FLAlertLayer.hpp"

class SongObject;
class SongInfoLayer : public FLAlertLayer
{
public:
    static SongInfoLayer* create(SongObject*);
};

class SongObject : public CCObject {
public:
    static SongObject* create(int);
};
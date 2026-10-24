#include <cocos2d.h>
using namespace cocos2d;

// robtop class
class CCCircleWave : public CCNode, public CCActionTweenDelegate
{
public:

    static CCCircleWave* create(float, float, float, bool, bool);
    static CCCircleWave* create(float, float, float, bool);
};
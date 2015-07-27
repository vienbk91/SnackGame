#ifndef __Pause_SCENE_H__
#define __Pause_SCENE_H__

#include "cocos2d.h"

class Pause : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();    
	void exitPause(cocos2d::Ref* pSender);
    CREATE_FUNC(Pause);
private:
	cocos2d::Sprite* sprLogo;
	cocos2d::Director *director;
	cocos2d::Size visibleSize;	
};

#endif // __Pause_SCENE_H__

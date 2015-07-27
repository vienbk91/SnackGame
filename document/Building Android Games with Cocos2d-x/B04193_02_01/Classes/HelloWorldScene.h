#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PauseScene.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void pauseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);
	void moveFinished(cocos2d::Node* sender);
private:
	cocos2d::Director *_director;
	cocos2d::Size _visibleSize;	
};

#endif // __HELLOWORLD_SCENE_H__

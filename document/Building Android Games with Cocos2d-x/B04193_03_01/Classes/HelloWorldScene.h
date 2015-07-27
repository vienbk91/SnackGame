#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void pauseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Director *_director;
	cocos2d::Size _visibleSize;	
	cocos2d::Sprite* _sprBomb;
	void initPhysics();
	bool onCollision(cocos2d::PhysicsContact& contact);
	void setPhysicsBody(cocos2d::Sprite* sprite);
};

#endif // __HELLOWORLD_SCENE_H__

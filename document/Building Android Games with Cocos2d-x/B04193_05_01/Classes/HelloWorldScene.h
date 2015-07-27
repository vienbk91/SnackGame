#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PauseScene.h"
#include "GameOverScene.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void pauseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Director *_director;
	cocos2d::Size visibleSize;	
	cocos2d::Sprite* _sprBomb;
	cocos2d::Sprite* _sprPlayer;	
	int _score;
	void initPhysics();
	bool onCollision(cocos2d::PhysicsContact& contact);
	void setPhysicsBody(cocos2d::Sprite* sprite);
	void initTouch();
	void movePlayerByTouch(cocos2d::Touch* touch, cocos2d::Event* event);
	void movePlayerIfPossible(float newX);
	void movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event);
	void initAccelerometer();
	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void updateScore(float dt);
	void addBombs(float dt);	
};


#endif // __HELLOWORLD_SCENE_H__

#ifndef _PLAY_SCENE_H__
#define _PLAY_SCENE_H__

#include "cocos2d.h"


#include "PauseScene.h"
#include "Define.h"

class PlayScene : public cocos2d::Layer
{
public:

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void pauseBtnCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(PlayScene);

	

private :
	Director* _director;
	Size _visibleSize;
	Sprite* bomb;
	Sprite* player;

	void setPhysicsBodyForSprite(Sprite* sprite);

	bool onContactBegin(PhysicsContact& contact);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	void updatePlayerPosition(float newPosition);

};

#endif // __HELLOWORLD_SCENE_H__

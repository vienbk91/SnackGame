#ifndef _GAMEOVER_SCENE_H_
#define _GAMEOVER_SCENE_H_


#include "cocos2d.h"

USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(GameOver);

private:

	LayerColor* _background;
	Director* _director;

};

#endif //_GAMEOVER_SCENE_H_

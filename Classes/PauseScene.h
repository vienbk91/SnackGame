#ifndef _PAUSESCENE_H_
#define _PAUSESCENE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class Pause : public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(Pause);

	void exitPause(cocos2d::Ref* pSender);

private:

	Director* _director;
	Size _visibleSize;

};


#endif //_PAUSESCENE_H_
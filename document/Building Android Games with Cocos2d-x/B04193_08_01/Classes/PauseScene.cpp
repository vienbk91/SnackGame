#include "PauseScene.h"

USING_NS_CC;

Scene* Pause::createScene()
{
    auto scene = Scene::create();
    auto layer = Pause::create();
    scene->addChild(layer);
    return scene;
}

bool Pause::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	director = Director::getInstance();  
    visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
    auto pauseItem = MenuItemImage::create(
                                           "play.png",
                                           "play_pressed.png",
                                           CC_CALLBACK_1(Pause::exitPause, this));    
	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
							origin.y + pauseItem->getContentSize().height / 2));
	auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	auto label = Label::createWithTTF("PAUSE", "fonts/Marker Felt.ttf", 96);
    label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height /2);
    this->addChild(label, 1);
    return true;
}

void Pause::exitPause(cocos2d::Ref* pSender){
	/*Pop the pause scene from the Scene stack.
	This will remove current scene.*/
	Director::getInstance()->popScene();
}
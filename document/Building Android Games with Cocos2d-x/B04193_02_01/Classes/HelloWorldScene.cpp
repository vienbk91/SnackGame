#include "HelloWorldScene.h"
#include "PauseScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	_director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
	auto origin = _director->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "pause.png",
                                           "pause_pressed.png",
                                           CC_CALLBACK_1(HelloWorld::pauseCallback, this));
    
	closeItem->setPosition(Vec2(_visibleSize.width - closeItem->getContentSize().width/2 ,
                                closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	auto sprBomb = Sprite::create("bomb.png");	
	sprBomb -> setPosition(_visibleSize.width / 2, _visibleSize.height + sprBomb -> getContentSize().height/2);
	this->addChild(sprBomb,1);
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(0, 0);
	this->addChild(bg, -1);
	auto sprPlayer = Sprite::create("player.png");	
	sprPlayer->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.23);
	this->addChild(sprPlayer, 0);
	//Animations
	Vector<SpriteFrame*> frames;
	Size playerSize = sprPlayer->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, playerSize.width, playerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, playerSize.width, playerSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames,0.2f);
	auto animate = Animate::create(animation);
	sprPlayer->runAction(RepeatForever::create(animate));
	
	
	//actions
	auto moveFinished = CallFuncN::create(CC_CALLBACK_1(HelloWorld::moveFinished, this));
	auto moveTo = MoveTo::create(2, Vec2(sprBomb->getPositionX(), 0 - sprBomb->getContentSize().height/2));
	auto sequence = Sequence::create(moveTo, moveFinished, nullptr);
	sprBomb->runAction(sequence);
    return true;
}

void HelloWorld::moveFinished(Node* sender){
	CCLOG("Move finished");
}

void HelloWorld::pauseCallback(cocos2d::Ref* pSender){
	_director->pushScene(TransitionFlipX::create(1.0, Pause::createScene()));
}

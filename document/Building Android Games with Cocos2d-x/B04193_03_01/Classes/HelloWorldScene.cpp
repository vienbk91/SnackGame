#include "HelloWorldScene.h"
#include "PauseScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();	
	scene->getPhysicsWorld()->setGravity(Vect(0,0));
    auto layer = HelloWorld::create();
	//enable debug draw
	#if COCOS2D_DEBUG
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	#endif
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::onCollision(PhysicsContact& contact)
{
	CCLOG("Collision detected");

	//_sprBomb->setVisible(false);	
	auto body = _sprBomb -> getPhysicsBody();
	body->setVelocity(Vect());
	body->applyTorque(100900.5f);	
	return false;
}

void HelloWorld::setPhysicsBody(cocos2d::Sprite* sprite)
{
	auto body = PhysicsBody::createCircle(sprite->getContentSize().width/2);
	body->setContactTestBitmask(true);
	body->setDynamic(true);	
	sprite -> setPhysicsBody(body);
}

void HelloWorld::initPhysics()
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onCollision,this);
	getEventDispatcher() ->addEventListenerWithSceneGraphPriority(contactListener,this);
	
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
	_sprBomb = Sprite::create("bomb.png");	
	_sprBomb->setPosition(_visibleSize.width / 2, _visibleSize.height + _sprBomb->getContentSize().height/2);
	this->addChild(_sprBomb,1);
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2());
	bg->setPosition(0,0);
	this->addChild(bg, -1);
	auto sprPlayer = Sprite::create("player.png");	
	sprPlayer->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.23);
	setPhysicsBody(sprPlayer);

	this->addChild(sprPlayer, 0);
	//Animations
	Vector<SpriteFrame*> frames;
	Size playerSize = sprPlayer->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, playerSize.width, playerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, playerSize.width, playerSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames,0.2f);
	auto animate = Animate::create(animation);
	sprPlayer->runAction(RepeatForever::create(animate));	

	setPhysicsBody(_sprBomb);	
	initPhysics();	
	_sprBomb->getPhysicsBody()->setVelocity(Vect(0,-100));	
    return true;
}

void HelloWorld::pauseCallback(cocos2d::Ref* pSender){
	_director->pushScene(TransitionFlipX::create(1.0, Pause::createScene()));
}



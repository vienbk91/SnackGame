#include "HelloWorldScene.h"
#include "PauseScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../cocos2d/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif


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

//Move the player if it does not go outside of the screen
void HelloWorld::movePlayerIfPossible(float newX){
	float sprHalfWidth = _sprPlayer->getBoundingBox().size.width /2 ;
	if(newX >= sprHalfWidth && newX < visibleSize.width - sprHalfWidth){
		_sprPlayer->setPositionX(newX);
	}
}

void HelloWorld::movePlayerByTouch(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();
	if(_sprPlayer->getBoundingBox().containsPoint(touchLocation)){
		movePlayerIfPossible(touchLocation.x);
	}
}

void HelloWorld::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event){return true;};
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::movePlayerByTouch,this);
	listener->onTouchEnded = [=](Touch* touch, Event* event){};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::initAccelerometer()
{
	Device::setAccelerometerEnabled(true);
	auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(HelloWorld::movePlayerByAccelerometer, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event)
{
	movePlayerIfPossible(_sprPlayer->getPositionX() + (acceleration->x * 10));
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
    visibleSize = _director->getVisibleSize();
	auto origin = _director->getVisibleOrigin();
    auto closeItem = MenuItemImage::create(
                                           "pause.png",
                                           "pause_pressed.png",
                                           CC_CALLBACK_1(HelloWorld::pauseCallback, this));
    
	closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width/2 ,
                                closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	_sprBomb = Sprite::create("bomb.png");	
	_sprBomb->setPosition(visibleSize.width / 2, visibleSize.height + _sprBomb->getContentSize().height/2);
	this->addChild(_sprBomb,1);
	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2());
	bg->setPosition(0,0);
	this->addChild(bg, -1);
	_sprPlayer = Sprite::create("player.png");	
	_sprPlayer->setPosition(visibleSize.width / 2, visibleSize.height * 0.23);
	setPhysicsBody(_sprPlayer);

	this->addChild(_sprPlayer, 0);
	//Animations
	Vector<SpriteFrame*> frames;
	Size playerSize = _sprPlayer->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, playerSize.width, playerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, playerSize.width, playerSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames,0.2f);
	auto animate = Animate::create(animation);
	_sprPlayer->runAction(RepeatForever::create(animate));	
	
	setPhysicsBody(_sprBomb);	
	initPhysics();	
	_sprBomb->getPhysicsBody()->setVelocity(Vect(0,-100));	
	initTouch();
	initAccelerometer();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeepScreenOnJni(true);
#endif
	initBackButtonListener();
    return true;
}

void HelloWorld::pauseCallback(cocos2d::Ref* pSender){
	_director->pushScene(TransitionFlipX::create(1.0, Pause::createScene()));
}

void HelloWorld::initBackButtonListener(){
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
		Director::getInstance()->end();
	}
}

#include "PlayScene.h"

USING_NS_CC;

Scene* PlayScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    auto layer = PlayScene::create();

	// Thiet lap trong luong bang 0
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));


#if COCOS2D_DEBUG
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif 

    scene->addChild(layer);

    return scene;
}

bool PlayScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
	_director = Director::getInstance();
    _visibleSize = _director->getVisibleSize();
    Vec2 origin = _director->getVisibleOrigin();

	//===========================================================================
	// Create background
	auto background = Sprite::create("background.png");
	background->setAnchorPoint(Vec2::ZERO);
	background->setPosition(Vec2::ZERO);
	this->addChild(background , -1);


	//===========================================================================
	// Create bom
	bomb = Sprite::create("bomb.png");
	bomb->setTag(objectTag::Bomb_Tag);
	bomb->setPosition(_visibleSize.width / 2, _visibleSize.height / 2 + bomb->getContentSize().height / 2);
	this->addChild(bomb , 1);

	setPhysicsBodyForSprite(bomb);

	//=========== Cho bomb roi bang cach tao cho no 1 van toc theo huong xuong duoi
	bomb->getPhysicsBody()->setVelocity(Vect( 0 , -100));

	//===========================================================================
	// Create PlaySceneer

	player = Sprite::create("PlaySceneer.png");
	player->setTag(objectTag::Player_Tag);
	player->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.23);
	this->addChild(player, 0);

	setPhysicsBodyForSprite(player);


	//===========================================================================
	// Create PlaySceneer animations usign spritesheet

	Vector<SpriteFrame*> frames;
	Size PlayerSize = player->getContentSize();
	frames.pushBack(SpriteFrame::create("PlaySceneer.png", Rect(0, 0, PlayerSize.width, PlayerSize.height)));
	frames.pushBack(SpriteFrame::create("PlaySceneer2.png", Rect(0, 0, PlayerSize.width, PlayerSize.height)));

	auto animation = Animation::createWithSpriteFrames(frames , 0.2f);

	auto animate = Animate::create(animation);
	player->runAction(RepeatForever::create(animate));

	//===========================================================================
	// Create Pause button
	auto closeItem = MenuItemImage::create("pause.png", "pause_pressed.png", CC_CALLBACK_1(PlayScene::pauseBtnCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + _visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


	//===========================================================================
	// Event check collisiton

	auto collisitionEvent = EventListenerPhysicsContact::create();
	collisitionEvent->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(collisitionEvent, this);

	//===========================================================================
	// Event touch move

	auto touchEvent = EventListenerTouchOneByOne::create();
	touchEvent->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	touchEvent->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	touchEvent->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);


    return true;
}


void PlayScene::setPhysicsBodyForSprite(Sprite* sprite)
{

	//===========================================================================
	// Create sprite physics body
	auto spriteBody = PhysicsBody::createCircle(sprite->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
	spriteBody->setDynamic(true);
	spriteBody->setContactTestBitmask(0x000001);


	sprite->setPhysicsBody(spriteBody);
}


bool PlayScene::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
void PlayScene::onTouchMoved(Touch* touch, Event* event)
{

}

void PlayScene::onTouchEnded(Touch* touch, Event* event)
{

}


bool PlayScene::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto nodeB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	auto tagA = nodeA->getTag();
	auto tagB = nodeB->getTag();

	if (tagA == objectTag::Bomb_Tag && tagB == objectTag::Player_Tag || tagB == objectTag::Bomb_Tag && tagA == objectTag::Player_Tag)
	{
		log("Xay ra va cham");
		nodeA->getPhysicsBody()->setVelocity(Vect::ZERO);
		nodeB->getPhysicsBody()->setVelocity(Vect::ZERO);
	}



	return true;
}

void PlayScene::pauseBtnCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->pushScene(TransitionFlipX::create(1.0f, Pause::createScene()));
}



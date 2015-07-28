#include "PlayScene.h"

USING_NS_CC;
using namespace experimental;

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

	if (!Layer::init())
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
	this->addChild(background, -1);


	//===========================================================================
	// Create bom
	bomb = Sprite::create("bomb.png");
	bomb->setTag(objectTag::Bomb_Tag);
	bomb->setPosition(_visibleSize.width / 2, _visibleSize.height / 2 + bomb->getContentSize().height / 2);
	this->addChild(bomb, 1);

	setPhysicsBodyForSprite(bomb);

	//=========== Cho bomb roi bang cach tao cho no 1 van toc theo huong xuong duoi
	bomb->getPhysicsBody()->setVelocity(Vect(0, -100));
	_bombs.pushBack(bomb);

	//===========================================================================
	// Create PlaySceneer

	player = Sprite::create("player.png");
	player->setTag(objectTag::Player_Tag);
	player->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.23);
	this->addChild(player, 0);

	setPhysicsBodyForSprite(player);


	//===========================================================================
	// Create PlaySceneer animations usign spritesheet

	Vector<SpriteFrame*> frames;
	Size PlayerSize = player->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, PlayerSize.width, PlayerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, PlayerSize.width, PlayerSize.height)));

	auto animation = Animation::createWithSpriteFrames(frames, 0.2f);

	auto animate = Animate::create(animation);
	player->runAction(RepeatForever::create(animate));

	//===========================================================================
	// Create Pause button
	auto closeItem = MenuItemImage::create("pause.png", "pause_pressed.png", CC_CALLBACK_1(PlayScene::pauseBtnCallback, this));

	closeItem->setPosition(Vec2(origin.x + _visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//===========================================================================
	// Create remove line

	Vec2 vec[2] = {
		Vec2(0,0) ,
		Vec2(_visibleSize.width , 0)
	};

	auto deadLine = Node::create();
	deadLine->setTag(objectTag::Line_Tag);
	auto lineBody = PhysicsBody::createEdgeChain(vec, 2, PHYSICSBODY_MATERIAL_DEFAULT);
	lineBody->setDynamic(false);
	lineBody->setRotationEnable(false);
	lineBody->setGravityEnable(false);
	lineBody->setContactTestBitmask(0x00000001);

	deadLine->setPhysicsBody(lineBody);
	deadLine->setPosition(Vec2(0 , 100));

	this->addChild(deadLine);


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
	

	//===========================================================================
	// Thuc hien sinh bomb , cu 4s se sinh ra bomb

	this->schedule(schedule_selector(PlayScene::addBombs, this), 4.0f);



    return true;
}


void PlayScene::addBombs(float dt)
{
	srand(time(NULL));
	Sprite* bombs = nullptr;
	for (int i = 0; i < 3 ; i++)
	{
		bombs = Sprite::create("bomb.png");
		bombs->setPosition(Vec2(_visibleSize.width * (random(0.1f , 0.9f)) , _visibleSize.height + bombs->getContentSize().height / 2));
		this->addChild(bombs);
		setPhysicsBodyForSprite(bombs);
		bombs->getPhysicsBody()->setVelocity(Vect(0, (random(0.0f, 1.0f) + 0.2f) * (-350)));
		_bombs.pushBack(bombs);
	}
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
	Vec2 touchBegin = touch->getLocation();

	Vector<Sprite*> toErase; // Mang chua cac sprite da bi giet

	for (auto bom : _bombs)
	{
		// Thuc hien cho no bom khi bi touch
		if (bom->getBoundingBox().containsPoint(touchBegin)){
			AudioEngine::play2d("bomb.mp3");

			auto explosion = ParticleExplosion::create();
			explosion->setPosition(bom->getPosition());

			this->addChild(explosion);

			bom->setVisible(false);
			this->removeChild(bom);
			toErase.pushBack(bom);

		}
	}

	for (auto bom : toErase)
	{
		_bombs.eraseObject(bom);
	}


	return true;
}
void PlayScene::onTouchMoved(Touch* touch, Event* event)
{
	Vec2 touchLocation = touch->getLocation();

	if (player->getBoundingBox().containsPoint(touchLocation))
	{
		updatePlayerPosition(touchLocation.x);
	}
}

void PlayScene::onTouchEnded(Touch* touch, Event* event)
{

}


// Ham update vi tri cua player dong thoi khong thuc hien viec di chuyen neu player thoat ra khoi man hinh
void PlayScene::updatePlayerPosition(float newPosition)
{
	float spirteHalfWidth = player->getBoundingBox().size.width / 2;
	// Neu nam trong man hinh thi duoc di chuyen
	if (newPosition > spirteHalfWidth && newPosition < _visibleSize.width - spirteHalfWidth)
	{
		player->setPositionX(newPosition);
	}
}


bool PlayScene::onContactBegin(PhysicsContact& contact)
{
	auto nodeA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto nodeB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	auto tagA = nodeA->getTag();
	auto tagB = nodeB->getTag();


	/*
	if (tagA == objectTag::Bomb_Tag && tagB == objectTag::Player_Tag || tagB == objectTag::Bomb_Tag && tagA == objectTag::Player_Tag)
	{
		log("Xay ra va cham bomb va player");
		nodeA->getPhysicsBody()->setVelocity(Vect::ZERO);
		nodeB->getPhysicsBody()->setVelocity(Vect::ZERO);

		_director->pushScene(GameOver::createScene());
	}


	if (tagA == objectTag::Bomb_Tag && tagB == objectTag::Line_Tag)
	{
		log("Xay ra va cham bomb va line");
		nodeA->removeFromParentAndCleanup(true);
	}
	
	if (tagB == objectTag::Bomb_Tag && tagA == objectTag::Line_Tag){
		log("Xay ra va cham bomb va line");
		nodeB->removeFromParentAndCleanup(true);
	}

	*/

	//auto body = bomb->getPhysicsBody();
	//body->applyTorque(100900.5f);



	return true;
}

void PlayScene::pauseBtnCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->pushScene(TransitionFlipX::create(1.0f, Pause::createScene()));
}



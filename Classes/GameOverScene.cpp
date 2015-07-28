#include "GameOverScene.h"

USING_NS_CC;


Scene* GameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();

	scene->addChild(layer);

	return scene;
}

bool GameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_director = Director::getInstance();
	Size visibleSize = _director->getVisibleSize();
	Vec2 origin = _director->getVisibleOrigin();


	_background = LayerColor::create(Color4B(0, 0, 0, 255));
	_background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(_background);

	auto label = Label::createWithTTF("GAME OVER", "fonts/Marker Felt.ttf", 96);

	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(label, 1);

	return true;
}
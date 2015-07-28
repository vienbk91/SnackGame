#include "AppDelegate.h"
#include "PlayScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

	// Thiet lap ti le man hinh test tren win32
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		glview->setFrameSize(480, 800);
	}


	// Thiet lap resource file paths theo kich thuoc man hinh
	Size screenSize = glview->getFrameSize();
	Size designSize = CCSizeMake(768, 1280);

	std::vector<std::string> searchPaths;

	searchPaths.push_back("sounds"); // Add sounds
	searchPaths.push_back("particles"); // Add effect particle


	if (screenSize.height > 800){
		searchPaths.push_back("images/high");
		director->setContentScaleFactor(1280.0f / designSize.height);
	}
	else if (screenSize.height > 600){
		searchPaths.push_back("images/mid");
		director->setContentScaleFactor(800.0f / designSize.height);
	}
	else {
		searchPaths.push_back("images/low");
		director->setContentScaleFactor(320.0f / designSize.height);
	}

	FileUtils::getInstance()->setSearchPaths(searchPaths);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);


    // turn on display FPS
    director->setDisplayStats(false); // An thong tin FPS di

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = PlayScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameLayer.h"
#include "utils.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	CCSize frameSize = pEGLView->getFrameSize();
	gDesignSize.width = 480;
	gDesignSize.height = 320;

	float fResouceWH = gDesignSize.width / gDesignSize.height;
	float fViewWH = frameSize.width / frameSize.height;
	if (fViewWH >= fResouceWH)
	{
		pEGLView->setDesignResolutionSize(gDesignSize.width, gDesignSize.height, kResolutionFixedHeight);
		gSolutionType = kResolutionFixedHeight;
	} 
	else
	{
		pEGLView->setDesignResolutionSize(gDesignSize.width, gDesignSize.height, kResolutionFixedWidth);
		gSolutionType = kResolutionFixedWidth;
	}
	gViewSize = pEGLView->getVisibleSize();
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    //CCScene *pScene = HelloWorld::scene();
	CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

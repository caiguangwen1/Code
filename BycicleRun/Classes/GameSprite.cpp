/*
    文件名：    GameSprite.cpp
    描　述：    游戏中定义的一些精灵类
    创建人：    郝萌主 (博客：http://blog.csdn.net/haomengzhu)

    创建日期：   2013.12.05
*/
#include "GameSprite.h"

USING_NS_CC;

GameSprite::GameSprite(void)
	:_vector(ccp(0,0))
	,_screenSize(CCDirector::sharedDirector()->getWinSize())
{
}

GameSprite::~GameSprite(void){}


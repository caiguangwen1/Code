/*
    �ļ�����    GameSprite.cpp
    �衡����    ��Ϸ�ж����һЩ������
    �����ˣ�    ������ (���ͣ�http://blog.csdn.net/haomengzhu)

    �������ڣ�   2013.12.05
*/
#include "GameSprite.h"

USING_NS_CC;

GameSprite::GameSprite(void)
	:_vector(ccp(0,0))
	,_screenSize(CCDirector::sharedDirector()->getWinSize())
{
}

GameSprite::~GameSprite(void){}

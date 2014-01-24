/*
    文件名：    Block.h
    描　述：    游戏中定义的街区块头文件
    创建人：    郝萌主 (博客：http://blog.csdn.net/haomengzhu)

    创建日期：   2013.12.05
*/
#ifndef __BLOCK_H__
#define __BLOCK_H__



#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;


enum {
	kWallTile,//墙
	kRoofTile,//屋顶
	kChimney //烟囱
};

enum  {
	kBlockGap,//块间隔
	kBlock1,
	kBlock2,
	kBlock3,
	kBlock4

};

//街区类
class Block : public GameSprite {

	//屏幕宽高
	CCSize _screenSize;

	int _tileWidth;
	int _tileHeight;
	int _puffIndex;

	CCSpriteFrame * _tile1;
	CCSpriteFrame * _tile2;
	CCSpriteFrame * _tile3;
	CCSpriteFrame * _tile4;

	CCSpriteFrame * _roof1;//屋顶1
	CCSpriteFrame * _roof2;//屋顶2

	CCArray * _wallTiles;
	CCArray * _roofTiles;

	CCAction * _puffSpawn;
	CCAction * _puffMove;
    CCAction * _puffFade;
    CCAction * _puffScale;
    CCAction * _puffAnimation;

	void initBlock(void);
    void createPuff(void);
public:

	CC_SYNTHESIZE(int, _type, Type);

	//声明一个成员变量_puffing以及getfunName函数，没有set函数。getfunName函数的实现要自己做
	CC_SYNTHESIZE_READONLY(bool, _puffing, Puffing);

	//声明成员变量数组：烟囱
	CC_SYNTHESIZE(CCArray *, _chimneys, Chimneys);

	Block();
	~Block();

	static Block * create();
	void setupBlock (int width, int height, int type);
	void setPuffing (bool value);
	void hidePuffs();

	inline virtual int left() {
		return this->getPositionX();
	}

	inline virtual int right() {
		return this->getPositionX() + _width;
	}

	inline virtual int top() {
		return this->getHeight();
	}

	inline virtual int bottom() {
		return 0;
	}


};

#endif // __BLOCK_H__

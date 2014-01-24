/*
    文件名：    Player.h
    描　述：    游戏中定义的主角
    创建人：    郝萌主 (博客：http://blog.csdn.net/haomengzhu)

    创建日期：   2013.12.05
*/
#ifndef __PLAYER_H__
#define __PLAYER_H__

//主角初始速度
#define PLAYER_INITIAL_SPEED 4
//#define PLAYER_JUMP 42
#define PLAYER_JUMP 15
#define GRAVITY 1.3f
#define FLOATNG_GRAVITY 0.4f
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

typedef enum
{
	kPlayerMoving,//主角在移动
	kPlayerFalling,//主角在降落
	kPlayerDying//主角死亡

} PlayerState; 

//主角继承于游戏精灵类
class Player : public GameSprite {
    
    CCAction * _rideAnimation;
    CCAction * _floatAnimation;
    float _speed;
	int _floatingTimerMax;
	float _floatingTimer;
	int _floatingInterval;
	bool _hasFloated;

	CCSize _screenSize;

	void initPlayer (void);

public:

	Player(void);
	~Player(void);

	//定义变量，并且直接定义默认的get/set方法
	CC_SYNTHESIZE(PlayerState, _state, State);
	CC_SYNTHESIZE(bool, _inAir, InAir);
	CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
	CC_SYNTHESIZE(bool, _jumping, Jumping);
	CC_SYNTHESIZE(float, _maxSpeed, MaxSpeed);

	static Player * create (void);

	virtual void update (float dt);

	void setFloating (bool value);

	void reset (void);

	//主角位置的真正实现
	inline virtual void place () { 
		this->setPositionY( _nextPosition.y );
		if (_vector.x > 0 && this->getPositionX() < _screenSize.width * 0.2f) {
			this->setPositionX(this->getPositionX() + _vector.x);
			if (this->getPositionX() > _screenSize.width * 0.2f) {
				this->setPositionX(_screenSize.width * 0.2f);
			}
		}
	};

	//主角在X轴上的位置[左侧有效碰撞]
	inline int left() {
		return this->getPositionX() - _width * 0.5f;
	}

	//主角在X轴上的位置[右侧有效碰撞]
	inline int right() {
		return this->getPositionX() + _width * 0.5f;
	}

	//主角在Y轴上的位置
	inline int top() {
		return this->getPositionY() ;
	}

	//主角在Y轴上的位置【底部有效碰撞】
	inline int bottom() {
		return this->getPositionY() - _height ;
	}

	//主角下一个位置的左侧
	inline int next_left() {
		return _nextPosition.x - _width * 0.5f;
	}

	//主角下一个位置的右侧
	inline int next_right() {
		return _nextPosition.x + _width * 0.5f;
	}

	//主角下一个位置的上部
	inline int next_top() {
		return _nextPosition.y ;
	}

	//主角下一个位置的底部
	inline int next_bottom() {
		return _nextPosition.y - _height;
	}
};

#endif // __PLAYER_H__



/*
    文件名：    Player.h
    描　述：    游戏主角的实现
    创建人：    郝萌主 (博客：http://blog.csdn.net/haomengzhu)

    创建日期：   2013.12.05
*/
#include "Player.h"
#define ACCELERATION 0.05f

Player::~Player(){

    CC_SAFE_RELEASE(_rideAnimation);
    CC_SAFE_RELEASE(_floatAnimation);
}

Player::Player() {
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	_floatingTimerMax = 2;
	_floatingTimer = 0;
	_speed = PLAYER_INITIAL_SPEED;
	_maxSpeed = PLAYER_INITIAL_SPEED;
	_floating = false;
	_nextPosition = CCPointZero;
	_nextPosition.y = _screenSize.height * 0.6f;
	_state = kPlayerMoving;
	_jumping = false;
	_hasFloated = false;
}

Player * Player::create () {

	Player * player = new Player();
	if (player && player->initWithSpriteFrameName("player_1.png")) {
		player->autorelease();
		player->setSize();
		player->initPlayer();
		return player;
	}
	CC_SAFE_DELETE(player);
	return NULL;
}


void Player::update (float dt) {

	//加速度到最大，然后匀速
	if (_speed + ACCELERATION <= _maxSpeed) {
		_speed += ACCELERATION;
	} else {
		_speed = _maxSpeed;
	}

	_vector.x = _speed;

	//CCLog("play state:%d",_state);

	switch (_state) {
	case kPlayerMoving:
		_vector.y -= GRAVITY;
		if (_hasFloated) 
			_hasFloated = false;
		break;

	case kPlayerFalling:

		if (_floating ) {
			_vector.y -= FLOATNG_GRAVITY;
			_vector.x *= FLOATING_FRICTION;

		} else {
			_vector.y -= GRAVITY;
			_vector.x *= AIR_FRICTION;
			_floatingTimer = 0;
		}
		break;

	case kPlayerDying:
		_vector.y -= GRAVITY;
		_vector.x = -_speed;
		this->setPositionX(this->getPositionX() + _vector.x);
		break;

	}

	if (_jumping) {
		_state = kPlayerFalling;
		_vector.y += PLAYER_JUMP * 0.25f;
		if (_vector.y > PLAYER_JUMP ) 
			_jumping = false;
	}

	if (_vector.y < -TERMINAL_VELOCITY) 
		_vector.y = -TERMINAL_VELOCITY;

	_nextPosition.y = this->getPositionY() + _vector.y;
	if (_vector.x * _vector.x < 0.01) 
		_vector.x = 0;
	if (_vector.y * _vector.y < 0.01) 
		_vector.y = 0;

	if (_floating) {
		_floatingTimer += dt;
		if (_floatingTimer > _floatingTimerMax) {
			_floatingTimer = 0;
			this->setFloating(false);
		}
	}
}

//主角重置
void Player::reset () {

	_speed = PLAYER_INITIAL_SPEED;
	_maxSpeed = PLAYER_INITIAL_SPEED;
	_vector = ccp(0,0);
	this->setFloating(false);
	this->setRotation(0);
	_nextPosition.y = _screenSize.height * 0.6f;
	this->setPosition(ccp( _screenSize.width * 0.2f, _nextPosition.y ));
	_state = kPlayerMoving;
	_jumping = false;
	_hasFloated = false;
}

//设置下降动作
void Player::setFloating (bool value) {

	if (_floating == value) 
		return;

	if (value && _hasFloated) 
		return;

	_floating = value;

    this->stopAllActions();

	if (value) {
		_hasFloated = true;
        this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("player_float.png"));
        this->runAction(_floatAnimation);
		_vector.y += PLAYER_JUMP * 0.5f;
    } else {
        
        this->runAction(_rideAnimation);
	} 
}

//初始化主角
void Player::initPlayer () {

	//设置锚点
	this->setAnchorPoint(ccp(0.5f, 1.0f));
	this->setPosition(ccp(_screenSize.width * 0.2f, _nextPosition.y));

	//_height = 228;
	//_width = 180;
	_height = 50;
	_width = 50;
    CCAnimation* animation;
	//创建一个空白的序列帧动画信息
    animation = CCAnimation::create();

	//CCSpriteFrame对应的就是帧，将CCSpriteFrame添加到CCAnimation生成动画数据，
	//用CCAnimation生成CCAnimate（就是最终的动画动作），最后可以用CCSprite执行这个动作。
    CCSpriteFrame * frame;
    int i;
	//共有3帧，这里用for循环将对应的序列图加入到动画中
    for(i = 1; i <= 3; i++) {
        char szName[100] = {0};
        sprintf(szName, "player_%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
	//设置每两帧间时间间隔
    animation->setDelayPerUnit(0.2f / 3.0f);
	//设置动画结束后是否保留动画帧信息
    animation->setRestoreOriginalFrame(false);
	//设置循环播放次数 (-1:无限循环)
    animation->setLoops(-1);
	//由这个动画信息创建一个序列帧动画
    _rideAnimation = CCAnimate::create(animation);
	//保存这个动画
    _rideAnimation->retain();
    
	//CCFiniteTimeAction是所有有限次执行类（或者也可以叫顺序执行类吧）的基类
	//CCSequence创建一个动作序列
	//CCEaseInOut创建对应匀速动画的变速动画，参数一为一个匀速动画。参数二为速度
    CCFiniteTimeAction* easeSwing = CCSequence::create(
           CCEaseInOut::create(CCRotateTo::create(0.8f, -10), 2),
           CCEaseInOut::create(CCRotateTo::create(0.8f, 10), 2),
           NULL);
    _floatAnimation = CCRepeatForever::create( (CCActionInterval*) easeSwing );
    _floatAnimation->retain();
    
    this->runAction(_rideAnimation);
}

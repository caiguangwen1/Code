/*
    �ļ�����    GameLayer.cpp
    �衡����    ��Ϸ�����ʵ��
    �����ˣ�    ������ (���ͣ�http://blog.csdn.net/haomengzhu)

    �������ڣ�   2013.12.05
*/
#include "GameLayer.h"
#include "utils.h"

using namespace cocos2d;

GameLayer::~GameLayer() {
    
    CC_SAFE_RELEASE(_jamMove);
    CC_SAFE_RELEASE(_jamAnimate);
}
CCScene* GameLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameLayer *layer = GameLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		//��ȡ��Ļ��С
		_screenSize = CCDirector::sharedDirector()->getWinSize();

		//������Ϸ����
		createGameScreen();

		//���¿�ʼ��Ϸ
		resetGame();

		//listen for touches
		this->setTouchEnabled(true);

		//create main loop
		this->schedule(schedule_selector(GameLayer::update));

        bRet = true;
    } while (0);

    return bRet;
}

//���¿�ʼ��Ϸ
void GameLayer::resetGame () {

	//��ʼ������
    _score = 0;
	_speedIncreaseInterval = 15;
	_speedIncreaseTimer = 0;

	//��ʾ����
    char szValue[100] = {0};
    sprintf(szValue, "%i", (int) _score);
    _scoreDisplay->setString (szValue);
    _scoreDisplay->setAnchorPoint(ccp(1,0));
    _scoreDisplay->setPosition(transPoint(ccp(gDesignSize.width * 0.95f, gDesignSize.height * 0.88f)));
    
    _state = kGameIntro;
    
    _intro->setVisible(true);
    _mainMenu->setVisible(true);
    
    _jam->setPosition(transPoint(ccp(gDesignSize.width * 0.19f, gDesignSize.height * 0.47f)));
    _jam->setVisible(true);
    _jam->runAction(_jamAnimate);
	_running = true;
}

void GameLayer::update(float dt) {
	//CCLog("_running:%d",_running);
	if (!_running) 
		return;

	//��ȡ����λ��
	if (_player->getPositionY() < -_player->getHeight() ||
		_player->getPositionX() < -_player->getWidth() * 0.5f) {
        if (_state == kGamePlay) {
            
            _running = false;

            //create GAME OVER state           
            _state = kGameOver;
            
            _tryAgain->setVisible(true);
            _scoreDisplay->setAnchorPoint(ccp(0.5f, 0.5f));
            //_scoreDisplay->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.88f));
			_scoreDisplay->setPosition(transPoint(ccp(gDesignSize.width * 0.5f, gDesignSize.height * 0.88f)));
            //_hat->setPosition(ccp(_screenSize.width * 0.2f, -_screenSize.height * 0.1f));
			_hat->setPosition(transPoint(ccp(gDesignSize.width * 0.2f, -gDesignSize.height * 0.1f)));
            _hat->setVisible(true);

			//��ת���飬�Զ�Ϊ��λ
            CCAction * rotate = CCRotateBy::create(2.0f, 660);
            //CCAction * jump = CCJumpBy::create(2.0f, ccp(0,10), _screenSize.height * 0.8f, 1);
			CCAction * jump = CCJumpBy::create(2.0f, ccp(0,10), transYPoint(gDesignSize.height * 0.8f), 1);
			
			//����������ñ����ת����
            _hat->runAction(rotate);
            _hat->runAction(jump);
            //SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            //SimpleAudioEngine::sharedEngine()->playEffect("crashing.wav");
        }
    }

	//��������״̬
	_player->update(dt);

	_terrain->move(_player->getVector().x);

	//�����ײ��ͻ
	if (_player->getState() != kPlayerDying) 
		_terrain->checkCollision(_player);

	//��������
	_player->place();

	if (_player->getNextPosition().y > _screenSize.height * 0.6f) {
		_gameBatchNode->setPositionY( (_screenSize.height * 0.6f - _player->getNextPosition().y) * 0.8f);
	} else {
		_gameBatchNode->setPositionY  ( 0 );
	}

    //update paralax
    if (_player->getVector().x > 0) {
		//һֱ������
        _background->setPositionX(transXPoint(_background->getPosition().x - _player->getVector().x * 0.25f));
        float diffx;
        //����һ������ʱ�����°�λ������Ϊ�ӽ�0��λ��
        if (_background->getPositionX() < -_background->getContentSize().width) {
            diffx = fabs(_background->getPositionX()) - _background->getContentSize().width;
            _background->setPositionX(-diffx);
        }
        
        _foreground->setPositionX(_foreground->getPosition().x - _player->getVector().x * 4);
        
        if (_foreground->getPositionX() < -_foreground->getContentSize().width * 4) {
            diffx = fabs(_foreground->getPositionX()) - _foreground->getContentSize().width * 4;
            _foreground->setPositionX(-diffx);
        }
        
        int count = _clouds->count();
        CCSprite * cloud;
        for (int i = 0; i < count; i++) {
            cloud = (CCSprite *) _clouds->objectAtIndex(i);
            cloud->setPositionX(cloud->getPositionX() - _player->getVector().x * 0.15f);
            if (cloud->getPositionX() + cloud->boundingBox().size.width * 0.5f < 0 )
                cloud->setPositionX(_screenSize.width + cloud->boundingBox().size.width * 0.5f);
        }
    }
    
    if (_jam->isVisible()) {
        if (_jam->getPositionX() < -_screenSize.width * 0.2f) {
            _jam->stopAllActions();
            _jam->setVisible(false);
        }
    }
	if (_terrain->getStartTerrain() && _player->getVector().x > 0) {

        _score += dt * 50;
        char szValue[100] = {0};
        sprintf(szValue, "%i", (int) _score);
        _scoreDisplay->setString (szValue);
		_speedIncreaseTimer += dt;
		if (_speedIncreaseTimer > _speedIncreaseInterval) {
			_speedIncreaseTimer = 0;
			_player->setMaxSpeed (_player->getMaxSpeed() + 4);
		}
	}
    if (_state > kGameTutorial) {
        if (_state == kGameTutorialJump) {
            if (_player->getState() == kPlayerFalling && _player->getVector().y < 0) {
                _player->stopAllActions();
                _jam->setVisible(false);
                _jam->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While in the air, tap the screen to float.");
                _state = kGameTutorialFloat;
            }
        } else if (_state == kGameTutorialFloat) {
            if (_player->getPositionY() < _screenSize.height * 0.95f) {
                _player->stopAllActions();
                _running = false;
                _tutorialLabel->setString("While floating, tap the screen again to drop.");
                _state = kGameTutorialDrop;
            }
        } else {
            _tutorialLabel->setString("That's it. Tap the screen to play.");
            _state = kGameTutorial;
        }
    }
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event) {

	CCTouch *touch = (CCTouch *)pTouches->anyObject();
    
    if (touch) {
	    
	    CCPoint tap = touch->getLocation();
        
        switch (_state) {
            
            case kGameIntro:
                break;
            case kGameOver:
                
                if (_tryAgain->boundingBox().containsPoint(tap)) {
                    _hat->setVisible(false);
                    _hat->stopAllActions();
                    _tryAgain->setVisible(false);
                    _terrain->reset();
                    _player->reset();
                    
                    resetGame();
                }
                break;
                
            case kGamePlay:
                
                if (_player->getState() == kPlayerFalling) {
                    _player->setFloating ( _player->getFloating() ? false : true );
                
                } else {
                    if (_player->getState() !=  kPlayerDying) {
                        //SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
                        _player->setJumping(true);
                    }
                }
                
                _terrain->activateChimneysAt(_player);
                
                break;
            case kGameTutorial:
                _tutorialLabel->setString("");
                _tutorialLabel->setVisible(false);
                _terrain->setStartTerrain ( true );
                _state = kGamePlay;
                break;
            case kGameTutorialJump:
                if (_player->getState() == kPlayerMoving) {
                   // SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
                    _player->setJumping(true);
                }
                break;
            case kGameTutorialFloat:
                if (!_player->getFloating()) {
                    _player->setFloating (true);
                    _running = true;
                }
                break;
            case kGameTutorialDrop:
                _player->setFloating (false);
                _running = true;
                break;
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event) {
    if (_state == kGamePlay) {
        _player->setJumping(false);
    }
}

void GameLayer::showTutorial (CCObject* pSender) {
    _tutorialLabel->setString("Tap the screen to make the player jump.");
    _state = kGameTutorialJump;
    _jam->runAction(_jamMove);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
    //SimpleAudioEngine::sharedEngine()->playEffect("start.wav");
    _tutorialLabel->setVisible(true);
    
}
void GameLayer::startGame (CCObject* pSender) {
    _tutorialLabel->setVisible(false);
    _intro->setVisible(false);
    _mainMenu->setVisible(false);
    
    _jam->runAction(_jamMove);
   // SimpleAudioEngine::sharedEngine()->playEffect("start.wav");
    _terrain->setStartTerrain ( true );
    _state = kGamePlay;
}

void GameLayer::createGameScreen () {
	CCSprite * frameSpr = CCSprite::create("full_screen_back_small.png");
	//frameSpr->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	frameSpr->setPosition(transPoint(ccp(gDesignSize.width/2, gDesignSize.height/2)));
	this->addChild(frameSpr, kMax);
	//������Ϸ����
    CCSprite * bg = CCSprite::create("bg.png");
    //bg->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	bg->setPosition(transPoint(ccp(gDesignSize.width/2, gDesignSize.height/2)));
    this->addChild(bg, kBackground);
    
	//CCSpriteFrameCache������֡���棩��Ҫ�������CCSpriteFrame����û���ṩ�ر�����ԣ������ṩһϵ�����ڹ���CCSpriteFrame�ķ���
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist","sprite_sheet.png");
    _gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png", 200);
    this->addChild(_gameBatchNode, kMiddleground);
    
    CCSprite * repeat;
    
    _background = CCSprite::createWithSpriteFrameName("background.png");
    _background->setAnchorPoint(ccp(0,0));
    _gameBatchNode->addChild(_background, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("background.png");
    repeat->setAnchorPoint(ccp(0,0));
    //repeat->setPosition(ccp(repeat->getContentSize().width - 1, 0));
	repeat->setPosition(transPoint(ccp(repeat->getContentSize().width - 1, 0)));
    _background->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("background.png");
    repeat->setAnchorPoint(ccp(0,0));
    //repeat->setPosition(ccp(2 * (repeat->getContentSize().width - 1), 0));
	repeat->setPosition(transPoint(ccp(2 * (repeat->getContentSize().width - 1), 0)));
    _background->addChild(repeat, kBackground);
    
	//����·��
    _foreground = CCSprite::createWithSpriteFrameName("lamp.png");
    _foreground->setAnchorPoint(ccp(0,0));
    _gameBatchNode->addChild(_foreground, kForeground);
    
    repeat = CCSprite::createWithSpriteFrameName("lamp.png");
    repeat->setAnchorPoint(ccp(0,0));
    //repeat->setPosition(ccp(repeat->getContentSize().width * 4, 0));
	repeat->setPosition(transPoint(ccp(repeat->getContentSize().width * 4, 0)));
    _foreground->addChild(repeat, kBackground);
    
    repeat = CCSprite::createWithSpriteFrameName("lamp.png");
    repeat->setAnchorPoint(ccp(0,0));
    //repeat->setPosition(ccp(repeat->getContentSize().width * 8, 0));
	repeat->setPosition(transPoint(ccp(repeat->getContentSize().width * 8, 0)));
    _foreground->addChild(repeat, kBackground);
    
    //���뾫����
    CCSprite * cloud;
    _clouds = CCArray::createWithCapacity(4);
    _clouds->retain();
    float cloud_y;
    for (int i = 0; i < 4; i++) {
		//��λ�ã�һ��һ��һ��һ����ʾ
        cloud_y = i % 2 == 0 ? _screenSize.height * 0.7f : _screenSize.height * 0.8f;
        cloud = CCSprite::createWithSpriteFrameName("cloud.png");
        cloud->setPosition(ccp (_screenSize.width * 0.15f + i * _screenSize.width * 0.25f,  cloud_y));
        _gameBatchNode->addChild(cloud, kBackground);
        _clouds->addObject(cloud);
    }

	//����������
	_terrain = Terrain::create();
	_gameBatchNode->addChild(_terrain, kMiddleground);

	//����������
	_player = Player::create();
	_gameBatchNode->addChild(_player, kBackground);

	//����logo
    _intro = CCSprite::createWithSpriteFrameName("logo.png");
    //_intro->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
	_intro->setPosition(transPoint(ccp(gDesignSize.width * 0.5f, gDesignSize.height * 0.7f)));
    _gameBatchNode->addChild(_intro, kForeground);

	//��������
    _tryAgain = CCSprite::createWithSpriteFrameName("label_tryagain.png");
    //_tryAgain->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.7f));
	_tryAgain->setPosition(transPoint(ccp(gDesignSize.width * 0.5f, gDesignSize.height * 0.7f)));
    _tryAgain->setVisible(false);
    this->addChild(_tryAgain, kForeground);
    
    //������ְ�
    _scoreDisplay = CCLabelBMFont::create("000000", "font.fnt", _screenSize.width * 0.3f, kCCTextAlignmentCenter);
    _scoreDisplay->setAnchorPoint(ccp(1,1));
    //_scoreDisplay->setPosition(ccp(_screenSize.width * 0.95f, _screenSize.height * 0.88f));
	//_scoreDisplay->setPosition(transPoint(ccp(gDesignSize.width * 0.95f, gDesignSize.height * 0.88f)));
	_scoreDisplay->setPosition(ccp(gDesignSize.width * 0.95f, gDesignSize.height * 0.88f));
    this->addChild(_scoreDisplay, kBackground);
    
    //����ñ��
    _hat = CCSprite::createWithSpriteFrameName("hat.png");
    _hat->setVisible(false);
    _gameBatchNode->addChild(_hat, kMiddleground);
   
	//����һ�������Ŷ�
    _jam = CCSprite::createWithSpriteFrameName("jam_1.png");
    
    CCAnimation* animation;
    animation = CCAnimation::create();
    CCSpriteFrame * frame;
    int i;
    for(i = 1; i <= 3; i++) {
        char szName[100] = {0};
        sprintf(szName, "jam_%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szName);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.2f / 3.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    
    _jamAnimate = CCAnimate::create(animation);
    _jamAnimate->retain();
    _gameBatchNode->addChild(_jam, kBackground);
    
    _jam->setPosition(ccp(_screenSize.width * 0.19f, _screenSize.height * 0.47f));
    _jamMove = CCMoveTo::create(6.0f, ccp(-_screenSize.width * 0.3f, _jam->getPositionY()));
    _jamMove->retain();
    
    //add menu
    CCSprite * menuItemOn;
    CCSprite * menuItemOff;
    //�˵�������״̬��ƽʱչʾ�����Ӻ͵��������
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_new_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_new_off.png");
    //New Game �˵�
    CCMenuItemSprite * starGametItem = CCMenuItemSprite::create(
                                                                menuItemOff,
                                                                menuItemOn,
                                                                this,
																//�������Ҫ������˵�����ϵͳ�ĸ�����
                                                                menu_selector(GameLayer::startGame));
    
    menuItemOn = CCSprite::createWithSpriteFrameName("btn_howto_on.png");
    menuItemOff = CCSprite::createWithSpriteFrameName("btn_howto_off.png");
    //How to Play �˵�
    CCMenuItemSprite * howToItem = CCMenuItemSprite::create(
                                                            menuItemOff,
                                                            menuItemOn,
                                                            this,
                                                            menu_selector(GameLayer::showTutorial));
    
    _mainMenu = CCMenu::create(howToItem, starGametItem, NULL);//�����˵�
    _mainMenu->alignItemsHorizontallyWithPadding(120);//���������˵���ˮƽ���
    //_mainMenu->alignItemsVerticallyWithPadding(50);
    _mainMenu->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.54));

    this->addChild(_mainMenu, kForeground);
      
    _tutorialLabel = CCLabelTTF::create("", "Times New Roman", 25);
    _tutorialLabel->setPosition(ccp (_screenSize.width * 0.5f, _screenSize.height * 0.6f) );
    this->addChild(_tutorialLabel, kForeground);
    _tutorialLabel->setVisible(false);
    
}
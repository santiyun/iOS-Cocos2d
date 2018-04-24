#include "HelloCocosScene.h"
#include "HomeScene.h"
#include "ui/CocosGUI.h"
//#include "SimpleAudioEngine.h"

USING_NS_CC;

static HelloCocosScene *s_helloCocosScene = nullptr;

HelloCocosScene* HelloCocosScene::getInstance()
{
    if (s_helloCocosScene == nullptr) {
        s_helloCocosScene = HelloCocosScene::create();
    }
    
//    auto layer = Layer::create();
//    s_helloCocosScene->addChild(layer);
    
    return s_helloCocosScene;
}

HelloCocosScene::~HelloCocosScene()
{
    s_helloCocosScene = nullptr;
}

void HelloCocosScene::setUserID(unsigned int userID)
{
    if (_myUserControl != nullptr) {
        _myUserControl->setUserID(userID);
    }
}

unsigned int HelloCocosScene::getUserID()
{
    if (_myUserControl != nullptr) {
        return _myUserControl->getUserID();
    }
    return 0;
}

// on "init" you need to initialize your instance
bool HelloCocosScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    Vec2 origin = director->getVisibleOrigin();

    auto backSprite = Sprite::create("background.png");
    float backScaleX = visibleSize.width / backSprite->getContentSize().width;
    float backScaleY = visibleSize.height / backSprite->getContentSize().height;
    backSprite->setScale(backScaleX, backScaleY);
    backSprite->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(backSprite, 0);
    
    auto gameSprite = Sprite::create("game.png");
    gameSprite->setScale(backScaleX, backScaleY);
    gameSprite->setPosition(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - gameSprite->getContentSize().height * backScaleY / 2);
    this->addChild(gameSprite, 1);
    
    auto leaveButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    leaveButton->setScale(backScaleX);
    leaveButton->setTitleFontSize(8 * (1 / backScaleX));
    leaveButton->setTitleText("退出房间");
    leaveButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + leaveButton->getContentSize().height * backScaleX * 3 / 2));
    leaveButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onLeaveButtonClicked();
                break;
            default:
                break;
        }
    });
    this->addChild(leaveButton, 1);
    
    TTTUserControl *userControl1 = TTTUserControl::create("UserEmpty.png");
    userControl1->setScale(backScaleX);
    float userControlWidth = userControl1->getContentSize().width * backScaleX;
    float userControlHeight = userControl1->getContentSize().height * backScaleX;
    float userControlY = gameSprite->getPositionY() - gameSprite->getContentSize().height * backScaleY / 2 - userControlHeight;
    userControl1->setPosition(origin.x + userControlWidth, userControlY);
    this->addChild(userControl1, 1);
    _myUserControl = userControl1;
    
    TTTUserControl *userControl4 = TTTUserControl::create("UserEmpty.png");
    userControl4->setScale(backScaleX);
    userControl4->setPosition(origin.x + visibleSize.width - userControlWidth, userControlY);
    this->addChild(userControl4, 1);
    
    float theSpacing = (userControl4->getPositionX() - userControl1->getPositionX()) / 3;
    
    TTTUserControl *userControl2 = TTTUserControl::create("UserEmpty.png");
    userControl2->setScale(backScaleX);
    userControl2->setPosition(userControl1->getPositionX() + theSpacing, userControlY);
    this->addChild(userControl2, 1);

    TTTUserControl *userControl3 = TTTUserControl::create("UserEmpty.png");
    userControl3->setScale(backScaleX);
    userControl3->setPosition(userControl4->getPositionX() - theSpacing, userControlY);
    this->addChild(userControl3, 1);

    _userControls.clear();
    _userControls.push_back(userControl1);
    _userControls.push_back(userControl2);
    _userControls.push_back(userControl3);
    _userControls.push_back(userControl4);

    scheduleUpdate();

    return true;
}

TTTUserControl* HelloCocosScene::getUserControlWithUserID(unsigned int userID)
{
    if (userID == 0) {
        return nullptr;
    }
    
    for (auto iter = _userControls.cbegin(); iter != _userControls.cend(); iter++) {
        TTTUserControl *userControl = *iter;
        if (userControl->getUserID() == userID) {
            return userControl;
        }
    }
    return nullptr;
}

TTTUserControl* HelloCocosScene::getAvailableUserControl()
{
    for (auto iter = _userControls.cbegin(); iter != _userControls.cend(); iter++) {
        TTTUserControl *userControl = *iter;
        if (userControl->getUserID() == 0) {
            return userControl;
        }
    }
    return nullptr;
}

void HelloCocosScene::update(float delta)
{
    TTTRtcEngine::getInstance(APP_ID)->poll();
}

void HelloCocosScene::onLeaveButtonClicked()
{
    TTTRtcEngine::getInstance(APP_ID)->leaveChannel();
}

void HelloCocosScene::onError(ERROR_CODE code, const char *message)
{
    CCLOG("[Debug] onError %d, %s", (int)code, message);
}

void HelloCocosScene::onConnectionLost()
{
    CCLOG("[Debug] onConnectionLost");
}

void HelloCocosScene::onLeaveChannel(const RtcStats& stats)
{
    CCLOG("[Debug] onLeaveChannel %d", stats.duration);
    Director::getInstance()->popToRootScene();
}

void HelloCocosScene::onUserJoined(unsigned int uid, int elapsed)
{
    CCLOG("[Debug] onUserJoined %d", uid);
    TTTUserControl *theUserControl = getAvailableUserControl();
    if (theUserControl != nullptr) {
        theUserControl->setUserID(uid);
    }
}

void HelloCocosScene::onUserOffline(unsigned int uid, USER_OFFLINE_REASON reason)
{
    CCLOG("[Debug] onUserOffline %d", uid);
    TTTUserControl *theUserControl = getUserControlWithUserID(uid);
    if (theUserControl != nullptr) {
        theUserControl->setUserID(0);
    }
}

void HelloCocosScene::onReportAudioLevel(unsigned int userID, unsigned int audioLevel, unsigned int audioLevelFullRange)
{
    TTTUserControl *theUserControl = getUserControlWithUserID(userID);
    if (theUserControl != nullptr) {
        theUserControl->setVolume(audioLevel);
    }
}

void HelloCocosScene::onReportRtcStats(const RtcStats &stats)
{
}

void HelloCocosScene::onAudioMuted(unsigned int uid, bool muted)
{
}

void HelloCocosScene::onAudioRouteChanged(AUDIO_ROUTE routing)
{
}

void HelloCocosScene::onAudioMixingFinished()
{
}

#include "HomeScene.h"
#include "HelloCocosScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static HomeScene *s_homeScene = nullptr;

HomeScene* HomeScene::getInstance()
{
    if (s_homeScene == nullptr) {
        s_homeScene = HomeScene::create();
    }
    return s_homeScene;
}

HomeScene::~HomeScene()
{
    s_homeScene = nullptr;
}

// on "init" you need to initialize your instance
bool HomeScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backSprite = Sprite::create("login.png");
    float backScaleX = visibleSize.width / backSprite->getContentSize().width;
    float backScaleY = visibleSize.height / backSprite->getContentSize().height;
    backSprite->setScale(backScaleX, backScaleY);
    backSprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 origin.y + visibleSize.height / 2));
    this->addChild(backSprite, 0);
    
    /*
     auto closeItem = MenuItemImage::create(
     "CloseNormal.png",
     "CloseSelected.png",
     CC_CALLBACK_1(HomeScene::menuCloseCallback, this));
     closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
     origin.y + visibleSize.height - closeItem->getContentSize().height / 2));
     
     // create menu, it's an autorelease object
     auto menu = Menu::create(closeItem, NULL);
     menu->setPosition(Vec2::ZERO);
     this->addChild(menu, 1);
     */
    
    auto logoSprite = Sprite::create("logo.png");
    logoSprite->setScale(backScaleY);
    logoSprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 origin.y + visibleSize.height - logoSprite->getContentSize().height * 5 / 2));
    this->addChild(logoSprite, 1);

    auto joinButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    joinButton->setScale(backScaleX);
    float joinButtonWidth  = joinButton->getContentSize().width * joinButton->getScaleX();
    float joinButtonHeight = joinButton->getContentSize().height * joinButton->getScaleY();
    joinButton->setTitleFontSize(8 * (1 / joinButton->getScale()));
    joinButton->setTitleText("进入房间");
    joinButton->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 origin.y + visibleSize.height / 2 - joinButtonHeight * 3 / 2));
    joinButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onJoinButtonClicked();
                break;
            default:
                break;
        }
    });
    this->addChild(joinButton, 1);
    
    auto roomLabel = Label::createWithSystemFont("房间ID：", joinButton->getTitleFontName(), 8);
    roomLabel->setPosition(Vec2(joinButton->getPositionX() - joinButtonWidth / 2 + roomLabel->getContentSize().width / 2,
                                origin.y + visibleSize.height / 2 ));
    this->addChild(roomLabel, 1);
    
    float editBoxWidth = joinButtonWidth - roomLabel->getContentSize().width;
    float editBoxHeight = joinButtonHeight;
    _channelEditBox = ui::EditBox::create(Size(editBoxWidth, editBoxHeight), "TextBox.png");
    _channelEditBox->setFontSize(7);
    _channelEditBox->setFontColor(Color4B(44, 252, 254, 255));
    _channelEditBox->setText(std::to_string(arc4random() % 10000000 + 1).c_str());
    _channelEditBox->setPlaceHolder("请输入房间ID");
    _channelEditBox->setPlaceholderFontSize(7);
    _channelEditBox->setPosition(Vec2(joinButton->getPositionX() + joinButtonWidth / 2 - _channelEditBox->getContentSize().width / 2,
                                      roomLabel->getPositionY()));
    this->addChild(_channelEditBox, 1);

    auto versionLabel = Label::createWithTTF("http://www.3ttech.cn  version 1.0", "fonts/arial.ttf", 8);
    versionLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                   origin.y + versionLabel->getContentSize().height * 5 / 2));
    this->addChild(versionLabel, 1);
    
    auto cocosLabel = Label::createWithTTF("Cocos2D-Aova", "fonts/arial.ttf", 9);
    cocosLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                 versionLabel->getPositionY() + cocosLabel->getContentSize().height * 3 / 2));
    this->addChild(cocosLabel, 1);
    
    _infoLabel = Label::createWithSystemFont("", joinButton->getTitleFontName(), 7);
    _infoLabel->setTextColor(_channelEditBox->getFontColor());
    _infoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                joinButton->getPositionY() - joinButtonHeight * 3 / 2));
    this->addChild(_infoLabel, 1);
    
    scheduleUpdate();

    return true;
}

void HomeScene::onEnter()
{
    Scene::onEnter();
    TTTRtcEngine::getInstance(APP_ID)->setEventHandler(this);
}

void HomeScene::update(float delta)
{
    TTTRtcEngine::getInstance(APP_ID)->poll();
}

void HomeScene::onJoinButtonClicked()
{
    if (_channelEditBox == nullptr || ::strlen(_channelEditBox->getText()) == 0) {
        return;
    }
    
    if (_isJoiningChannel) {
        return;
    }
    _isJoiningChannel = true;
    
    auto rtcEngine = TTTRtcEngine::getInstance(APP_ID);
    rtcEngine->setChannelProfile(CHANNEL_PROFILE_GAME_FREEMODE);
    
    _infoLabel->setTextColor(_channelEditBox->getFontColor());
    _infoLabel->setString("正在进入房间，请稍候 ...");
    rtcEngine->joinChannel("", _channelEditBox->getText(), 0);
}

void HomeScene::menuCloseCallback(Ref* pSender)
{
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application,
     do not use Director::getInstance()->end() and exit(0) as given above,
     instead trigger a custom event created in RootViewController.mm as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    // _eventDispatcher->dispatchEvent(&customEndEvent);
}

void HomeScene::onError(ERROR_CODE code, const char *message)
{
    _isJoiningChannel = false;
    
    std::string errorMessage = message;
    std::stringstream ss;
    ss << "进入房间失败：“" << errorMessage << "”";
    _infoLabel->setTextColor(Color4B::RED);
    _infoLabel->setString(ss.str());
}

void HomeScene::onConnectionLost()
{
    CCLOG("[Debug] onConnectionLost");
}

void HomeScene::onJoinChannelSuccess(const char* channel, unsigned int uid, int elapsed)
{
    _isJoiningChannel = false;
    _infoLabel->setString("");
    
    auto helloCocosScene = HelloCocosScene::getInstance();
    helloCocosScene->setUserID(uid);
    TTTRtcEngine::getInstance(APP_ID)->setEventHandler(helloCocosScene);
    Director::getInstance()->pushScene(helloCocosScene);
}

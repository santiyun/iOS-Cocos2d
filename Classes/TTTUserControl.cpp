//
//  TTTUserControl.cpp
//  TTTRtcEngineCocos
//
//  Created by doubon on 2018/1/16.
//

#include "TTTUserControl.h"

TTTUserControl::TTTUserControl()
{
}

TTTUserControl* TTTUserControl::create(const std::string &bgImageName)
{
    TTTUserControl *userControl = new (std::nothrow) TTTUserControl();
    if (userControl && userControl->initWithBackgroundImage(bgImageName))
    {
        userControl->autorelease();
        return userControl;
    }
    CC_SAFE_DELETE(userControl);
    return nullptr;
}

bool TTTUserControl::initWithBackgroundImage(const std::string &bgImageName)
{
    bool result = this->initWithFile(bgImageName);
    if (result) {
        _volumeSprite = Sprite::create("VolumeSmall.png");
        _volumeSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
        _volumeSprite->setVisible(false);
        this->addChild(_volumeSprite);
        
        _userLabel = Label::createWithTTF("0", "fonts/arial.ttf", 10);
        _userLabel->setPosition(this->getContentSize().width / 2, -_userLabel->getContentSize().height);
        _userLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
        _userLabel->setVisible(false);
        this->addChild(_userLabel);
    }
    return result;
}

void TTTUserControl::setUserID(unsigned int userID) {
    _userID = userID;
    if (userID == 0) {
        this->initWithFile("UserEmpty.png");
        _volumeSprite->setVisible(false);
        _userLabel->setVisible(false);
    }
    else {
        this->initWithFile("User.png");
        _volumeSprite->setVisible(true);
        _userLabel->setVisible(true);
        _userLabel->setString(std::to_string(userID));
    }
}

unsigned int TTTUserControl::getUserID() {
    return _userID;
}

void TTTUserControl::setVolume(unsigned int volume)
{
    _volumeSprite->setVisible(volume > 0);
    if (!_volumeSprite->isVisible()) {
        return;
    }
    
    if (volume > 0 && volume <= 3) {
        _volumeSprite->initWithFile("VolumeSmall.png");
    }
    else if (volume > 3 && volume <= 6) {
        _volumeSprite->initWithFile("VolumeMedium.png");
    }
    else {
        _volumeSprite->initWithFile("VolumeLarge.png");
    }
}

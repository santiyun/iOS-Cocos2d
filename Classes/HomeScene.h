#ifndef __HOMESCENE_H__
#define __HOMESCENE_H__

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
USING_NS_CC;

#include "../TTTRtcEngineVoice/include/TTTRtcEngineVoice.h"
using namespace TTTRtc;

#define APP_ID "a967ac491e3acf92eed5e1b5ba641ab7"

class HomeScene : public cocos2d::Scene, public TTTRtcEngineEventHandler
{
private:
    ~HomeScene();
    
    ui::EditBox *_channelEditBox = nullptr;
    Label *_infoLabel = nullptr;
    bool _isJoiningChannel = false;
    
    void menuCloseCallback(cocos2d::Ref *pSender);
    void onJoinButtonClicked();
public:
    CREATE_FUNC(HomeScene);
    static HomeScene* getInstance();

    virtual bool init() override;
    virtual void onEnter() override;
    virtual void update(float delta) override;

    void onError(ERROR_CODE code, const char *message) override;
    void onConnectionLost() override;
    void onJoinChannelSuccess(const char *channel, unsigned int uid, int elapsed) override;
};

#endif // __HOMESCENE_H__

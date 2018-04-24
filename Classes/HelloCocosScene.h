#ifndef __HELLOCOCOS_SCENE_H__
#define __HELLOCOCOS_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "../TTTRtcEngineVoice/include/TTTRtcEngineVoice.h"
using namespace TTTRtc;

#include "TTTUserControl.h"

class HelloCocosScene : public cocos2d::Scene, public TTTRtcEngineEventHandler
{
private:
    virtual ~HelloCocosScene();
    
    TTTUserControl *_myUserControl = nullptr;
    std::vector<TTTUserControl *> _userControls;
    
    TTTUserControl* getUserControlWithUserID(unsigned int userID);
    TTTUserControl* getAvailableUserControl();
    void onLeaveButtonClicked();
public:
    CREATE_FUNC(HelloCocosScene);
    static HelloCocosScene* getInstance();
    
    void setUserID(unsigned int userID);
    unsigned int getUserID();

    virtual bool init() override;
    virtual void update(float delta) override;

    void onError(ERROR_CODE code, const char *message) override;
    void onConnectionLost() override;
    void onLeaveChannel(const RtcStats &stats) override;
    void onUserJoined(unsigned int uid, int elapsed) override;
    void onUserOffline(unsigned int uid, USER_OFFLINE_REASON reason) override;
    void onReportAudioLevel(unsigned int userID, unsigned int audioLevel, unsigned int audioLevelFullRange) override;
    void onReportRtcStats(const RtcStats &stats) override;
    void onAudioMuted(unsigned int uid, bool muted) override;
    void onAudioRouteChanged(AUDIO_ROUTE routing) override;
    void onAudioMixingFinished() override;
};

#endif // __HELLOCOCOS_SCENE_H__

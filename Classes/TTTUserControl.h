//
//  TTTUserControl.h
//  TTTRtcEngineCocos
//
//  Created by doubon on 2018/1/16.
//

#ifndef TTTUserControl_h
#define TTTUserControl_h

#include "cocos2d.h"
USING_NS_CC;

class TTTUserControl : public cocos2d::Sprite
{
private:
    unsigned int _userID = 0;
    Sprite *_volumeSprite = nullptr;
    Label *_userLabel = nullptr;
protected:
    bool initWithBackgroundImage(const std::string &bgImageName);
public:
    TTTUserControl();
    static TTTUserControl *create(const std::string &bgImageName);
    
    void setUserID(unsigned int userID);
    unsigned int getUserID();
    
    void setVolume(unsigned int volume);
};

#endif /* TTTUserControl_h */

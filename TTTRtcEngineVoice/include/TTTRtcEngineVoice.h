#ifndef TTTRTCENGINEVOICE_H
#define TTTRTCENGINEVOICE_H

#include <string>
#include <vector>

#if defined(__APPLE__)
#define TTT_API __attribute__((visibility("default"))) extern "C"
#elif defined(__ANDROID__) || defined(__linux__) || defined(__linux)
#define TTT_API extern "C" __attribute__((visibility("default")))
#else
#define TTT_API extern "C"
#endif

namespace TTTRtc {
    enum ERROR_CODE
    {
        ERROR_INVALID_CHANNEL_NAME = 9000,
        ERROR_ENTER_TIMEOUT        = 9001,
        ERROR_ENTER_FAILED         = 9002,
        ERROR_ENTER_VERIFYFAILED   = 9003,
        ERROR_ENTER_BADVERSION     = 9004,
        ERROR_ENTER_UNKNOWN        = 9005,
    };
    
    enum LOG_FILTER
    {
        LOG_FILTER_OFF      = 0,
        LOG_FILTER_DEBUG    = 0x080f,
        LOG_FILTER_INFO     = 0x000f,
        LOG_FILTER_WARNING  = 0x000e,
        LOG_FILTER_ERROR    = 0x000c,
    };

    enum CHANNEL_PROFILE {
        CHANNEL_PROFILE_COMMUNICATION    = 0, // 通信
        CHANNEL_PROFILE_LIVEBROADCASTING = 1, // 直播
        CHANNEL_PROFILE_GAME_FREEMODE    = 2, // 游戏（自由发言模式）
    };
    
    enum CLIENT_ROLE
    {
        CLIENT_ROLE_BROADCASTER = 1,
        CLIENT_ROLE_AUDIENCE    = 2,
    };
    
    enum AUDIO_ROUTE
    {
        AUDIO_ROUTE_HEADSET   = 0, // 耳机或蓝牙
        AUDIO_ROUTE_SPEAKERE  = 1, // 扬声器
        AUDIO_ROUTE_HEADPHONE = 2, // 手机听筒
    };
    
    enum USER_OFFLINE_REASON
    {
        USER_OFFLINE_QUIT = 0,
        USER_OFFLINE_DROPPED = 1,
        USER_OFFLINE_BECOME_AUDIENCE = 2,
    };
    
    struct RtcStats
    {
        unsigned int   duration;
        unsigned int   txBytes;
        unsigned int   rxBytes;
        unsigned short txAudioKBitRate;
        unsigned short rxAudioKBitRate;
        unsigned int   users;
    };
    
    class TTTRtcEngineEventHandler;
    
    class TTTRtcEngine
    {
    private:
        TTTRtcEngine(const char *appID);
        virtual ~TTTRtcEngine();

        TTTRtcEngineEventHandler *_eventHandler;
    public:
        static TTTRtcEngine* getInstance(const char *appID);
        static void destroy();

        /**
         *  获取SDK版本号
         *
         *  @return SDK版本号
         */
        static const char *getVersion();

        /**
         *  获取错误描述
         *  @param code 错误代码
         *
         *  @return 错误代码对应的错误描述
         */
        static const char *getErrorDescription(ERROR_CODE code);

        /**
         *  设置回调处理句柄
         */
        virtual void setEventHandler(TTTRtcEngineEventHandler *eventHandler);

        /**
         *  设置频道模式
         *
         *  @param profile 频道模式
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setChannelProfile(CHANNEL_PROFILE profile);

        /**
         *  设置用户角色
         *
         *  @param role          用户角色
         *  @param permissionKey 连麦鉴权密钥
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setClientRole(CLIENT_ROLE role, const char *permissionKey);

        /**
         *  设置日志文件过滤器
         *
         *  @param filter 日志过滤器
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setLogFilter(LOG_FILTER filter);

        /**
         *  设置SDK输出的日志文件 应用程序必须保证指定的目录存在而且可写
         *
         *  @param filePath 日志文件的完整路径。该日志文件为UTF-8编码。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setLogFile(const char *filePath);

        /**
         *  允许/禁止往网络发送本地音频流
         *
         *  @param mute true: 麦克风静音，false: 取消静音。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int muteLocalAudioStream(bool mute);

        /**
         *  允许/禁止播放远端用户的音频流，即对所有远端用户进行静音与否。
         *
         *  @param mute true: 停止播放所有收到的音频流，false: 允许播放所有收到的音频流。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int muteAllRemoteAudioStreams(bool mute);

        /**
         *  允许/禁止播放远端用户的音频流
         *
         *  @param uid 用户 ID
         *  @param mute true: 停止播放指定用户的音频流，false: 允许播放指定用户的音频流。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int muteRemoteAudioStream(unsigned int uid, bool mute);

        /**
         *  打开外放(扬声器)
         *
         *  @param speakerphone true: 切换到从外放(扬声器)出声，false: 语音会根据默认路由出声。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setEnableSpeakerphone(bool speakerphone);

        /**
         *  修改默认的语音路由
         *
         *  @param speakerphone true: 默认路由改为外放(扬声器)，false: 默认路由改为听筒。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int setDefaultAudioRouteToSpeakerphone(bool speakerphone);

        /**
         *  启用/禁用说话者音量提示
         *
         *  @param interval 指定音量提示的时间间隔（<=0: 禁用音量提示功能；>0: 提示间隔，单位为毫秒。建议设置到大于200毫秒。）
         *  @param smooth   平滑系数。默认可以设置为3。
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int enableAudioVolumeIndication(int interval, int smooth);

        /**
         *  开始客户端本地混音
         *
         *  @param filePath 指定需要混音的本地音频文件名和文件路径
         *  @param loopback true: 只有本地可以听到混音或替换后的音频流，false: 本地和对方都可以听到混音或替换后的音频流。
         *  @param replace  true: 音频文件内容将会替换本地录音的音频流，false: 音频文件内容将会和麦克风采集的音频流进行混音。
         *  @param cycle    指定音频文件循环播放的次数
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int startAudioMixing(const char *filePath, bool loopback, bool replace, int cycle);

        /**
         *  停止客户端本地混音
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int stopAudioMixing();

        /**
         *  暂停播放伴奏
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int pauseAudioMixing();

        /**
         *  恢复播放伴奏
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int resumeAudioMixing();

        /**
         * adjust mixing audio file volume
         *
         * @param volume range from 0 to 100
         * @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int adjustAudioMixingVolume(int volume);

        /**
         * get the duration of the specified mixing audio file
         *
         * @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int getAudioMixingDuration();

        /**
         * get the current playing position of the specified mixing audio file
         *
         * @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int getAudioMixingCurrentPosition();

        /**
         *  加入频道，在同一个频道内的用户可以互相通话。
         *
         *  @param channelName 标识通话的频道名称
         *  @param uid  用户 ID
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int joinChannel(const char *channelKey, const char *channelName, unsigned int uid);

        /**
         *  离开频道
         *
         *  @return 0: 方法调用成功，<0: 方法调用失败。
         */
        virtual int leaveChannel();

        /**
         * trigger the SDK event working according to vertical synchronization such as onUpdate(Cocos2d)
         *
         */
        virtual void poll();
    private:
        static std::vector<std::string> splitMsg(const std::string &msg, char delim);
    };

    /**
     *  事件回调接口
     */
    class TTTRtcEngineEventHandler
    {
    public:
        virtual ~TTTRtcEngineEventHandler() {}
        
        /**
         *  发生错误
         *
         *  @param code    错误代码
         *  @param message 错误描述
         */
        virtual void onError(ERROR_CODE code, const char *message) {}
        
        /**
         *  网络连接丢失
         */
        virtual void onConnectionLost() {}
        
        /**
         *  加入频道成功
         *
         *  @param channel 频道名
         *  @param uid     用户ID
         *  @param elapsed 从joinChannel开始到该事件产生的延迟（毫秒）
         */
        virtual void onJoinChannelSuccess(const char *channel, unsigned int uid, int elapsed) {}
        
        /**
         *  成功离开频道
         *
         *  @param stats 统计数据
         */
        virtual void onLeaveChannel(const RtcStats &stats) {}
        
        /**
         *  用户加入
         *
         *  @param uid     用户ID
         *  @param elapsed 加入频道开始到该回调触发的延迟（毫秒)
         */
        virtual void onUserJoined(unsigned int uid, int elapsed) {}
        
        /**
         *  用户离线
         *
         *  @param uid    用户ID
         *  @param reason 离线原因
         */
        virtual void onUserOffline(unsigned int uid, USER_OFFLINE_REASON reason) {}
        
        /**
         *  上报远端用户音量
         *  提示谁在说话及其音量，默认禁用。可通过enableAudioVolumeIndication方法设置。
         *
         *  @param userID              用户ID
         *  @param audioLevel          非线性区间[0,9]
         *  @param audioLevelFullRange 线性区间[0,32768]
         */
        virtual void onReportAudioLevel(unsigned int userID, unsigned int audioLevel, unsigned int audioLevelFullRange) {}
        
        /**
         *  上报统计数据
         *
         *  @param stats 统计数据
         */
        virtual void onReportRtcStats(const RtcStats &stats) {}

        /**
         *  用户音频静音
         *
         *  @param uid   用户ID
         *  @param muted true: 静音，false: 取消静音。
         */
        virtual void onAudioMuted(unsigned int uid, bool muted) {}
        
        /**
         *  音频输出路由发生变化
         *
         *  @param routing 当前音频输出路由
         */
        virtual void onAudioRouteChanged(AUDIO_ROUTE routing) {}
        
        /**
         *  客户端本地混音完成
         */
        virtual void onAudioMixingFinished() {}
    };
}

#endif

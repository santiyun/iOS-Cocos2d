#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 *  错误代码
 */
typedef NS_ENUM(NSInteger, TTTRtcErrorCode) {
    //进入房间错误
    TTTRtc_Error_InvalidChannelName = 9000, // 无效的房间名称
    TTTRtc_Error_Enter_TimeOut      = 9001, // 超时,10秒未收到服务器返回结果
    TTTRtc_Error_Enter_Failed       = 9002, // 无法连接服务器
    TTTRtc_Error_Enter_VerifyFailed = 9003, // 验证码错误
    TTTRtc_Error_Enter_BadVersion   = 9004, // 版本错误
    TTTRtc_Error_Enter_Unknown      = 9005, // 未知错误
};

/**
 *  踢出房间原因
 */
typedef NS_ENUM(NSInteger, TTTRtcKickedOutReason) {
    TTTRtc_KickedOut_KickedByHost      = 1, // 被主播踢出
    TTTRtc_KickedOut_PushRtmpFailed    = 2, // rtmp推流失败
    TTTRtc_KickedOut_ServerOverload    = 3, // 服务器过载
    TTTRtc_KickedOut_MasterExit        = 4, // 主播已退出
    TTTRtc_KickedOut_ReLogin           = 5, // 重复登录
    TTTRtc_KickedOut_NoAudioData       = 6, // 长时间没有上行音频数据
    TTTRtc_KickedOut_NoVideoData       = 7, // 长时间没有上行视频数据
    TTTRtc_KickedOut_NewChairEnter     = 8, // 其他人以主播身份进入
    TTTRtc_KickedOut_ChannelKeyExpired = 9, // Channel Key失效
};

/**
 *  频道模式
 */
typedef NS_ENUM(NSInteger, TTTRtcChannelProfile) {
    TTTRtc_ChannelProfile_Communication    = 0, // 通信
    TTTRtc_ChannelProfile_LiveBroadcasting = 1, // 直播
    TTTRtc_ChannelProfile_Game_FreeMode    = 2, // 游戏（自由发言模式）
};

/**
 *  用户角色
 */
typedef NS_ENUM(NSInteger, TTTRtcClientRole) {
    TTTRtc_ClientRole_Anchor      = 0, // 主播
    TTTRtc_ClientRole_Broadcaster = 1, // 副播
    TTTRtc_ClientRole_Audience    = 2, // 观众(默认)
};

/**
 *  用户离线原因
 */
typedef NS_ENUM(NSUInteger, TTTRtcUserOfflineReason) {
    TTTRtc_UserOffline_Quit           = 0, // 用户主动离开
    TTTRtc_UserOffline_Dropped        = 1, // 因过长时间收不到对方数据包，超时掉线。
    TTTRtc_UserOffline_BecomeAudience = 2, // 当用户身份从主播切换为观众时触发
};

/**
 *  音频输出路由
 */
typedef NS_ENUM(NSInteger, TTTRtcAudioOutputRouting)
{
    TTTRtc_AudioOutput_Headset   = 0, //耳机或蓝牙
    TTTRtc_AudioOutput_Speaker   = 1, //扬声器
    TTTRtc_AudioOutput_Headphone = 2  //手机听筒
};

/**
 *  日志过滤器
 */
typedef NS_ENUM(NSUInteger, TTTRtcLogFilter) {
    TTTRtc_LogFilter_Off      = 0,
    TTTRtc_LogFilter_Debug    = 0x080f,
    TTTRtc_LogFilter_Info     = 0x000f,
    TTTRtc_LogFilter_Warning  = 0x000e,
    TTTRtc_LogFilter_Error    = 0x000c,
};

/**
 *  聊天消息类型
 */
typedef NS_ENUM(NSUInteger, TTTRtcChatMessageType) {
    TTTRtc_ChatMessageType_Text    = 1,
    TTTRtc_ChatMessageType_Picture = 2,
    TTTRtc_ChatMessageType_Audio   = 3
};

/**
 *  会话数据信息
 */
@interface TTTRtcStats : NSObject
@property (assign, nonatomic) NSUInteger duration;        // 通话时长，累计值
@property (assign, nonatomic) NSUInteger txBytes;         // 发送字节数，累计值
@property (assign, nonatomic) NSUInteger rxBytes;         // 接收字节数，累计值
@property (assign, nonatomic) NSUInteger txAudioKBitrate;
@property (assign, nonatomic) NSUInteger rxAudioKBitrate;
@property (assign, nonatomic) NSUInteger users;
@end

/**
 本地音频统计信息
 */
@interface TTTRtcLocalAudioStats : NSObject
@property (assign, nonatomic) NSUInteger encodedBitrate;  // 编码的码率(kbps)
@property (assign, nonatomic) NSUInteger sentBitrate;     // 发送的码率(kbps)
@property (assign, nonatomic) NSUInteger receivedBitrate; // 接收的码率(kbps)
@end

/**
 远端音频统计信息
 */
@interface TTTRtcRemoteAudioStats : NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger receivedBitrate;
@end

@protocol TTTRtcEngineDelegate;

@interface TTTRtcEngineKit : NSObject

/**
 *  SDK回调对象，由调用方赋值。
 */
@property (nonatomic, weak) id<TTTRtcEngineDelegate> delegate;

/**
 *  初始化SDK，加载资源。
 *
 *  @param appId    应用ID，由连麦平台分配，用于区分不同的客户和应用，在同一个连麦平台内保证唯一。
 *  @param delegate SDK回调代理
 *
 *  @return TTTRtc 对象(单例)
 */
+ (instancetype)sharedEngineWithAppId:(NSString *)appId
                             delegate:(id<TTTRtcEngineDelegate>)delegate;

/**
 *  销毁引擎实例
 */
+ (void)destroy;

/**
 *  获取SDK版本号
 */
+ (NSString *)getSdkVersion;

/**
 *  设置服务器地址
 * 
 *  @param ip   ip地址或域名
 *  @param port 端口
 */
- (void)setServerIp:(NSString*)ip port:(int)port;

/**
 *  设置频道模式
 *
 *  @param profile 频道模式
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setChannelProfile:(TTTRtcChannelProfile)profile;

/**
 *  设置用户角色
 *
 *  @param role          直播场景里的用户角色
 *  @param permissionKey 连麦鉴权密钥
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setClientRole:(TTTRtcClientRole)role withKey: (NSString *)permissionKey;

/**
 *  加入通话频道
 *
 *  @param channelKey        此为程序生成的Channel Key（当用户使用静态Key也即只使用App ID时, 该参数是可选的）
 *  @param channelName       标识通话的频道名称，长度在64字节以内的字符串。
 *  @param uid               用户ID
 *  @param joinSuccessBlock  用户加入成功回调
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)joinChannelByKey:(NSString *)channelKey
            channelName:(NSString *)channelName
                    uid:(NSUInteger)uid
            joinSuccess:(void(^)(NSString* channel, NSUInteger uid, NSInteger elapsed))joinSuccessBlock;

/**
 *  离开频道，即挂断或退出通话。
 *
 *  @param leaveChannelBlock 成功离开频道的回调
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)leaveChannel:(void(^)(TTTRtcStats *stat))leaveChannelBlock;

/**
 *  静音/取消静音。该方法用于允许/禁止往网络发送本地音频流。
 *
 *  @param mute YES: 麦克风静音, NO: 取消静音。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteLocalAudioStream:(BOOL)mute;

/**
 *  静音所有远端音频/对所有远端音频取消静音。该方法用于允许/禁止播放远端用户的音频流，即对所有远端用户进行静音与否。
 *
 *  @param mute YES: 停止播放所接收的音频流, NO: 恢复播放所接收的音频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteAllRemoteAudioStreams:(BOOL)mute;

/**
 *  静音指定远端用户/对指定远端用户取消静音。本方法用于允许/禁止播放远端用户的音频流。
 *
 *  @param mute YES: 停止播放指定用户的音频流, NO: 恢复播放指定用户的音频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteRemoteAudioStream:(NSUInteger)uid mute:(BOOL)mute;

/**
 *  禁言指定远端用户/对指定远端用户取消禁言。
 *
 *  @param mute YES: 禁止发言, NO: 允许发言。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteRemoteSpeaking:(NSUInteger)uid mute:(BOOL)mute;

/**
 *  切换音频输出方式：扬声器或听筒
 *
 *  @param enableSpeaker YES: 音频输出至扬声器，NO: 语音会根据默认路由出声。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setEnableSpeakerphone:(BOOL)enableSpeaker;

/**
 *  是否是扬声器状态
 *
 *  @return YES: 表明输出到扬声器，NO: 表明输出到非扬声器(听筒，耳机等)。
 */
- (BOOL)isSpeakerphoneEnabled;

/**
 *  设置默认的语音路由
 *
 *  @param defaultToSpeaker YES: 从扬声器出声，NO: 语音聊天：从听筒出声；视频聊天：从扬声器出声。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setDefaultAudioRouteToSpeakerphone:(BOOL)defaultToSpeaker;

/**
 *  启用说话者音量提示
 *
 *  @param interval 指定音量提示的时间间隔（<=0: 禁用音量提示功能；>0: 提示间隔，单位为毫秒。建议设置到大于200毫秒。）
 *  @param smooth   平滑系数。默认可以设置为3。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableAudioVolumeIndication:(NSInteger)interval smooth:(NSInteger)smooth;

/**
 *  设置日志文件
 *
 *  @param filePath 日志文件的完整路径。该日志文件为UTF-8编码。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setLogFile:(NSString*)filePath;

/**
 *  设置日志文件过滤器
 *
 *  @param filter 日志过滤器
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setLogFilter:(TTTRtcLogFilter)filter;

/**
 *  设置音频高音质选项
 *
 *  @param fullband    全频带编解码器（48kHz采样率）
 *  @param stereo      立体声编解码器
 *  @param fullBitrate 高码率模式，建议仅在纯音频模式下使用。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setHighQualityAudioParametersWithFullband:(BOOL)fullband stereo:(BOOL)stereo fullBitrate:(BOOL)fullBitrate;

/**
 *  开始客户端本地混音
 *
 *  @param filePath 指定需要混音的本地音频文件名和文件路径
 *  @param loopback True: 只有本地可以听到混音或替换后的音频流，False: 本地和对方都可以听到混音或替换后的音频流。
 *  @param replace  True: 音频文件内容将会替换本地录音的音频流，False: 音频文件内容将会和麦克风采集的音频流进行混音。
 *  @param cycle    指定音频文件循环播放的次数
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startAudioMixing:(NSString *)filePath loopback:(BOOL)loopback replace:(BOOL)replace cycle:(NSInteger)cycle;

/**
 *  停止客户端本地混音
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopAudioMixing;

/**
 *  暂停播放伴奏
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)pauseAudioMixing;

/**
 *  恢复播放伴奏
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)resumeAudioMixing;

/**
 *  调节伴奏音量
 *
 *  @param volume 伴奏音量范围为0~100。默认100为原始文件音量。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)adjustAudioMixingVolume:(NSInteger)volume;

/**
 *  获取伴奏时长
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)getAudioMixingDuration;

/**
 *  获取伴奏播放进度
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)getAudioMixingCurrentPosition;

/**
 *  拖动语音进度条
 *
 *  @param pos 进度条位置，单位为毫秒。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setAudioMixingPosition:(NSInteger)pos;

/**
 *  踢出房间
 *  角色为“TTTRtc_ClientRole_Anchor”调用有效
 *
 *  @param uid 被踢者userID
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)kickChannelUser:(NSUInteger)uid;

/**
 *  发送聊天消息
 *
 *  @param roomID 房间ID
 *  @param userID 用户ID
 *  @param seqID  唯一标识
 *  @param data   消息内容
 *  @param length 消息长度
 */
- (void)sendChatMessageWithRoomID:(NSUInteger)roomID userID:(NSUInteger)userID chatType:(TTTRtcChatMessageType)type seqID:(NSString *)seqID
                             data:(NSString *)data length:(NSUInteger)length;

@end

/**
 *  SDK回调代理，由调用方实现。
 */
@protocol TTTRtcEngineDelegate <NSObject>

@optional

/**
 *  发生错误回调
 *  通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要应用程序干预或提示用户。
 *
 *  @param errorCode 错误代码
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didOccurError:(TTTRtcErrorCode)errorCode;

/**
 *  网络连接丢失回调
 */
- (void)rtcEngineConnectionDidLost:(TTTRtcEngineKit *)engine;

/**
 *  加入频道成功回调
 *
 *  @param channel 频道名
 *  @param uid     用户ID
 *  @param elapsed 从joinChannel开始到该事件产生的延迟（毫秒）
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didJoinChannel:(NSString*)channel withUid:(NSUInteger)uid elapsed:(NSInteger) elapsed;

/**
 *  成功离开频道的回调
 *
 *  @param stats 统计数据
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didLeaveChannelWithStats:(TTTRtcStats*)stats;

/**
 *  用户加入回调
 *
 *  @param uid        用户ID
 *  @param clientRole 用户角色
 *  @param elapsed    加入频道开始到该回调触发的延迟（毫秒)
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didJoinedOfUid:(NSUInteger)uid clientRole:(TTTRtcClientRole)clientRole elapsed:(NSInteger)elapsed;

/**
 *  用户离线回调
 *
 *  @param uid    用户ID
 *  @param reason 离线原因
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didOfflineOfUid:(NSUInteger)uid reason:(TTTRtcUserOfflineReason)reason;

/**
 *  用户被踢出回调
 *
 *  @param uid    用户ID
 *  @param reason 用户被踢出的原因
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didKickedOutOfUid:(NSUInteger)uid reason:(TTTRtcKickedOutReason)reason;

/**
 *  禁止/允许用户发言回调
 *
 *  @param muted YES: 禁止发言，NO: 允许发言。
 *  @param uid   用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didSpeakingMuted:(BOOL)muted ofUid:(NSUInteger)uid;

/**
 *  用户音频静音回调
 *
 *  @param muted YES: 静音，NO: 取消静音。
 *  @param uid   用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didAudioMuted:(BOOL)muted byUid:(NSUInteger)uid;

/**
 *  音频输出路由发生变化
 *
 *  @param routing 当前音频输出路由
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didAudioRouteChanged:(TTTRtcAudioOutputRouting)routing;

/**
 *  本地音频统计回调
 *
 *  @param stats 本地音频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine localAudioStats:(TTTRtcLocalAudioStats*)stats;

/**
 *  远端音频统计回调
 *
 *  @param stats 远端音频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteAudioStats:(TTTRtcRemoteAudioStats*)stats;

/**
 *  远端用户音量回调
 *  提示谁在说话及其音量，默认禁用。可通过enableAudioVolumeIndication方法设置。
 *
 *  @param userID              用户ID
 *  @param audioLevel          非线性区间[0,9]
 *  @param audioLevelFullRange 线性区间[0,32768]
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine reportAudioLevel:(NSUInteger)userID
       audioLevel:(NSUInteger)audioLevel audioLevelFullRange:(NSUInteger)audioLevelFullRange;

/**
 *  发送聊天消息的回调
 *
 *  @param seqID     唯一标识
 *  @param errorCode 错误代码
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onChatMessageSentOfSeqID:(NSString *)seqID errorCode:(TTTRtcErrorCode)errorCode;

/**
 *  收到聊天消息的回调
 *
 *  @param userID 用户ID
 *  @param seqID  唯一标识
 *  @param data   消息内容
 *  @param length 消息长度
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onChatMessageReceivedOfUserID:(NSUInteger)userID chatType:(TTTRtcChatMessageType)type seqID:(NSString *)seqID
             data:(NSString *)data length:(NSUInteger)length;

@end

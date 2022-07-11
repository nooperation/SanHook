#pragma once

#include <cstdint>

class PacketReader;

extern uint32_t myControllerId;
extern uint32_t mySessionId;
extern uint64_t myComponentId;
extern uint64_t targetComponentId;
extern uint64_t targetFollowComponentId;
extern std::string targetHandle;
extern int followMode;
extern bool isUsingTargetAsSpawnPosition;

extern float *CameraPositionOffset;
extern std::unordered_map<uint32_t, uint64_t> sessionToComponentIdMap;
extern std::unordered_map<std::string, uint32_t> handleToSessionIdMap;

extern "C"
{
    extern float *AvatarPositionOffset;
}

class MessageHandler
{
protected:
    bool _isSender = false;
    bool _isVerbose = false;


public:
    void SetIsSending(bool isSender)
    {
        this->_isSender = isSender;
    }

    virtual bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending) = 0;
};

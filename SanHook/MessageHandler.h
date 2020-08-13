#pragma once

#include <cstdint>

class PacketReader;

extern uint32_t myControllerId;
extern uint32_t mySessionId;
extern uint64_t myComponentId;

class MessageHandler
{
protected:
    bool _isSender = false;


public:
    void SetIsSending(bool isSender)
    {
        this->_isSender = isSender;
    }

    virtual bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending) = 0;
};

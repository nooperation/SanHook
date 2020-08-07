#pragma once

#include <cstdint>

class PacketReader;

class MessageHandler
{
public:
    virtual bool OnMessage(uint32_t messageId, PacketReader &reader) = 0;
};

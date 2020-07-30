#pragma once

extern "C"
{
    void intercept_ProcessPacketRecv();
    void ProcessPacketRecv(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessPacketRecv;
};

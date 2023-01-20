#pragma once

extern "C"
{
    void intercept_ProcessPacketRecv();
    void ProcessPacketRecv(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessPacketRecv;

    void intercept_ProcessPacketSend();
    void ProcessPacketSend(uint8_t *packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessPacketSend;

    void intercept_ProcessPacketSendB();
    void ProcessPacketSendB(uint8_t *packet, uint64_t length);

    void intercept_ProcessHttpBodyRecv();
    void ProcessHttpBodyRecv(char *packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessHttpBodyRecv;

    void intercept_ProcessHttpSend();
    void ProcessHttpSend(char *packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessHttpSend;


    void intercept_ProcessPositionUpdate();
    void ProcessPositionUpdate(float *avatarPosition);
    extern unsigned long long ReturnPoint_ProcessPositionUpdate;
    float *AvatarPositionOffset;
    //extern float *AvatarPositionOffset;


    void intercept_ProcessBodyCinfo();
    void ProcessBodyCinfo(uint8_t *clusterPtrAtBodyCInfo, uint8_t *packet, uint64_t length);
    extern unsigned long long ReturnPoint_ProcessBodyCinfo;
};


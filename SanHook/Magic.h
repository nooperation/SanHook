#pragma once

extern "C"
{
    void intercept_client_region_message();
    void ProcessClientRegionMessage(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint;

    void intercept_HandleUpdateNotificationMessage();
    void ProcessUpdateNotificationMessage(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint_HandleUpdateNotificationMessage;

    void intercept_HandleChatMessage();
    void ProcessChatMessage(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint_HandleChatMessage;

    void intercept_HandleScriptConsoleStuff();
    void ProcessScriptConsoleStuff(uint64_t MessageId, uint8_t* packet, uint64_t length);
    extern unsigned long long ReturnPoint_HandleScriptConsoleStuff;
};

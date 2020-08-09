#include <winsock2.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <map>

#pragma comment(lib, "ws2_32.lib")

static int (WINAPI *original_recvfrom)(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen) = recvfrom;
static int (WINAPI *original_sendto)(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen) = sendto;
int (WINAPI *original_recv)(SOCKET s, char *buf, int len, int flags) = recv;
int (WINAPI *original_send)(SOCKET s, const char *buf, int len, int flags) = send;

std::string GetAddressFromAddr(const sockaddr *addr);

#include "bitReader.hpp"
#include "Messages.h"
#include "PacketReader.hpp"

#include "ClientKafka.h"
#include "AnimationComponent.h"
#include "GameWorld.h"
#include "AgentController.h"
#include "Audio.h"
#include "Simulation.h"
#include "Render.h"
#include "EditServer.h"
#include "ClientVoice.h"
#include "WorldState.h"
#include "RegionRegion.h"
#include "ClientRegion.h"
#include "Utils.hpp"

#include "Magic.h"
unsigned long long ReturnPoint_ProcessPacketRecv = 0;
unsigned long long ReturnPoint_ProcessPacketSend = 0;

std::string GetAddressFromAddr(const sockaddr *addr)
{
    if (!addr)
    {
        return "NULL";
    }

    static std::map<uint64_t, std::string> addr_lookup;
    auto cached = addr_lookup.find((uint64_t)addr);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    int port = 0;
    char address[INET6_ADDRSTRLEN];

    if (addr->sa_family == AF_INET)
    {
        auto s = (sockaddr_in *)addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, address, sizeof(address));
    }
    else
    {
        auto s = (sockaddr_in6 *)addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, address, sizeof(address));
    }

    auto result = std::string(address) + " port " + std::to_string(port);
    // addr_lookup[(uint64_t)addr] = result;

    return result;
}

std::string GetAddressFromSocket(SOCKET s)
{
    static std::map<SOCKET, std::string> addr_lookup;
    auto cached = addr_lookup.find(s);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    sockaddr_storage addr = {};
    socklen_t namelen = sizeof(addr);

    auto result = getpeername(s, (sockaddr *)&addr, &namelen);

    auto name = GetAddressFromAddr((const sockaddr *)&addr);
    addr_lookup[s] = name;
    return name;
}

int WINAPI Hooked_Recvfrom(SOCKET s, char *buff, int len, int flags, struct sockaddr *from, int *fromlen)
{
    auto result = original_recvfrom(s, buff, len, flags, from, fromlen);
    if (result == SOCKET_ERROR)
    {
        return result;
    }

    return result;
}

int WINAPI Hooked_Sendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
    if (buf == nullptr)
    {
        return original_sendto(s, buf, len, flags, to, tolen);
    }

    //  Utils::DumpPacket(buf, len, true);
 
    auto result = original_sendto(s, buf, len, flags, to, tolen);
    return result;
}


int WINAPI Hooked_Recv(SOCKET s, char *buf, int len, int flags)
{
    auto result = original_recv(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("hooked recv [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        //Utils::DumpPacket(buf, result, false);
    }

    return result;
}


int WINAPI Hooked_Send(SOCKET s, const char *buf, int len, int flags)
{
    auto result = original_send(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("Hooked send [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        Utils::DumpPacket(buf, len, true);
    }

    return result;
}



void RewriteCode(void *targetAddress, uint8_t *newCode, std::size_t newCodeLength)
{
    DWORD oldProtection = 0;
    if (!VirtualProtect(targetAddress, newCodeLength, PAGE_EXECUTE_READWRITE, &oldProtection))
    {
        printf("Failed to VirtualProtect address...\n");
    }

    printf("Writing %lld bytes to %llX...\n", newCodeLength, (uint64_t)targetAddress);
    auto result = memcpy(targetAddress, newCode, newCodeLength);

    if (!VirtualProtect(targetAddress, newCodeLength, oldProtection, &oldProtection))
    {
        printf("Failed to restore VirtualProtect protection...\n");
    }
}

std::vector<std::unique_ptr<MessageHandler>> messageHandlers;

void ProcessPacketSend(uint8_t *packet, uint64_t length)
{
    //printf("ProcessPacketSend: Length = %lld | packet = %llX\n", length, (uint64_t)packet);

    try
    {
        PacketReader reader(packet, length);
        auto messageId = reader.ReadUint32();
    
        //printf("SEND: messageid = %X\n", messageId);

        auto handled_packet = false;
    
        for (auto &item : messageHandlers)
        {
            if (item->OnMessage(messageId, reader, true))
            {
                handled_packet = true;
                break;
            }
        }
    
        if (!handled_packet)
        {
            printf("ProcessPacketSend: Unhandled message %X\n", messageId);
        }
    }
    catch (std::exception ex)
    {
        printf("!!! ProcessPacketSend Caught exception -> %s\n", ex.what());
    }
}



void ProcessPacketRecv(uint64_t messageId, uint8_t *packet, uint64_t length)
{
    // printf("ProcessPacketRecv: MessageId = %llX | Length = %lld | packet = %llX\n", messageId, length, (uint64_t)packet);

    try
    {
        PacketReader reader(packet, length);
        messageId = reader.ReadUint32();

        auto handled_packet = false;

        for (auto &item : messageHandlers)
        {
            if (item->OnMessage(messageId, reader, false))
            {
                handled_packet = true;
                break;
            }
        }

        if(!handled_packet)
        {
            printf("ProcessPacketRecv: Unhandled message %llX\n", messageId);
        }

        // "Illegally formatted message received. You probably failed to bind a callback for this message. Message was: %s (id: 0x%x)\n"'
    }
    catch (std::exception ex)
    {
        printf("!!! Caught exception -> %s\n", ex.what());
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        FILE *pFile = nullptr;
        AllocConsole();
        freopen_s(&pFile, "CONOUT$", "w", stdout);

        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");

        messageHandlers.emplace_back(std::make_unique<AgentController>());
        messageHandlers.emplace_back(std::make_unique<AnimationComponent>());
        messageHandlers.emplace_back(std::make_unique<Audio>());
        messageHandlers.emplace_back(std::make_unique<ClientKafka>());
        messageHandlers.emplace_back(std::make_unique<ClientRegion>());
        messageHandlers.emplace_back(std::make_unique<ClientVoice>());
        messageHandlers.emplace_back(std::make_unique<EditServer>());
        messageHandlers.emplace_back(std::make_unique<GameWorld>());
        messageHandlers.emplace_back(std::make_unique<RegionRegion>());
        messageHandlers.emplace_back(std::make_unique<Render>());
        messageHandlers.emplace_back(std::make_unique<Simulation>());
        messageHandlers.emplace_back(std::make_unique<WorldState>());

        auto base = Utils::GetBaseAddress();

        if (true)
        {
            // Search for "Illegally formatted message received. You probably failed to bind a callback for this message. Message was: %s (id: 0x%x)\n"'
            //
            // mov rdx, qword ptr ds : [rdi + 8] | RDX = packet ?
            // mov r8d, dword ptr ds : [rdi + 10] | R8 = packet length maybe ?
            // call qword ptr ds : [rax + 8] | call our packet handler
            //

            /*
                mov RDX, <address>
                jmp RDX
                nop
                nop
                nop
                nop
            */
            uint8_t hijack_ProcessPacketRecv[] = {
                0x48, 0xBA, 0xC1, 0x25, 0x85, 0xAF, 0xF6, 0x7F, 0x00, 0x00, 0xFF, 0xE2, // mov rdx, N
                0x90, // nop
                0x90, // nop
                0x90, // nop
                0x90  // nop
            };

            *((uint64_t *)&hijack_ProcessPacketRecv[2]) = (uint64_t)intercept_ProcessPacketRecv;
            RewriteCode(base + 0x14DADAA, hijack_ProcessPacketRecv, sizeof(hijack_ProcessPacketRecv));

            // Return point will not be directly after our injected code, but instead follow the existing jmp that we overwrote
           // ReturnPoint_ProcessPacketRecv = (uint64_t)(base + 0x14DB161);
            ReturnPoint_ProcessPacketRecv = (uint64_t)(base + 0x14DB161);
        }

        if (true)
        {
            // Search for "OutgoingPacket"
            // next call, call qword ptr[rax+18] or whatever

            uint8_t hijack_ProcessPacketSend[] = {
                0x48, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RDX, [address]
                0xFF, 0xE2                                                   // JMP RDX           
            };

            *((uint64_t *)&hijack_ProcessPacketSend[2]) = (uint64_t)intercept_ProcessPacketSend;
            RewriteCode(base + 0x1353696, hijack_ProcessPacketSend, sizeof(hijack_ProcessPacketSend));

            // Return point will not be directly after our injected code, but instead follow the existing jmp that we overwrote
            ReturnPoint_ProcessPacketSend = (uint64_t)(base + 0x1353696 + sizeof(hijack_ProcessPacketSend));
        }


        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        // DetourAttach(&(PVOID &)original_recvfrom, Hooked_Recvfrom); // This is too much to handle. A million different edge cases in packets and ordering. Just going to let the program take care of all the work for us and hook the end calls...
       //  DetourAttach(&(PVOID &)original_recv, Hooked_Recv); // need to hard hook into ssl functions or post-decrypted recv handler...
       // DetourAttach(&(PVOID &)original_sendto, Hooked_Sendto);
        // DetourAttach(&(PVOID &)original_send, Hooked_Send);

        DetourTransactionCommit();

        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}


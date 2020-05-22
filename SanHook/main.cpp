#include <winsock2.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <mutex>
#include <string>

#pragma comment(lib, "ws2_32.lib")

static int (WINAPI* original_recvfrom)(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen) = recvfrom;
static int (WINAPI* original_sendto)(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen) = sendto;
int (WINAPI* original_recv)(SOCKET s, char* buf, int len, int flags) = recv;
int (WINAPI* original_send)(SOCKET s, const char* buf, int len, int flags) = send;

uint16_t last_chat_server_sequence = 0;

void DumpPacket(const char* buff, int len, bool is_sending);
std::string GetAddressFromAddr(const sockaddr* addr);

#include "bitReader.hpp"


#include <thread>
#include <intrin.h>
uint16_t Sansar_ShortCRC(uint8_t* packet, int64_t packetLength, uint64_t initialChecksum)
{
    auto packetIter = packet;
    auto checksum = ~((uint32_t)initialChecksum);

    auto num_8byte_chunks = packetLength / 8;
    for (uint64_t i = 0; i < num_8byte_chunks; ++i)
    {
        checksum = _mm_crc32_u64(checksum, *(uint64_t*)packetIter);
        packetIter += 8;
    }

    auto remainingBytes = packetLength & 7;
    if (remainingBytes >= 4)
    {
        checksum = _mm_crc32_u32(checksum, *(uint32_t*)packetIter);
        packetIter += 4;
        remainingBytes -= 4;
    }

    if (remainingBytes >= 2)
    {
        checksum = _mm_crc32_u16(checksum, *(uint16_t*)packetIter);
        packetIter += 2;
        remainingBytes -= 2;
    }

    if (remainingBytes > 0)
    {
        checksum = _mm_crc32_u8(checksum, *(uint8_t*)packetIter);
    }

    checksum = ~checksum;
    return (uint16_t)checksum;
}

uint32_t checksumSeed_send = 0;
uint32_t checksumSeed_recv = 0;

bool found_first_cvc100 = false;
uint32_t checksumSeed_first_cvc100 = 0;

const struct sockaddr* last_chat_sendto = nullptr;
uint8_t last_chat_sendto_raw[128] = {};

int last_chat_sendto_length = 0;
int last_chat_sendto_flags = 0;
int last_chat_sequence = 0;

int last_sent_sequence = 0;
SOCKET last_chat_sendto_socket = 0;

void SendPacket(const char* buff, int len)
{

}

std::mutex send_mutex;

int Chat(const std::string& newMessage, int sequence, SOCKET s, int flags, const struct sockaddr* to, int tolen) {
    uint8_t payloadLength = 0;

    auto buffer_length = 18 + newMessage.length();
    auto buff_ptr = std::make_unique<char[]>(buffer_length);
    auto buffer = buff_ptr.get();

    buffer[0] = 0x07;

    *((uint16_t*)&buffer[1]) = sequence;
    buffer[3] = buffer_length - 6;
    *((uint32_t*)&buffer[4]) = 0xDDDEC199;
    *((uint32_t*)&buffer[8]) = 0;
    *((uint32_t*)&buffer[12]) = newMessage.length();

    memcpy(&buffer[16], newMessage.c_str(), newMessage.length());

    auto checksum = Sansar_ShortCRC((uint8_t*)buffer, buffer_length - 2, checksumSeed_send);

    *((uint16_t*)&buffer[16 + newMessage.length()]) = checksum;

    auto result = original_sendto(s, buffer, buffer_length, flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1) {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

int SpecialChat(const std::string& newMessage, uint16_t sequence, SOCKET s, int flags, const struct sockaddr* to, int tolen, uint64_t seed) {
    auto text_length = newMessage.size();

    auto buffer_length = 90 + text_length;
    auto buff_ptr = std::make_unique<char[]>(buffer_length);
    auto buffer = buff_ptr.get();

    memset(buffer, 0, buffer_length);

    buffer[0] = 0x07;
    *((uint16_t*)&buffer[1]) = sequence;
    buffer[3] = buffer_length - 13;
    *((uint32_t*)&buffer[4]) = 0x304D3746;
    *((uint64_t*)&buffer[8]) = 0x1c3aad2b02584c90;
    *((uint64_t*)&buffer[16]) = 0xa6040da35a9743f9;

    //*((uint64_t*)&buffer[24]) = 0;
    //*((uint64_t*)&buffer[32]) = 0;
    //*((uint64_t*)&buffer[40]) = 0;

    *((uint32_t*)&buffer[48]) = text_length;
    memcpy(&buffer[52], newMessage.c_str(), text_length);

    // ID of telemetry data?
    *((uint16_t*)&buffer[buffer_length - 13]) = 0xFF;
    *((uint16_t*)&buffer[buffer_length - 12]) = 8;
    *((uint32_t*)&buffer[buffer_length - 10]) = 0x4AC30FE7;

    //*((uint16_t*)&buffer[buffer_length-7]) = 0xc;


    /*
    *((uint64_t*)&buffer[52 + text_length]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 8]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 16]) = 0;
    *((uint8_t*)&buffer[52 + text_length + 24]) = 0;
    */

    auto checksum = Sansar_ShortCRC((uint8_t*)buffer, buffer_length - 2, seed);
    *((uint16_t*)&buffer[buffer_length - 2]) = checksum;

    printf("------------\n");
    DumpPacket(buffer, buffer_length, true);
    printf("------------\n");

    printf("INTERCEPTION SEND ON: (seq %X) Socket: %08X | sendto: %08X | tolen: %d\n", last_chat_server_sequence, last_chat_sendto_socket, last_chat_sendto, last_chat_sendto_length);
    printf("INTERCEPTION: GetAddressFromAddr -> %s\n", GetAddressFromAddr((const sockaddr *)&last_chat_sendto_raw).c_str());
   // auto result = original_sendto(last_chat_sendto_socket, buffer, buffer_length, last_chat_sendto_flags, last_chat_sendto, last_chat_sendto_length);


    auto result = original_sendto(last_chat_sendto_socket, buffer, buffer_length, last_chat_sendto_flags, (const sockaddr*)&last_chat_sendto_raw, last_chat_sendto_length);


    printf("Result = %d\n", result);
    if (result == -1) {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

int SendAcceptFriend(uint8_t* newUuid, int sequence, SOCKET s, int flags, const struct sockaddr* to, int tolen) {

    printf("SendAcceptFriend...");

    //unsigned char rawData1[52] = {
    //    // id | seq(2) |totalLen(1) |segLen(2)| unknownid(4) | text-len(4)| totallen?(1) | segLen(2) | unknown(4) | text-len(4) | segLen(1) | unknown(4) | targetID(16) | zero(4) | checksum(2)
    //    0x07, 0xAF, 0x01, 0x2E, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00,
    //    0x00, 0x00, 0xFF, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00, 0x00,
    //    0x00, 0x18, 0xF7, 0x39, 0x09, 0x65, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
    //    0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00,
    //    0x00, 0x00, 0xCC, 0xCC
    //};

    //unsigned char rawData[] = {
    //    // id7 | seq(2) | totalLen(1) | packetId(4) |  targetID(16) | zero(4) | checksum(2)
    //    0x07, 0xAF, 0x01, 0x18, 0xF7, 0x39, 0x09, 0x65, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
    //    0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00,
    //    0x00, 0x00, 0xCC, 0xCC
    //};

    //unsigned char rawDataToNopNop[] = {
    //    // id7 | seq(2) | totalLen(1) | packetId(4) |  targetID(16) | zero(4) | checksum(2)
    //    0x07, 0xAF, 0x01, 0x18, 0xF7, 0x39, 0x09, 0x65, 0x1E, 0x4E, 0x10, 0x8D, 0x41, 0x6D,
    //    0xCE, 0x09, 0x29, 0x1B, 0x53, 0xFA, 0xAA, 0x75, 0x26, 0x81, 0x00, 0x00,
    //    0x00, 0x00, 0x00, 0x00
    //};


    /* R:\dec\new_sansar_dec\packet_accept_friend.bin (2/23/2020 8:57:04 PM)
   StartOffset(h): 00000000, EndOffset(h): 00000033, Length(h): 00000034 */

    /*
    unsigned char rawDataToNopNop[52] = {
        0x07, 0xAF, 0x01, 0x2E, 0x7B, 0x02, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x53, 0x02, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00, 0x00,
        0x00, 0x18, 0xF7, 0x39, 0x09, 0x65, 0x1E, 0x4E, 0x10, 0x8D, 0x41, 0x6D,
        0xCE, 0x09, 0x29, 0x1B, 0x53, 0xFA, 0xAA, 0x75, 0x26, 0x81, 0x00, 0x00,
        0x00, 0x00, 0xDB, 0x9B
    };
    */






    /*
    unsigned char rawData[] = {
        0x07, 0x27, 0x00, 0xFF, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00, 0x00,
        0x00, 0x18, 0xF7, 0x39, 0x09, 0x65, 0x90, 0x4C, 0x58, 0x02, 0x2B, 0xAD,
        0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A, 0xA3, 0x0D, 0x04, 0xA6, 0x00, 0x00,
        0x00, 0x00, 0x97, 0xBD
    };
    */
    
    /*
    unsigned char rawData[] = {
        0x07, 0x27, 0x00, 0xFF, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x08, 0x00, 0xE7, 0x0F, 0xC3, 0x4A, 0x00, 0x00, 0x00,
        0x00, 0x18, 0xF7, 0x39, 0x09, 0x65, 0x90, 0x4C, 0x58, 0x02, 0x2B, 0xAD,
        0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A, 0xA3, 0x0D, 0x04, 0xA6, 0x00, 0x00,
        0x00, 0x00, 0x97, 0xBD
    };*/

    unsigned char rawData[] = {
        0x07, 0x27, 0x00, 0x18, 0xF7, 0x39, 0x09, 0x65, 0x90, 0x4C, 0x58, 0x02,
        0x2B, 0xAD, 0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A, 0xA3, 0x0D, 0x04, 0xA6,
        0x00, 0x00, 0x00, 0x00, 0x97, 0xBD
    };



    *((uint16_t*)&rawData[1]) = sequence;

    //memcpy(&rawData[8], newUuid, 16);


    auto checksum = Sansar_ShortCRC((uint8_t*)rawData, sizeof(rawData) - 2, checksumSeed_first_cvc100);
    *((uint16_t*)&rawData[sizeof(rawData) - 2]) = checksum;

    printf("*****************\n");
    DumpPacket((const char*)&rawData[0], sizeof(rawData), true);
    auto result = original_sendto(s, (const char*)&rawData[0], sizeof(rawData), flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1) {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

void DumpPacket(const char* buff, int len, bool is_sending)
{
    static const char kHexLookup[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    char output_buffer[65536 * 3 + 1] = {};
    std::size_t output_index = 0;

    for (int i = 0; i < len; ++i)
    {
        auto low = ((uint8_t)buff[i] & 0xF0) >> 4;
        auto high = (uint8_t)buff[i] & 0x0F;

        output_buffer[output_index++] = kHexLookup[low];
        output_buffer[output_index++] = kHexLookup[high];
        output_buffer[output_index++] = ' ';
    }

    if (!output_buffer)
    {
        printf("Bad output buffer\n");
    }
    else
    {
        if (is_sending) {
            printf("--> [%d] %s\n", len, output_buffer);
        }
        else {
            printf("<-- [%d] %s\n", len, output_buffer);
        }
    }
}


#include <map>
std::string GetAddressFromAddr(const sockaddr* addr)
{
    if (!addr) {
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
        auto s = (sockaddr_in*)addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, address, sizeof(address));
    }
    else
    {
        auto s = (sockaddr_in6*)addr;
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

    auto result = getpeername(s, (sockaddr*)&addr, &namelen);

    auto name = GetAddressFromAddr((const sockaddr*)&addr);
    addr_lookup[s] = name;
    return name;
}


uint32_t ReadThreeByteInt(const char* buf)
{
    uint32_t w = (char)buf[2] < 0 ? 0xff000000 : 0;
    uint32_t x = (uint8_t)buf[2] << 16;
    uint32_t y = (uint8_t)buf[1] << 8;
    uint32_t z = (uint8_t)buf[0] << 0;

    uint32_t result = w | x | y | z;

    return result;
}

void WriteThreeByteInt(int32_t value, char* buff) {
    ((uint8_t*)buff)[2] = (uint8_t)((value >> 16) & 0xFF);
    ((uint8_t*)buff)[1] = (uint8_t)((value >> 8) & 0xFF);
    ((uint8_t*)buff)[0] = (uint8_t)((value) & 0xFF);
}

int WINAPI Hooked_Recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
{
    //DumpPacket(buf, len, true);

    auto result = original_recvfrom(s, buf, len, flags, from, fromlen);
    if (result != SOCKET_ERROR)
    {
        //printf("hooked recvfrom [%s] -> %d\n", GetAddressFromAddr(from).c_str(), result);
        DumpPacket(buf, result, false);

        if (buf[0] == 0x0A) {
            if (s == last_chat_sendto_socket) {
                last_chat_server_sequence = *((uint16_t*)&buf[1]);
                printf("** last_chat_server_sequence = %X\n", last_chat_server_sequence);
            }
        }
        if (buf[0] == 0x07)
        {

        }
    }

    return result;
}

class AudioMessages
{
public:
    //static const int = 0x;
    static const int LoadSound = 0x412484C4;
    static const int PlaySound = 0x8FC77316;
    static const int PlayStream= 0x6A2C4CEF;
    static const int StopBroadcastingSound = 0x866BF5CF;
    static const int SetAudioStream = 0x5DCD6123;
    static const int SetMediaSource = 0xEC3CA8EC;
    static const int PerformMediaAction = 0x559B7F04;
    static const int StopSound = 0x1A5C9610;
    static const int SetLoudness= 0x20EDD0C4;
    static const int SetPitch = 0x7BB86A5B;
};

class RenderMessages
{
public:
    //static const int = 0x;
    static const int LightStateChanged = 0x6951DAEC;
};

class AgentControllerMessages 
{
public:
    static const int PlayAnimation = 0x009385A0;
    static const int AgentPlayanimation = 0x00AC2B81;
    static const int ExitSit = 0x0B617A9A;
    static const int ObjectInteractionPromptUpdate = 0x1651CD68;
    static const int ObjectInteractionCreate = 0xBB086E9B;
    static const int RequestSitOnObject = 0xE5321C47;
    static const int SitOnObject = 0x191F08C0;
    static const int RiggedMeshScaleChanged = 0xEA2934E8;
    static const int ScriptCameraMessage = 0x60C955C0;
    static const int SetAgentFiltersBody = 0x09DD53F6;
    static const int RequestSetAgentFiltersBody = 0x2B87F09D;
    static const int SetCharacterUserProperty = 0x31D1EC43;
    static const int CreateSpeechGraphicsPlayer = 0x158B2580;
    static const int RequestSpawnItem = 0x2C21850D;
    static const int RequestDeleteLatestSpawn = 0xEB3C4296;
    static const int RequestDeleteAllSpawns = 0x3EB3EDF7;
    static const int ControlPoint = 0x2DF35CF3;
    static const int WarpCharacter = 0x75C0AC6B;
    static const int RequestWarpCharacter = 0x25C093E0;
    static const int CharacterControlPointInput = 0xFCA3EF20;
    static const int CharacterControlPointInputReliable = 0x8FB6F456;
    static const int CharacterControllerInput = 0x3D490CAB;
    static const int CharacterControllerInputReliable = 0xA7D6EFD1;
    static const int RequestAgentPlayAnimation = 0x982B98D8;
    static const int RequestBehaviorStateUpdate = 0x5489A347;
    static const int AttachToCharacterNode = 0x85BA6E75;
    static const int DetachFromCharacterNode = 0x80F90328;
    static const int RequestDetachFromCharacterNode = 0x67B63AA3;
    static const int SetCharacterNodePhysics = 0x645C4976;
    static const int WarpCharacterNode = 0x83F1D7DB;
    static const int CharacterIKBone = 0xBB382C6B;
    static const int CharacterIKPose = 0xE945D8B8;
    static const int CharacterIKBoneDelta = 0x4C3B3B4B;
    static const int CharacterIKPoseDelta = 0x893A18BE;
    static const int ObjectInteraction = 0xA25F81AB;
    static const int ObjectInteractionUpdate = 0x17B7D18A;
};

class GameWorldMessages {
public:
   // static const int = 0x;
    static const int StaticMeshFlagsChanged = 0xAE522F17;
    static const int StaticMeshScaleChanged = 0xCA6CCC08;
    static const int RiggedMeshFlagsChange = 0x3F020C77;
    static const int Timestamp = 0xD22C9D73;
    static const int MoveEntity = 0xEFC20B7F;
    static const int ChangeMaterialVectorParam = 0x403D5704;
    static const int ChangeMaterialFloatParam = 0x4F20B073;
    static const int ChangeMaterial = 0x45C605B8;
};

class WorldStateMessages {
public:
    static const int DynamicPlayback = 0x1505C6D8;
    static const int MasterFrameSync = 0x5A4AFA33;
    static const int AgentControllerMapping = 0xBB5865E8;
    static const int CreateCharacterNode = 0x32DC63D7;
    static const int CreateAgentController = 0xF555FE2D;
};

class ClientKafkaMessages
{
public:
    static const int FriendResponseLoaded = 0x0AF50C12;
    static const int PresenceUpdateFanoutLoaded = 0x5915FBFE;
    static const int FriendTableLoaded = 0xB4AB87F5;
    static const int RelationshipTableLoaded = 0x0A7562A7;
    static const int PrivateChatLoaded = 0x4B73CF2C;
    static const int PrivateChatStatusLoaded = 0x9BC4EF8A;
    static const int ScriptRegionConsoleLoaded = 0xD3CAA979;
    static const int ClientMetric = 0x4AC30FE7;
    static const int RegionHeartbeatMetric = 0xDCF900A4;
    static const int RegionEventMetric = 0xBA6DB2FC;
    static const int SubscribeScriptRegionConsole = 0x3BFA4474;
    static const int UnsubscriptScriptRegionConsole = 0xD49B04C3;
    static const int ScriptConsoleLog = 0x00B0E15E;
    static const int LongLivedNotification = 0x46C5FDF3;
    static const int LongLivedNotificationDelete = 0x59CF6950;
    static const int LongLivedNotificationLoaded = 0x3494608D;
    static const int shortLivedNotification = 0xAD589C6F;
    static const int Login= 0x0C0C9D81;
    static const int LoginReply = 0xA685E82B;
    static const int EnterRegion = 0x08445006;
    static const int LeaveRegion = 0xE4ADC2EB;
    static const int RegionChat = 0x304D3746;
    static const int PrivateChat = 0x2DC9B029;
    static const int PrivateChatStatus = 0x955C35EB;
    static const int PresenceUpdate = 0x1DB989E8;
    static const int FriendRequest = 0xA356B3ED;
    static const int FriendRequestStatus = 0x14FFCD37;
    static const int FriendResponse = 0xE24EBDD3;
    static const int FriendresponseStatus = 0x22565685;
    static const int FriendTable = 0x203CC0A8;
    static const int RelationshipOperational= 0x650939F7;
    static const int RelationshipTable = 0x078DCC26;
    static const int InventoryItemCapabilities = 0xA2190F5D;
    static const int InventoryItemRevision = 0xE3466906;
    static const int InventoryItemUpdate = 0xD7C7DC26;
    static const int InventoryItemDelete = 0xB11C8C84;
    static const int InventoryLoaded = 0x75BAFB95;
    static const int FriendRequestLoaded = 0xF5361468;
};


class ClientRegionMessages {
public:
    static const int UserLogin = 0x3902800A;
    static const int UserLoginReply = 0x30CDBED6;
    static const int AddUser = 0xF6B9093E;
    static const int RemoveUser = 0x0A7FC621;
    static const int RenameUser = 0xC67C58F7;
    static const int ChatMessageToClient = 0x083642BD;
    static const int ChatMessageToServer = 0xDDDEC199;
    static const int SetAgentController = 0xD6F4CF23;
    static const int DebugTimeChangeToServer = 0x41FE0612;
    static const int VisualDebuggerCaptureToServer = 0x0741CA9B;
    static const int ClientStaticReady = 0xF8E77C8E;
    static const int ClientDynamicReady = 0x575AC239;
    static const int ClientRegionCommandMessage = 0xECE56EFD;
    static const int RequestDropPortal = 0x7D22C30C;
    static const int VibrationPulseToClient = 0x0D3809EB;
    static const int TeleportTo = 0x5C7CC1FC;
    static const int TeleportToUri = 0x2BDBDB56;
    static const int TeleportToEditMode = 0x706F63FB;
    static const int DebugTimeChangeToClient = 0x5178DC5E;
    static const int VisualDebuggerCaptureToClient = 0xF66AD9BF;
    static const int ScriptModalDialog = 0x88023C72;
    static const int ScriptModalDialogResponse = 0xB34F3A45;
    static const int TwitchEventSubscription = 0x981AB0D6;
    static const int TwitchEvent = 0x28F54053;
    static const int InitialChunkSubscribed = 0xB4E1AB7B;
    static const int ClientKickNotification = 0x4B68A51C;
    static const int ClientSmiteNotification = 0x58003034;
    static const int ClientNotification= 0x6188A537;
    static const int ClientVoiceBroadcastStartNotification = 0x7E28AEAF;
    static const int ClientVoiceBroadcastStopNotification = 0xC33DE58B;
    static const int CreateRegionBroadcasted = 0x87341F77;
    static const int SubscribeCommand = 0xABDA80C7;
    static const int UnsubscribeCommand = 0xA36E9F9C;
    static const int ClientCommand = 0xB87F9C66;
    static const int OpenStoreListing = 0x05C1A8D7D;
    static const int OpenUserStore = 0x53078A1E;
    static const int OpenQuestCharacterDialog = 0x4221836F;
    static const int UIScriptableBarStart = 0x036164050;
    static const int UIScriptableBarStopped = 0xBAFD799D;
    static const int UIScriptableBarCancel = 0x604E18DE;
    static const int UIHintTextUpdate = 0x64225637;
    static const int QuestOfferResponse = 0x4DB48E35;
    static const int QuestCompleted = 0xE1EE5F5D;
    static const int QuestRemoved = 0x34793AB0;
    static const int ShowWorldDetail = 0x5F483F0C;
    static const int ShowTutorialHint = 0x581827CC;
    static const int TutorialHintsSetEnabled = 0xE4C496DF;
};

int WINAPI Hooked_Sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
{

    // search for network ptr thingy, this + 0x7c or somewhat = sequence
    //unsigned char rawData[40] = {
    //    0x10, 0x78, 0xCA, 0x39, 0x2E, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    //    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
    //    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //    0x00, 0x00, 0x00, 0x00
    //};

    if (s == last_chat_sendto_socket) {
        last_chat_server_sequence = *((uint16_t*)&buf[1]);
        printf("** last_chat_server_sequence = %X\n", last_chat_server_sequence);
    }

    //  DumpPacket(buf, len, true);


    // TODO: Listen for 'ACK' messages from server. get our 'sequence' from there?

    if (buf == nullptr || len < 8 || (buf[0] != 0x01 && buf[0] != 0x07)) {
        return original_sendto(s, buf, len, flags, to, tolen);
    }

   // std::unique_lock<std::mutex> m(send_mutex);

    last_sent_sequence = *((uint16_t*)&buf[1]);
    auto messageId = *((uint32_t*)&buf[4]);

    //printf("Hooked sendto [%s] -> %d\n", GetAddressFromAddr(to).c_str(), result);
    if (buf[0] == 0x01)
    {
        auto seed = *(uint32_t*)&buf[5];
        auto seedNameLength = *(uint32_t*)&buf[13];
        auto seedName = std::string((const char*)&buf[17], (std::size_t)seedNameLength);

        if (seedName == "CRC_102") {
            printf("SEND checksum [%s] Seed = %08X | TO=%08X | s=%08X\n", seedName.c_str(), seed, to, s);
            checksumSeed_send = seed;
        }
        else if (seedName == "CVC_100") {
            printf("RECV checksum [%s] Seed = %08X | TO=%08X | s=%08X\n", seedName.c_str(), seed, to, s);

            if (!found_first_cvc100) {
                found_first_cvc100 = true;
                checksumSeed_first_cvc100 = seed;
            }
            else {
                checksumSeed_recv = seed;
            }
        }
        else {
            printf("UNKNOWN checksum [%s] Seed = %08X\n", seedName.c_str(), seed);
        }
    }
    else if (buf[0] == 0x0a)
    {
        //  printf("C -> S: Pong\n");
    }
    else if (buf[0] == 0x08)
    {
        // Useless kafka login... or is it?
        if (0 && messageId == 0x9D813000)
        {
            auto unknownA = *((uint16_t*)&buf[8]);
            auto accountIdBytes = ((uint8_t*)&buf[10]);
            auto personaIdBytes = ((uint8_t*)&buf[26]);
            auto secret = *((uint32_t*)&buf[42]);
            auto Unknown = *((uint64_t*)&buf[46]);

            //auto checksum = *((uint16_t*)&buf[len - 2]);

            printf("Secret = %u\n", secret);
            printf("AccountId = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
                accountIdBytes[7],
                accountIdBytes[6],
                accountIdBytes[5],
                accountIdBytes[4],
                accountIdBytes[3],
                accountIdBytes[2],
                accountIdBytes[1],
                accountIdBytes[0],
                accountIdBytes[15],
                accountIdBytes[14],
                accountIdBytes[13],
                accountIdBytes[12],
                accountIdBytes[11],
                accountIdBytes[10],
                accountIdBytes[9],
                accountIdBytes[8]
            );
            printf("PersonaId = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
                personaIdBytes[7],
                personaIdBytes[6],
                personaIdBytes[5],
                personaIdBytes[4],
                personaIdBytes[3],
                personaIdBytes[2],
                personaIdBytes[1],
                personaIdBytes[0],
                personaIdBytes[15],
                personaIdBytes[14],
                personaIdBytes[13],
                personaIdBytes[12],
                personaIdBytes[11],
                personaIdBytes[10],
                personaIdBytes[9],
                personaIdBytes[8]
            );


            unsigned char otherAccount[16] = {
                0x02, 0x46, 0x98, 0x5E, 0xB8, 0xE5, 0x48, 0xC7, 0xD7, 0xAC, 0x86, 0x2B,
                0xA4, 0x83, 0xBE, 0x8D
            };
            memcpy(accountIdBytes, otherAccount, sizeof(otherAccount) / sizeof(otherAccount[0]));

            static unsigned char otherPersona[16] = {
               0x96, 0x45, 0xD3, 0x64, 0xB7, 0x9C, 0x1F, 0xD6, 0x54, 0x83, 0x78, 0xF7,
               0x8F, 0x42, 0x69, 0xA3
            };
            memcpy(personaIdBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));


            auto myChecksum = Sansar_ShortCRC((uint8_t*)buf, len - 2, checksumSeed_recv); // yes this is the correct checksum to use here despite its name...
            *((uint16_t*)&buf[len - 2]) = myChecksum;
        }
    }
    else if (buf[0] == 0x07)
    {
        // rez item
        if (messageId == AgentControllerMessages::RequestSpawnItem)
        {
          //  ((char*)buf)[37] = 0x9a;
           // ((char*)buf)[38] = 0x1d;
           // ((char*)buf)[39] = 0x00;


           // WriteThreeByteInt(5000, (char*)&buf[37]);
           // WriteThreeByteInt(5000, (char*)&buf[40]);
            //WriteThreeByteInt(0, (char*)&buf[50]);

            //  Id   Seq     MsgId            Frame                     AgentId       ResourceId                            AttachmentNode   SpawnPosition(78bit)|height?(32bit)|SpawnOrientation(43bit)         Checksum
            // [07] [91 01] [31 0D 85 21] 2C [C6 6D 00 00 00 00 00 00] [05 00 00 00] [37 7F 61 06 51 D3 9B EF 5F B0 FB 67 89 AD EE 79] [FF] [EB AE 00 F2 3A 02 48 2A A0 C0 FE BF FF F7 FE 51] [77 96]

            // [frame?][agentControllerId?][ResourceId][AttachmentNode?][SpawnPosition(0x4E)][SpawnOrientation(0x2B)]

            /*
                result = v9 & 0x80000007;
                if ( (signed int)result < 0 )
                {
                    result = (((_BYTE)result - 1) | 0xFFFFFFF8) + 1;
                }
            */
           /*
            uint8_t* buff_out = (uint8_t*)buf;
            buff_out[43] = 0x48;
            buff_out[44] = 0x2a;
            buff_out[45] = 0xa0;
            buff_out[46] = 0xc0;
            */

            // 39 bit x,y pos??
            //int64_t positionX = 0;
           // int64_t positionY = 0;

            auto frame = *((uint64_t*)&buf[8]);
            auto agentControllerId = *((uint32_t*)&buf[16]);
            auto clusterId = ((uint8_t*)&buf[20]);
            auto attachmentNode = *((uint8_t*)&buf[36]); // 9

            BitReader br((uint8_t*)&buf[37], 10);
            auto positionX = br.ReadFloat(26, false);
            auto positionY = br.ReadFloat(26, false);
            auto positionZ = br.ReadFloat(26, true);

            br.Reset();
            br.WriteFloat(positionX, 26, false);
            br.WriteFloat(positionY, 26, false);
            br.WriteFloat(0.01f, 26, true);

            br.Reset();
            positionX = br.ReadFloat(26, false);
            positionY = br.ReadFloat(26, false);
            positionZ = br.ReadFloat(26, true);

            //static int agent = 0;
            //*((uint32_t*)&buf[16]) = agent;
            //*((uint8_t*)&buf[36]) = 3;

            /*
            if (positionY < 0) {
                *((uint32_t*)&height) ^= 1 << 30;
            }*/

            printf("ClusterId = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
                clusterId[7],
                clusterId[6],
                clusterId[5],
                clusterId[4],
                clusterId[3],
                clusterId[2],
                clusterId[1],
                clusterId[0],
                clusterId[15],
                clusterId[14],
                clusterId[13],
                clusterId[12],
                clusterId[11],
                clusterId[10],
                clusterId[9],
                clusterId[8]
            );
            printf("agentControllerId=%d attachmentNode=%d | Pos=<%f, %f, %f>\n", agentControllerId, attachmentNode, positionX, positionY, positionZ);
            DumpPacket(buf, len, true);
            //////////////////////////////
            uint8_t newClusterId[16] = {};
            bool enabled = false;

            FILE* inFile = nullptr;

            // time? will they expire if this is -1? ^^;
          //  *((uint64_t*)&buf[8]) = 0;
           // *((uint32_t*)&buf[16]) = -1;

            fopen_s(&inFile, "u:\\sanhook_config_rez.txt", "rb");
            if (inFile != nullptr) {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(newClusterId, sizeof(newClusterId), sizeof(newClusterId[0]), 16, inFile);
                fclose(inFile);

                if (bytesRead == 16 && enabled) {
                    memcpy(clusterId, newClusterId, 16);
                }
            }

            auto myChecksum = Sansar_ShortCRC((uint8_t*)buf, len - 2, checksumSeed_send); 
            *((uint16_t*)&buf[len - 2]) = myChecksum;
            ///////////////////////
        }
        // instant message
        if (0 && messageId == ClientKafkaMessages::PrivateChat)
        {
            auto unknownA = *((uint64_t*)&buf[8]);
            auto personaIdFromBytes = ((uint8_t*)&buf[16]);
            auto personaIdToBytes = ((uint8_t*)&buf[32]);

            auto textLength = *((uint32_t*)&buf[48]);
            auto text = std::string((const char*)&buf[52], (std::size_t)textLength);

            auto unknownB = *((uint64_t*)&buf[52 + textLength]);

            printf("From = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
                personaIdFromBytes[7],
                personaIdFromBytes[6],
                personaIdFromBytes[5],
                personaIdFromBytes[4],
                personaIdFromBytes[3],
                personaIdFromBytes[2],
                personaIdFromBytes[1],
                personaIdFromBytes[0],
                personaIdFromBytes[15],
                personaIdFromBytes[14],
                personaIdFromBytes[13],
                personaIdFromBytes[12],
                personaIdFromBytes[11],
                personaIdFromBytes[10],
                personaIdFromBytes[9],
                personaIdFromBytes[8]
            );
            printf("To = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
                personaIdToBytes[7],
                personaIdToBytes[6],
                personaIdToBytes[5],
                personaIdToBytes[4],
                personaIdToBytes[3],
                personaIdToBytes[2],
                personaIdToBytes[1],
                personaIdToBytes[0],
                personaIdToBytes[15],
                personaIdToBytes[14],
                personaIdToBytes[13],
                personaIdToBytes[12],
                personaIdToBytes[11],
                personaIdToBytes[10],
                personaIdToBytes[9],
                personaIdToBytes[8]
            );


            //////////////////////////////
            uint8_t personaId[16] = {};
            bool enabled = false;

            FILE* inFile = nullptr;

            fopen_s(&inFile, "u:\\sanhook_config_im.txt", "rb");
            if (inFile != nullptr) {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                if (bytesRead == 16) {
                    memcpy(personaIdFromBytes, personaId, 16);
                }

                bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                if (bytesRead == 16) {
                    memcpy(personaIdToBytes, personaId, 16);
                }
                fclose(inFile);
            }
            ///////////////////////

            if (enabled) {
                memcpy(personaIdFromBytes, &buf[16], 16);
                memcpy(personaIdToBytes, &buf[32], 16);
            }

            static bool aa = false;
            if (aa) {
                static unsigned char otherAccount[16] = {
                    0x02, 0x46, 0x98, 0x5E, 0xB8, 0xE5, 0x48, 0xC7, 0xD7, 0xAC, 0x86, 0x2B,
                    0xA4, 0x83, 0xBE, 0x8D
                };
                memcpy(personaIdFromBytes, otherAccount, sizeof(otherAccount) / sizeof(otherAccount[0]));
            }

            static bool bb = false;
            if (bb) {
                static unsigned char otherPersona[16] = {
                    0x90, 0x4C, 0x58, 0x02, 0x2B, 0xAD, 0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A,
                    0xA3, 0x0D, 0x04, 0xA6
                };

                memcpy(personaIdToBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));
            }

            //auto myChecksum = Sansar_ShortCRC((uint8_t*)buf, len - 2, checksumSeed_recv); // yes this is the correct checksum to use here despite its name...
            //*((uint16_t*)&buf[len - 2]) = myChecksum;
        }
        if (0 && messageId == AgentControllerMessages::AgentPlayanimation)
        {
            auto unknownB = *((uint32_t*)&buf[8]);
            auto Time = *((uint64_t*)&buf[12]);
            auto unknownD = *((uint32_t*)&buf[16]);
            auto unknownE = *((uint32_t*)&buf[24]);

            auto ResourceIdBytes = ((uint8_t*)&buf[28]);

            auto unknownF = *((uint16_t*)&buf[44]);
            auto unknownG = *((uint8_t*)&buf[46]);

            printf("PlayEmote:\n    ResourceID = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n    UnknownB = %d\n    Time = %llu\n    UnknownD = %d\n    UnknownE = %d\n    UnknownF = %d\n    UnknownG = %d\n",
                ResourceIdBytes[7],
                ResourceIdBytes[6],
                ResourceIdBytes[5],
                ResourceIdBytes[4],
                ResourceIdBytes[3],
                ResourceIdBytes[2],
                ResourceIdBytes[1],
                ResourceIdBytes[0],
                ResourceIdBytes[15],
                ResourceIdBytes[14],
                ResourceIdBytes[13],
                ResourceIdBytes[12],
                ResourceIdBytes[11],
                ResourceIdBytes[10],
                ResourceIdBytes[9],
                ResourceIdBytes[8],
                unknownB,
                Time,
                unknownD,
                unknownE,
                unknownF,
                unknownG
            );
        }
        // 07 9A 01 2D FD 6E E5 EC 25 00 00 00 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 C6 CE
        else if (0 && messageId == ClientRegionMessages::ClientRegionCommandMessage) {
            auto textLength = *((uint32_t*)&buf[8]);
            auto text = std::string((const char*)&buf[12], (std::size_t)textLength);

            if (text == "test123") {
               // auto result = original_sendto(s, buf, len, flags, to, tolen);
                auto result = SpecialChat("Intercepted: " + text, last_chat_server_sequence+1, s, flags, to, tolen, checksumSeed_first_cvc100);
                ++last_chat_server_sequence;
                return result;
            }
        }
        else if (messageId == ClientKafkaMessages::RegionChat)
        {
            memcpy(&last_chat_sendto_raw, to, tolen);

            last_chat_sendto = to;
            last_chat_sendto_length = tolen;
            last_chat_sendto_flags = flags;
            last_chat_sendto_socket = s;
            last_chat_sequence = last_sent_sequence;
            

            auto payload = (uint8_t*)&buf[8];
            auto personaIdBytes = (uint8_t*)&payload[0];
            auto unknownB = *((uint64_t*)&payload[16]);
            auto unknownC = *((uint64_t*)&payload[24]);
            auto unknownD = *((uint64_t*)&payload[32]);
            auto textLength = *((uint32_t*)&payload[40]);
            auto text = std::string((const char*)&payload[44], (std::size_t)textLength);
            auto unknownF = *((uint64_t*)&payload[44 + textLength]);
            auto unknownG = *((uint64_t*)&payload[44 + textLength + 8]);
            auto unknownH = *((uint64_t*)&payload[44 + textLength + 16]);
            auto unknownI = *((uint64_t*)&payload[44 + textLength + 24]);
            auto textLen2 = *((uint32_t*)&payload[44 + textLength + 32]);

            //printf("textLen2 -> %d", textLen2);
            //*((uint32_t*)&payload[44 + textLength + 18 + 4]) = 0;
            //len = 44 + textLength + 18 + 4 + 4;

            printf("SpecialChat:\n    PersonaID = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n    unknownB = %llu\n    UnknownC = %llu\n    unknownD = %llu\n    unknownF = %llu\n    unknownG = %llu\n    unknownH = %llu\n    unknownI = %llu\n    CHAT = %s | ",
                personaIdBytes[7],
                personaIdBytes[6],
                personaIdBytes[5],
                personaIdBytes[4],
                personaIdBytes[3],
                personaIdBytes[2],
                personaIdBytes[1],
                personaIdBytes[0],
                personaIdBytes[15],
                personaIdBytes[14],
                personaIdBytes[13],
                personaIdBytes[12],
                personaIdBytes[11],
                personaIdBytes[10],
                personaIdBytes[9],
                personaIdBytes[8],
                unknownB,
                unknownC,
                unknownD,
                unknownF,
                unknownG,
                unknownH,
                unknownI,
                text.c_str()
            );

            static unsigned char otherPersona[16] = {
                0x96, 0x45, 0xD3, 0x64, 0xB7, 0x9C, 0x1F, 0xD6, 0x54, 0x83, 0x78, 0xF7,
                0x8F, 0x42, 0x69, 0xA3
            };

            //////////////////////////////
            uint8_t personaId[16] = {};
            bool enabled = false;

            FILE* inFile = nullptr;

            fopen_s(&inFile, "u:\\sanhook_config.txt", "rb");
            if (inFile != nullptr) {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                fclose(inFile);

                if (bytesRead == 16) {
                    memcpy(otherPersona, personaId, 16);
                }
            }
            ///////////////////////

            //static bool x = false;
            static std::string trigger = "test123";

            /*
            if (text == "test123") {

                static unsigned char otherPersonaaa[16] = {
                    0x90, 0x4C, 0x58, 0x02, 0x2B, 0xAD, 0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A,
                    0xA3, 0x0D, 0x04, 0xA6
                };
               // memcpy(personaIdBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));

                SendAcceptFriend(otherPersona, last_chat_server_sequence, s, flags, to, tolen);
                *((uint16_t*)&buf[1]) = last_chat_server_sequence+1;
               //SpecialChat(text, last_chat_server_sequence, s, flags, to, tolen, checksumSeed_first_cvc100);
               // auto result = original_sendto(s, buf, len, flags, to, tolen);

            }
            */
            if (enabled /*|| text == trigger*/) {

                /*static unsigned char otherPersona[16] = {
                    0x96, 0x45, 0xD3, 0x64, 0xB7, 0x9C, 0x1F, 0xD6, 0x54, 0x83, 0x78, 0xF7,
                    0x8F, 0x42, 0x69, 0xA3
                };*/

                memcpy(personaIdBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));

                // return SpecialChat("XXXX", *((uint16_t*)&buf[1]), s, flags, to, tolen);
            }

            //return;
        }
        else if (0 && messageId == ClientRegionMessages::ChatMessageToServer)
        {
            //last_chat_sendto = to;
            last_chat_sendto_length = tolen;
            last_chat_sendto_flags = flags;
            last_chat_sendto_socket = s;

            auto payload = (uint8_t*)&buf[8];

            auto unknownB = *((uint32_t*)&payload[0]);
            auto textLength = *((uint32_t*)&payload[4]);

            memset(&payload[8], '*', textLength);

            auto text = std::string((const char*)&payload[8], (std::size_t)textLength);

            auto checksum = *((uint16_t*)&buf[len - 2]);
            auto myChecksum = Sansar_ShortCRC((uint8_t*)buf, len - 2, checksumSeed_send);

            printf("Chat: [%d] %s\n", unknownB, text.c_str());
            /*
            printf("Expected checksum: %d | calculated checksum: %d  -> %s", checksum, myChecksum, myChecksum == checksum ? "OK" : "FAIL");

            static bool x = true;//false;
            if (x == true || text == "test") {
                return Chat("Butts", *((uint16_t*)&buf[1]), s, flags, to, tolen);
            }
            */
        }
    }

   //printf("Socket: %08X | sendto: %08X | tolen: %d\n", s, to, tolen);
   //printf("^^: GetAddressFromAddr -> %s\n", GetAddressFromAddr(to).c_str());

  //  DumpPacket(buf, len, true);

    auto result = original_sendto(s, buf, len, flags, to, tolen);
    return result;
}


int WINAPI Hooked_Recv(SOCKET s, char* buf, int len, int flags)
{
    auto result = original_recv(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("hooked recv [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        //DumpPacket(buf, result, false);
    }

    return result;
}


int WINAPI Hooked_Send(SOCKET s, const char* buf, int len, int flags)
{
    auto result = original_send(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("Hooked send [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
          DumpPacket(buf, len, true);
    }

    return result;
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
        FILE* pFile = nullptr;
        AllocConsole();
        freopen_s(&pFile, "CONOUT$", "w", stdout);

        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");

        /*
        auto baseAddress = GetModuleHandle("SansarClient.exe");
        uint8_t freecamHack[] = {
            0xB0, 0x01, 0x90, 0x90, 0x90, 0x90
        };

        printf("Base Address: %X\n", baseAddress);
        printf("hinstDLL: %X\n", baseAddress);

        SIZE_T written = 0;
        auto xxx = WriteProcessMemory(GetModuleHandle("SansarClient.exe"), (void*)0x14170C24F, freecamHack, sizeof(freecamHack), &written);
        printf("Write memory: %d | %d bytes written | %X\n", xxx, written, baseAddress);
        */
        /*
        memcpy((void*)((uint64_t)baseAddress + 0x170FA12), freecamHack, sizeof(freecamHack));
        printf("Hack1: %X\n", (void*)((uint64_t)baseAddress + 0x170FA12));

        uint8_t inventoryAlwaysHack[] = {
           0xB0, 0x01, 0x88, 0x07
        };
        memcpy((void*)((uint64_t)baseAddress + 0x1715A85), inventoryAlwaysHack, sizeof(inventoryAlwaysHack));

        printf("----------------------------------\n");
        printf("----------------------------------\n");
        printf("----------------------------------\n");
        printf("----------------------------------\n");
        printf("----------------------------------\n");
        printf("----------------------------------\n");
        */
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        //DetourAttach(&(PVOID &)original_recvfrom, Hooked_Recvfrom);
      //  DetourAttach(&(PVOID &)original_recv, Hooked_Recv); // need to hard hook into ssl functions or post-decrypted recv handler...
        DetourAttach(&(PVOID&)original_sendto, Hooked_Sendto);
      // DetourAttach(&(PVOID &)original_send, Hooked_Send);

        DetourTransactionCommit();

        //// Freecam always
        //170FA12
        //B0 01 90 90 90 90

        //// Inventory always
        //1715A85
        //B0 01 88 07

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


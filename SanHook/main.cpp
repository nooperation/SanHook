#include <winsock2.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <mutex>
#include <string>

#pragma comment(lib, "ws2_32.lib")

static int (WINAPI *original_recvfrom)(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen) = recvfrom;
static int (WINAPI *original_sendto)(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen) = sendto;
int (WINAPI *original_recv)(SOCKET s, char *buf, int len, int flags) = recv;
int (WINAPI *original_send)(SOCKET s, const char *buf, int len, int flags) = send;

uint16_t last_chat_server_sequence = 0;

void DumpPacket(const char *buff, int len, bool is_sending);
std::string GetAddressFromAddr(const sockaddr *addr);

#include "bitReader.hpp"
#include "Messages.h"
#include "PacketReader.hpp"

#include <thread>
#include <intrin.h>

#include <map>
std::map<uint32_t, std::string> sessionIdToNameMap = std::map<uint32_t, std::string>();

uint16_t Sansar_ShortCRC(uint8_t *packet, int64_t packetLength, uint64_t initialChecksum)
{
    auto packetIter = packet;
    auto checksum = ~((uint32_t)initialChecksum);

    auto num_8byte_chunks = (uint64_t)(packetLength / 8);
    for (uint64_t i = 0; i < num_8byte_chunks; ++i)
    {
        checksum = (uint32_t)_mm_crc32_u64(checksum, *(uint64_t *)packetIter);
        packetIter += 8;
    }

    auto remainingBytes = packetLength & 7;
    if (remainingBytes >= 4)
    {
        checksum = _mm_crc32_u32(checksum, *(uint32_t *)packetIter);
        packetIter += 4;
        remainingBytes -= 4;
    }

    if (remainingBytes >= 2)
    {
        checksum = _mm_crc32_u16(checksum, *(uint16_t *)packetIter);
        packetIter += 2;
        remainingBytes -= 2;
    }

    if (remainingBytes > 0)
    {
        checksum = _mm_crc32_u8(checksum, *(uint8_t *)packetIter);
    }

    checksum = ~checksum;
    return (uint16_t)checksum;
}

uint32_t checksumSeed_send = 0;
uint32_t checksumSeed_recv = 0;

bool found_first_cvc100 = false;
uint32_t checksumSeed_first_cvc100 = 0;

const struct sockaddr *last_chat_sendto = nullptr;
uint8_t last_chat_sendto_raw[128] = {};

int last_chat_sendto_length = 0;
int last_chat_sendto_flags = 0;
int last_chat_sequence = 0;

int last_sent_sequence = 0;
SOCKET last_chat_sendto_socket = 0;

void SendPacket(const char *buff, int len)
{

}

std::mutex send_mutex;

int Chat(const std::string &newMessage, int sequence, SOCKET s, int flags, const struct sockaddr *to, int tolen)
{
    uint8_t payloadLength = 0;

    auto buffer_length = (uint32_t)(18 + newMessage.length());
    auto buff_ptr = std::make_unique<char[]>(buffer_length);
    auto buffer = buff_ptr.get();

    buffer[0] = 0x07;

    *((uint16_t *)&buffer[1]) = sequence;
    buffer[3] = buffer_length - 6;
    *((uint32_t *)&buffer[4]) = 0xDDDEC199;
    *((uint32_t *)&buffer[8]) = 0;
    *((uint32_t *)&buffer[12]) = (uint32_t)newMessage.length();

    memcpy(&buffer[16], newMessage.c_str(), (uint32_t)newMessage.length());

    auto checksum = Sansar_ShortCRC((uint8_t *)buffer, buffer_length - 2, checksumSeed_send);

    *((uint16_t *)&buffer[16 + newMessage.length()]) = checksum;

    auto result = original_sendto(s, buffer, buffer_length, flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1)
    {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

int SpecialChat(const std::string &newMessage, uint16_t sequence, SOCKET s, int flags, const struct sockaddr *to, int tolen, uint64_t seed)
{
    auto text_length = (uint32_t)newMessage.size();

    auto buffer_length = 90 + text_length;
    auto buff_ptr = std::make_unique<char[]>(buffer_length);
    auto buffer = buff_ptr.get();

    memset(buffer, 0, buffer_length);

    buffer[0] = 0x07;
    *((uint16_t *)&buffer[1]) = sequence;
    buffer[3] = buffer_length - 13;
    *((uint32_t *)&buffer[4]) = 0x304D3746;
    *((uint64_t *)&buffer[8]) = 0x1c3aad2b02584c90;
    *((uint64_t *)&buffer[16]) = 0xa6040da35a9743f9;

    //*((uint64_t*)&buffer[24]) = 0;
    //*((uint64_t*)&buffer[32]) = 0;
    //*((uint64_t*)&buffer[40]) = 0;

    *((uint32_t *)&buffer[48]) = text_length;
    memcpy(&buffer[52], newMessage.c_str(), text_length);

    // ID of telemetry data?
    *((uint16_t *)&buffer[buffer_length - 13]) = 0xFF;
    *((uint16_t *)&buffer[buffer_length - 12]) = 8;
    *((uint32_t *)&buffer[buffer_length - 10]) = 0x4AC30FE7;

    //*((uint16_t*)&buffer[buffer_length-7]) = 0xc;


    /*
    *((uint64_t*)&buffer[52 + text_length]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 8]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 16]) = 0;
    *((uint8_t*)&buffer[52 + text_length + 24]) = 0;
    */

    auto checksum = Sansar_ShortCRC((uint8_t *)buffer, buffer_length - 2, seed);
    *((uint16_t *)&buffer[buffer_length - 2]) = checksum;

    printf("------------\n");
    DumpPacket(buffer, buffer_length, true);
    printf("------------\n");

    printf("INTERCEPTION SEND ON: (seq %X) Socket: %08llX | sendto: %08llX | tolen: %d\n", last_chat_server_sequence, last_chat_sendto_socket, (uint64_t)last_chat_sendto, last_chat_sendto_length);
    printf("INTERCEPTION: GetAddressFromAddr -> %s\n", GetAddressFromAddr((const sockaddr *)&last_chat_sendto_raw).c_str());
    // auto result = original_sendto(last_chat_sendto_socket, buffer, buffer_length, last_chat_sendto_flags, last_chat_sendto, last_chat_sendto_length);


    auto result = original_sendto(last_chat_sendto_socket, buffer, buffer_length, last_chat_sendto_flags, (const sockaddr *)&last_chat_sendto_raw, last_chat_sendto_length);


    printf("Result = %d\n", result);
    if (result == -1)
    {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

int SendAcceptFriend(uint8_t *newUuid, int sequence, SOCKET s, int flags, const struct sockaddr *to, int tolen)
{

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



    *((uint16_t *)&rawData[1]) = sequence;

    //memcpy(&rawData[8], newUuid, 16);


    auto checksum = Sansar_ShortCRC((uint8_t *)rawData, sizeof(rawData) - 2, checksumSeed_first_cvc100);
    *((uint16_t *)&rawData[sizeof(rawData) - 2]) = checksum;

    printf("*****************\n");
    DumpPacket((const char *)&rawData[0], sizeof(rawData), true);
    auto result = original_sendto(s, (const char *)&rawData[0], sizeof(rawData), flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1)
    {
        printf("Last error = %d\n", WSAGetLastError());
    }

    return result;
}

void DumpPacket(const char *buff, int len, bool is_sending)
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
        if (is_sending)
        {
            printf("--> [%d] %s\n", len, output_buffer);
        }
        else
        {
            printf("<-- [%d] %s\n", len, output_buffer);
        }
    }
}

std::string ClusterButt(std::string uuidBytes)
{
    char buffer[32];
    uint32_t i = 0;

    buffer[i++] = uuidBytes[14];
    buffer[i++] = uuidBytes[15];
    buffer[i++] = uuidBytes[12];
    buffer[i++] = uuidBytes[13];
    buffer[i++] = uuidBytes[10];
    buffer[i++] = uuidBytes[11];
    buffer[i++] = uuidBytes[8];
    buffer[i++] = uuidBytes[9];
    buffer[i++] = uuidBytes[6];
    buffer[i++] = uuidBytes[7];
    buffer[i++] = uuidBytes[4];
    buffer[i++] = uuidBytes[5];
    buffer[i++] = uuidBytes[2];
    buffer[i++] = uuidBytes[3];
    buffer[i++] = uuidBytes[0];
    buffer[i++] = uuidBytes[1];
    buffer[i++] = uuidBytes[30];
    buffer[i++] = uuidBytes[31];
    buffer[i++] = uuidBytes[28];
    buffer[i++] = uuidBytes[29];
    buffer[i++] = uuidBytes[26];
    buffer[i++] = uuidBytes[27];
    buffer[i++] = uuidBytes[24];
    buffer[i++] = uuidBytes[25];
    buffer[i++] = uuidBytes[22];
    buffer[i++] = uuidBytes[23];
    buffer[i++] = uuidBytes[20];
    buffer[i++] = uuidBytes[21];
    buffer[i++] = uuidBytes[18];
    buffer[i++] = uuidBytes[19];
    buffer[i++] = uuidBytes[16];
    buffer[i++] = uuidBytes[17];

    auto newString = std::string(buffer, sizeof(buffer) / sizeof(buffer[0]));

    return newString;
}

std::string ToUUID(std::string id)
{
    std::string result = "";

    result.append(id, 0, 8);
    result.append("-");
    result.append(id, 8, 4);
    result.append("-");
    result.append(id, 12, 4);
    result.append("-");
    result.append(id, 16, 4);
    result.append("-");
    result.append(id, 20, 12);

    return result;
}

#include <map>
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


uint32_t ReadThreeByteInt(const char *buf)
{
    uint32_t w = (char)buf[2] < 0 ? 0xff000000 : 0;
    uint32_t x = (uint8_t)buf[2] << 16;
    uint32_t y = (uint8_t)buf[1] << 8;
    uint32_t z = (uint8_t)buf[0] << 0;

    uint32_t result = w | x | y | z;

    return result;
}

void WriteThreeByteInt(int32_t value, char *buff)
{
    ((uint8_t *)buff)[2] = (uint8_t)((value >> 16) & 0xFF);
    ((uint8_t *)buff)[1] = (uint8_t)((value >> 8) & 0xFF);
    ((uint8_t *)buff)[0] = (uint8_t)((value) & 0xFF);
}

#include <vector>

void OnUserLoginReply(PacketReader &reader)
{
    printf("ClientRegionMessages::UserLoginReply\n");

    auto success = reader.ReadUint8();
    auto sessionId = reader.ReadUint32();
    auto regionServerVersion = reader.ReadString();

    std::vector<std::string> privileges;
    auto count = reader.ReadUint32();
    for (size_t i = 0; i < count; i++)
    {
        auto priv = reader.ReadString();
        privileges.push_back(priv);
    }

    printf("LoginReply:\n Success: %d\n SessionId: %d\n RegionServerVersion: %s\n Privileges: \n", success, sessionId, regionServerVersion.c_str());
    for (const auto &item : privileges)
    {
        printf("   %s\n", item.c_str());
    }
}

#include <regex>
#include <filesystem>
#include <chrono>

std::regex patternAvatarType("avatarAssetId = \"([^\"]+)\"[^a]+avatarInventoryId = \"([^\"]+)\"");

void OnAddUser(PacketReader &reader)
{
    printf("ClientRegionMessages::AddUser\n");

    auto sessionId = reader.ReadUint32();
    auto userName = reader.ReadString();
    auto handle = reader.ReadString();
    auto avatarType = reader.ReadString();
    auto personaId = reader.ReadUUID();

    auto personaIdButts = ClusterButt(personaId);
    auto personaIdFormatted = ToUUID(personaId);

    sessionIdToNameMap[sessionId] = userName;

    std::string avatarAssetId = "";
    std::string avatarInventoryId = "";

    std::smatch match;
    std::regex_search(avatarType, match, patternAvatarType);

    if (match[1].matched)
    {
        avatarAssetId = match[1].str();
    }
    if (match[2].matched)
    {
        avatarInventoryId = match[2].str();
    }

    auto avatarAssetIdSwapped = ClusterButt(avatarAssetId);

    std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\userdump.csv";

    bool needToAddHeader = false;
    if (!std::filesystem::exists(userdumpPath) || std::filesystem::file_size(userdumpPath) == 0)
    {
        needToAddHeader = true;
    }

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    FILE *outFile = nullptr;
    fopen_s(&outFile, userdumpPath.string().c_str(), "a");
    if (needToAddHeader)
    {
        fprintf(outFile, "timestamp,username,handle,personaIdSwapped,avatarAssetIdSwapped,personaId,avatarAssetId,avatarInventoryId\n");
    }
    fprintf(outFile, "\"%lld\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
        timestamp,
        userName.c_str(),
        handle.c_str(),
        personaIdButts.c_str(),
        avatarAssetIdSwapped.c_str(),
        personaIdFormatted.c_str(),
        avatarAssetId.c_str(),
        avatarInventoryId.c_str()
    );
    fclose(outFile);

    printf("  SessionID: %d\n  Username: %s\n  Handle: %s\n  PersonaId: %s [%s]\n  avatarAssetId: %s [%s]\n  AvatarType: '%s'\n",
        sessionId,
        userName.c_str(),
        handle.c_str(),
        personaIdFormatted.c_str(),
        personaIdButts.c_str(),
        avatarAssetId.c_str(),
        avatarAssetIdSwapped.c_str(),
        avatarType.c_str()
    );
}


void OnRemoveUser(PacketReader &reader)
{
    auto sessionId = reader.ReadUint32();

    auto userName = std::string();
    auto foundName = sessionIdToNameMap.find(sessionId);
    if (foundName != sessionIdToNameMap.end())
    {
        userName = foundName->second;
    }
    else
    {
        userName = "UNKNOWN";
    }

    printf("ClientRegionMessages::RemoveUser -> (%s) %d\n", userName.c_str(), sessionId);
}

uint16_t lastSequence = 0;

std::mutex recvLock;


std::map<SOCKET, PacketReader> readers;
std::map<SOCKET, uint16_t> lastProcessedSequence;

int WINAPI Hooked_Recvfrom(SOCKET s, char *buff, int len, int flags, struct sockaddr *from, int *fromlen)
{
    auto result = original_recvfrom(s, buff, len, flags, from, fromlen);
    if (result == SOCKET_ERROR)
    {
        return result;
    }

    if (buff == nullptr || len < 3)
    {
        return result;
    }

    auto packetType = *((uint8_t *)&buff[0]);
    auto sequence = *((uint16_t *)&buff[1]);

    if (packetType != 0x07)
    {
        return result;
    }

    // std::unique_lock<std::mutex> lck(recvLock);

    PacketReader &reader = readers[s];
    lastProcessedSequence[s] = reader.getPreviousSequence();

    if (sequence <= lastProcessedSequence[s])
    {
        //  printf("Ignoring old packet sequence %d\n", sequence);
        return result;
    }

    //printf("[%d] Sequence = %d | Len = %d\n", s, sequence, len);

    //DumpPacket(buff, result, false);


    // |Type (1)|Sequence (2)|<packet>[1..N]|Checksum

    // <packet>:
    //   If <= 512 bytes
    //      |Length|Message ID|<Payload>
    //   Else
    //      |0xFF|2-byte Length|MessageId|<Payload>

    // NOTE: <Payload> may be split between multiple packets, so |Length| may refer to all of the remaining data
    //       in the current packet (excluding the two byte checksum and 4-byte message id) and the next N bytes
    //       in the next packets (excluding their headers and checksums)

    // For each chunk:
    //   Confirm first byte is 0x07
    //   Ignore sequence (second two bytes)
    //   Copy the packet to the PacketReader buffer (excluding 3-byte header and 2-byte checksum footer)
    //   Attempt to process packets (if we're done waiting for more data)
    //   
    // For each packet...
    //   Read the length.
    //        If the length is within the packet, process the packet.
    //        If the length exceeds the packet, un-read the length and                          [wait for the packet reader buffer to fill with the next packet(s)]
    //        If the length is 0xFF, read the full length and store it. Un-read the lengths and [Wait for the packet reader buffer to fill with the next packet(s)]
    //

    // |Type(1)|Sequence(2)|Len-0(1)|Id-0(4)
    // |Type(1)|Sequence(2)|Len-0(1)|Id-0(4)|<payload 0>|Len-1(1)|Id-1(4)|<payload 1>|...|Len-N(1)|Id-N(4)|<payload N>
    // |Type(1)|Sequence(2)|Len-0(1)|Id-0(4)


    // Copy the packet to the PacketReader buffer (excluding 3-byte header and 2-byte checksum footer)
    reader.Add(sequence, (const uint8_t *)(buff + 3), result - 5);

    try
    {
        // Attempt to process packets (if we're done waiting for more data)
        while (reader.GetBytesRemaining() > 0)
        {

            // For each packet...
            //   Read the length.
            //        If the length is within the packet, process the packet.
            //        If the length exceeds the packet, un-read the length and                          [wait for the packet reader buffer to fill with the next packet(s)]
            //        If the length is 0xFF, read the full length and store it. Un-read the lengths and [Wait for the packet reader buffer to fill with the next packet(s)]
            //

            uint64_t packetLength = reader.ReadUint8();
            if (packetLength == 0xFF)
            {
                packetLength = reader.ReadUint16();
                if (packetLength == 0xFFFF)
                {
                    // 32-bit length

                    packetLength = reader.ReadUint32();

                    //printf("This is a huge message. We need to wait for %d more bytes. Our buffer currently has %d bytes remaining. So in total we need %d bytes.\n", packetLength, reader.GetBytesRemaining(), packetLength - reader.GetBytesRemaining());

                    if (reader.GetBytesRemaining() - (int64_t)packetLength < 0)
                    {
                        reader.Skip(-7); // Un-read the 1bit, 2bit, 4bit lengths
                        return result;
                    }
                }
                else
                {
                    // 16-bit length

                   // printf("This is a big message. We need to wait for %d more bytes. Our buffer currently has %d bytes remaining. So in total we need %d bytes.\n", packetLength, reader.GetBytesRemaining(), packetLength - reader.GetBytesRemaining());

                    if (reader.GetBytesRemaining() - (int64_t)packetLength < 0)
                    {
                        reader.Skip(-3); // Un-read the 1bit,2bit lengths
                        return result;
                    }

                }

                //   printf("Ignore the previous message. We're actually done waiting and can now process this %d byte message with %d bytes to spare in the buffer.\n", packetLength, (reader.GetBytesRemaining() - packetLength));
            }
            else if ((int64_t)packetLength > reader.GetBytesRemaining())
            {
                // printf("This is a small message split between two packets. We need to wait for %d more bytes. Our buffer currently has %d bytes remaining. So in total we need %d bytes.\n", packetLength, reader.GetBytesRemaining(), packetLength - reader.GetBytesRemaining());

                if (reader.GetBytesRemaining() - (int64_t)packetLength < 0)
                {
                    reader.Skip(-1); // Un-read the length
                    return result;
                }

                // printf("Ignore the previous message. We're actually done waiting and can now process this message with %d bytes to spare in the buffer.\n", packetLength, (reader.GetBytesRemaining() - packetLength));
            }

            auto messageId = reader.ReadUint32();
            //printf("MessageId = %08X\n", messageId);

            if (messageId == ClientRegionMessages::UserLoginReply)
            {
                OnUserLoginReply(reader);
            }
            else if (messageId == ClientRegionMessages::AddUser)
            {
                OnAddUser(reader);
            }
            else
            {
                reader.Skip(packetLength - 4);
            }
        }

        reader.Reset();
    }
    catch (std::exception &)
    {
        printf("Shit hit the fan. Clearning reader on socket %lld.\n", s);
        reader.Reset();

        lastProcessedSequence.erase(s);
        readers.erase(s);
    }

    return result;
}


#include <windows.h>
#include <tlhelp32.h>
uint8_t *kBaseAddress = nullptr;

uint8_t *GetBaseAddress()
{
    if (kBaseAddress != nullptr)
    {
        return kBaseAddress;
    }

    auto process_id = GetCurrentProcessId();

    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPALL, process_id);
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    MODULEENTRY32 module_entry = { 0 };
    module_entry.dwSize = sizeof(module_entry);

    if (Module32First(hSnap, &module_entry))
    {
        do
        {
            if (module_entry.th32ProcessID == process_id)
            {
                kBaseAddress = (uint8_t *)module_entry.modBaseAddr;
                CloseHandle(hSnap);

                return kBaseAddress;
            }
        } while (Module32Next(hSnap, &module_entry));
    }

    CloseHandle(hSnap);
    return nullptr;
}


int WINAPI Hooked_Sendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
    if (s == last_chat_sendto_socket)
    {
        last_chat_server_sequence = *((uint16_t *)&buf[1]);
        //printf("** last_chat_server_sequence = %X\n", last_chat_server_sequence);
    }
    //  DumpPacket(buf, len, true);

    if (buf == nullptr || len < 8)
    {
        return original_sendto(s, buf, len, flags, to, tolen);
    }

    // (1:ID) (2:sequence) (1:payload length) (N: payload) (2: checksum)

    last_sent_sequence = *((uint16_t *)&buf[1]);
    auto messageId = *((uint32_t *)&buf[4]);


    if (messageId == ClientKafkaMessages::Login)
    {
        // where is this sent..?
        printf("ClientKafkaMessage::Login ->\n");
        DumpPacket(buf, len, true);
    }
    else if (messageId == ClientRegionMessages::UserLogin)
    {
        // ClientRegionMessages::UserLogin ->  07 | 03 00 | 08 | 0A 80 02 39 | 71 06 F7 77 | E4 8B
        auto secret = *((uint32_t *)&buf[8]);
        printf("ClientRegionMessages::UserLogin (secret = %u | %X)\n", secret, secret);
    }
    else if (messageId == ClientRegionMessages::SetAgentController)
    {
        printf("ClientRegionMessages::SetAgentController --> \n");
        DumpPacket(buf, len, true);
    }
    else if (messageId == ClientRegionMessages::SubscribeCommand)
    {
        printf("ClientRegionMessages::SubscribeCommand --> \n");
        DumpPacket(buf, len, true);
    }
    else if (messageId == ClientRegionMessages::RequestDropPortal)
    {
        printf("ClientRegionMessages::RequestDropPortal --> \n");
        DumpPacket(buf, len, true);
    }

    if (buf[0] != 0x01 && buf[0] != 0x07)
    {
        return original_sendto(s, buf, len, flags, to, tolen);
    }


    //printf("Hooked sendto [%s] -> %d\n", GetAddressFromAddr(to).c_str(), result);
    if (buf[0] == 0x01)
    {
        auto seed = *(uint32_t *)&buf[5];
        auto seedNameLength = *(uint32_t *)&buf[13];
        auto seedName = std::string((const char *)&buf[17], (std::size_t)seedNameLength);

        if (seedName == "CRC_102")
        {
            printf("SEND checksum [%s] Seed = %08X | TO=%08llX | s=%08llX\n", seedName.c_str(), seed, (uint64_t)to, s);
            checksumSeed_send = seed;
        }
        else if (seedName == "CVC_100")
        {
            printf("RECV checksum [%s] Seed = %08X | TO=%08llX | s=%08llX\n", seedName.c_str(), seed, (uint64_t)to, s);

            if (!found_first_cvc100)
            {
                found_first_cvc100 = true;
                checksumSeed_first_cvc100 = seed;
            }
            else
            {
                checksumSeed_recv = seed;
            }
        }
        else
        {
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
            auto unknownA = *((uint16_t *)&buf[8]);
            auto accountIdBytes = ((uint8_t *)&buf[10]);
            auto personaIdBytes = ((uint8_t *)&buf[26]);
            auto secret = *((uint32_t *)&buf[42]);
            auto Unknown = *((uint64_t *)&buf[46]);

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


            auto myChecksum = Sansar_ShortCRC((uint8_t *)buf, len - 2, checksumSeed_recv); // yes this is the correct checksum to use here despite its name...
            *((uint16_t *)&buf[len - 2]) = myChecksum;
        }
    }
    else if (buf[0] == 0x07)
    {
        // rez item
        if (messageId == AgentControllerMessages::RequestSpawnItem)
        {
            //  Id   Seq     MsgId            Frame                     AgentId       ResourceId                            AttachmentNode   SpawnPosition(78bit)|height?(32bit)|SpawnOrientation(43bit)         Checksum
            // [07] [91 01] [31 0D 85 21] 2C [C6 6D 00 00 00 00 00 00] [05 00 00 00] [37 7F 61 06 51 D3 9B EF 5F B0 FB 67 89 AD EE 79] [FF] [EB AE 00 F2 3A 02 48 2A A0 C0 FE BF FF F7 FE 51] [77 96]

            auto frame = *((uint64_t *)&buf[8]);
            auto agentControllerId = *((uint32_t *)&buf[16]);
            auto clusterId = ((uint8_t *)&buf[20]);
            auto attachmentNode = *((uint8_t *)&buf[36]); // 9

            BitReader br((uint8_t *)&buf[37], 10);
            auto positionX = br.ReadFloat(26, false);
            auto positionY = br.ReadFloat(26, false);
            auto positionZ = br.ReadFloat(26, true);

            auto camrez_enabled = false;
            FILE *inFileCamRez = nullptr;
            fopen_s(&inFileCamRez, "u:\\sanhook_config_camrez.txt", "rb");
            if (inFileCamRez != nullptr)
            {
                camrez_enabled = fgetc(inFileCamRez);
                fclose(inFileCamRez);
            }
            if (camrez_enabled)
            {
                auto base = GetBaseAddress();

                // We got this constant from memory.
                // Search for "no-input-source".
                // Between "room scale" and "no-input-source"
                // Right below a call to RotMatrix, enter that call
                // rcx+30 = our pointer
                // Function above it with a bunch of xmm stuff going on (see screenshots)
                //const static auto kCameraPositionOffset = 0x4AA91C0;
                const static auto kCameraPositionOffset = 0x4AA91C0;

                positionX = *((float *)(base + kCameraPositionOffset + 0));
                positionY = *((float *)(base + kCameraPositionOffset + 4));
                positionZ = *((float *)(base + kCameraPositionOffset + 8));

                br.Reset();
                br.WriteFloat(positionX, 26, false);
                br.WriteFloat(positionY, 26, false);
                br.WriteFloat(positionZ, 26, true);

                br.Reset();
                positionX = br.ReadFloat(26, false);
                positionY = br.ReadFloat(26, false);
                positionZ = br.ReadFloat(26, true);
            }

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
            //DumpPacket(buf, len, true);
            //////////////////////////////

            uint8_t newClusterId[16] = {};
            bool enabled = false;
            FILE *inFile = nullptr;
            fopen_s(&inFile, "u:\\sanhook_config_rez.txt", "rb");
            if (inFile != nullptr)
            {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(newClusterId, sizeof(newClusterId), sizeof(newClusterId[0]), 16, inFile);
                fclose(inFile);

                if (bytesRead == 16 && enabled)
                {
                    memcpy(clusterId, newClusterId, 16);
                }
            }

            auto myChecksum = Sansar_ShortCRC((uint8_t *)buf, len - 2, checksumSeed_send);
            *((uint16_t *)&buf[len - 2]) = myChecksum;
            ///////////////////////
        }
        // instant message
        if (0 && messageId == ClientKafkaMessages::PrivateChat)
        {
            auto unknownA = *((uint64_t *)&buf[8]);
            auto personaIdFromBytes = ((uint8_t *)&buf[16]);
            auto personaIdToBytes = ((uint8_t *)&buf[32]);

            auto textLength = *((uint32_t *)&buf[48]);
            auto text = std::string((const char *)&buf[52], (std::size_t)textLength);

            auto unknownB = *((uint64_t *)&buf[52 + textLength]);

            //printf("From = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
            //    personaIdFromBytes[7],
            //    personaIdFromBytes[6],
            //    personaIdFromBytes[5],
            //    personaIdFromBytes[4],
            //    personaIdFromBytes[3],
            //    personaIdFromBytes[2],
            //    personaIdFromBytes[1],
            //    personaIdFromBytes[0],
            //    personaIdFromBytes[15],
            //    personaIdFromBytes[14],
            //    personaIdFromBytes[13],
            //    personaIdFromBytes[12],
            //    personaIdFromBytes[11],
            //    personaIdFromBytes[10],
            //    personaIdFromBytes[9],
            //    personaIdFromBytes[8]
            //);
            //printf("To = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
            //    personaIdToBytes[7],
            //    personaIdToBytes[6],
            //    personaIdToBytes[5],
            //    personaIdToBytes[4],
            //    personaIdToBytes[3],
            //    personaIdToBytes[2],
            //    personaIdToBytes[1],
            //    personaIdToBytes[0],
            //    personaIdToBytes[15],
            //    personaIdToBytes[14],
            //    personaIdToBytes[13],
            //    personaIdToBytes[12],
            //    personaIdToBytes[11],
            //    personaIdToBytes[10],
            //    personaIdToBytes[9],
            //    personaIdToBytes[8]
            //);


            //////////////////////////////
            uint8_t personaId[16] = {};
            bool enabled = false;

            FILE *inFile = nullptr;

            fopen_s(&inFile, "u:\\sanhook_config_im.txt", "rb");
            if (inFile != nullptr)
            {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                if (bytesRead == 16)
                {
                    memcpy(personaIdFromBytes, personaId, 16);
                }

                bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                if (bytesRead == 16)
                {
                    memcpy(personaIdToBytes, personaId, 16);
                }
                fclose(inFile);
            }
            ///////////////////////

            if (enabled)
            {
                memcpy(personaIdFromBytes, &buf[16], 16);
                memcpy(personaIdToBytes, &buf[32], 16);
            }

            static bool aa = false;
            if (aa)
            {
                static unsigned char otherAccount[16] = {
                    0x02, 0x46, 0x98, 0x5E, 0xB8, 0xE5, 0x48, 0xC7, 0xD7, 0xAC, 0x86, 0x2B,
                    0xA4, 0x83, 0xBE, 0x8D
                };
                memcpy(personaIdFromBytes, otherAccount, sizeof(otherAccount) / sizeof(otherAccount[0]));
            }

            static bool bb = false;
            if (bb)
            {
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
            auto unknownB = *((uint32_t *)&buf[8]);
            auto Time = *((uint64_t *)&buf[12]);
            auto unknownD = *((uint32_t *)&buf[16]);
            auto unknownE = *((uint32_t *)&buf[24]);

            auto ResourceIdBytes = ((uint8_t *)&buf[28]);

            auto unknownF = *((uint16_t *)&buf[44]);
            auto unknownG = *((uint8_t *)&buf[46]);

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
        else if (0 && messageId == ClientRegionMessages::ClientRegionCommandMessage)
        {
            auto textLength = *((uint32_t *)&buf[8]);
            auto text = std::string((const char *)&buf[12], (std::size_t)textLength);

            if (text == "test123")
            {
                // auto result = original_sendto(s, buf, len, flags, to, tolen);
                auto result = SpecialChat("Intercepted: " + text, last_chat_server_sequence + 1, s, flags, to, tolen, checksumSeed_first_cvc100);
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

            auto payload = (uint8_t *)&buf[8];
            auto personaIdBytes = (uint8_t *)&payload[0];
            auto unknownB = *((uint64_t *)&payload[16]);
            auto unknownC = *((uint64_t *)&payload[24]);
            auto unknownD = *((uint64_t *)&payload[32]);
            auto textLength = *((uint32_t *)&payload[40]);
            auto text = std::string((const char *)&payload[44], (std::size_t)textLength);
            auto unknownF = *((uint64_t *)&payload[44 + textLength]);
            auto unknownG = *((uint64_t *)&payload[44 + textLength + 8]);
            auto unknownH = *((uint64_t *)&payload[44 + textLength + 16]);
            auto unknownI = *((uint64_t *)&payload[44 + textLength + 24]);
            auto textLen2 = *((uint32_t *)&payload[44 + textLength + 32]);

            /*
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
            );*/

            static unsigned char otherPersona[16] = {
                0x96, 0x45, 0xD3, 0x64, 0xB7, 0x9C, 0x1F, 0xD6, 0x54, 0x83, 0x78, 0xF7,
                0x8F, 0x42, 0x69, 0xA3
            };

            //////////////////////////////
            uint8_t personaId[16] = {};
            bool enabled = false;

            FILE *inFile = nullptr;

            fopen_s(&inFile, "u:\\sanhook_config.txt", "rb");
            if (inFile != nullptr)
            {
                enabled = fgetc(inFile);

                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                fclose(inFile);

                if (bytesRead == 16)
                {
                    memcpy(otherPersona, personaId, 16);
                }
            }
            ///////////////////////

            if (enabled)
            {
                memcpy(personaIdBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));
            }
        }
        else if (0 && messageId == ClientRegionMessages::ChatMessageToServer)
        {
            //last_chat_sendto = to;
            last_chat_sendto_length = tolen;
            last_chat_sendto_flags = flags;
            last_chat_sendto_socket = s;

            auto payload = (uint8_t *)&buf[8];

            auto unknownB = *((uint32_t *)&payload[0]);
            auto textLength = *((uint32_t *)&payload[4]);

            memset(&payload[8], '*', textLength);

            auto text = std::string((const char *)&payload[8], (std::size_t)textLength);

            auto checksum = *((uint16_t *)&buf[len - 2]);
            auto myChecksum = Sansar_ShortCRC((uint8_t *)buf, len - 2, checksumSeed_send);

            //printf("Chat: [%d] %s\n", unknownB, text.c_str());
        }
    }

    //printf("Socket: %08X | sendto: %08X | tolen: %d\n", s, to, tolen);
    //printf("^^: GetAddressFromAddr -> %s\n", GetAddressFromAddr(to).c_str());

   //  DumpPacket(buf, len, true);

    auto result = original_sendto(s, buf, len, flags, to, tolen);
    return result;
}


int WINAPI Hooked_Recv(SOCKET s, char *buf, int len, int flags)
{
    auto result = original_recv(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("hooked recv [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        //DumpPacket(buf, result, false);
    }

    return result;
}


int WINAPI Hooked_Send(SOCKET s, const char *buf, int len, int flags)
{
    auto result = original_send(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        printf("Hooked send [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        DumpPacket(buf, len, true);
    }

    return result;
}

#include "Magic.h"
unsigned long long ReturnPoint_ProcessPacketRecv = 0;

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


void OnChatMessageToClient(PacketReader &reader)
{
    auto fromSessionId = reader.ReadUint32();
    auto toSessionId = reader.ReadUint32();
    auto message = reader.ReadString();

    auto username = std::string();

    auto usernameFromSessionId = sessionIdToNameMap.find(fromSessionId);
    if (usernameFromSessionId == sessionIdToNameMap.end())
    {
        username = "UNKNOWN";
    }
    else
    {
        username = usernameFromSessionId->second;
    }

    printf("OnChatMessageToClient: (%s) %d -> %d: %s\n", username.c_str(), fromSessionId, toSessionId, message.c_str());
}


void OnClientVoiceLoginReply(PacketReader &reader)
{
    auto success = reader.ReadUint8();
    auto message = reader.ReadString();

    printf("OnClientVoiceLoginReply: Success=%d: %s\n", success, message.c_str());
}

void OnLocalAudioStreamState(PacketReader &reader)
{
    auto instance = reader.ReadString();
    auto agentControllerId = reader.ReadUint32();
    auto broadcast = reader.ReadUint8();
    auto mute = reader.ReadUint8();

    printf("OnClientVoiceLoginReply:\n  instance=%s\n  agentControllerId = %u\n  broacast = %u\n  mute = %u\n",
        instance.c_str(),
        agentControllerId,
        broadcast,
        mute
    );
}


void OnScriptConsoleLog(PacketReader &reader)
{
    auto logLevel = reader.ReadUint32();
    auto tag = reader.ReadString();
    auto message = reader.ReadString();
    auto timestamp = reader.ReadUint64();
    auto scriptId = reader.ReadUint32();
    auto scriptClassName = reader.ReadString();
    auto worldId = reader.ReadString();
    auto instanceId = reader.ReadString();
    auto ownerPersonaId = reader.ReadUUID();
    auto offset = reader.ReadUint64();

    printf("OnScriptConsoleLog: logLevel = %u\n  tag = %s\n  message = %s\n  timestamp = %llu\n  scriptId = %u\n  scriptClassName = %s\n  worldId = %s\n  instanceId = %s\n  ownerPersonaId = %s\n  offset = %llu\n",
        logLevel,
        tag.c_str(),
        message.c_str(),
        timestamp,
        scriptId,
        scriptClassName.c_str(),
        worldId.c_str(),
        instanceId.c_str(),
        ownerPersonaId.c_str(),
        offset
    );
}

void OnCharacterControllerInput(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto agentControllerId = reader.ReadUint32();
    auto jumpState = reader.ReadUint8();
    auto jumpBtnPressed = reader.ReadUint8();

    printf("OnCharacterControllerInput: Frame = %llu | Agent = %u | jumpState = %u | jump = %u\n",
        frame,
        agentControllerId,
        jumpState,
        jumpBtnPressed
    );
    /*
    auto moveRight = ;
    auto moveForward = ;
    auto cameraYaw = ;
    auto cameraPitch = ;
    auto cameraYaw = ;
    auto cameraYaw = ;
    auto cameraYaw = ;
    auto cameraYaw = ;
    ... TBD, weird packed float stuff again
    */
}

void OnClientKafkaMessageLoginReply(PacketReader &reader)
{
    auto success = reader.ReadUint8();
    auto message = reader.ReadString();

    printf("ClientKafkaMessages::LoginReply: Success = %u (Message = '%s')\n", success, message.c_str());
}

void OnClientKafkaMessagePresenceUpdate(PacketReader &reader)
{
    auto personaId = reader.ReadUUID();
    auto present = reader.ReadUint8();
    auto sessionId = reader.ReadUUID();
    auto state = reader.ReadString();
    auto sansarUri = reader.ReadString();

    printf("ClientKafkaMessages::PresenceUpdate:\n  personaId = %s\n  present = %u\n  sessionId = %s\n  state = %s\n  sansarUri = %s\n",
        personaId.c_str(),
        present,
        sessionId.c_str(),
        state.c_str(),
        sansarUri.c_str()
    );
}

void OnClientKafkaMessageRelationshipTable(PacketReader &reader)
{
    auto other = reader.ReadUUID();
    auto fromSelf = reader.ReadUint8();
    auto fromOther = reader.ReadUint8();
    auto status = reader.ReadUint32();

    printf("ClientKafkaMessages::RelationshipTable: other = %s, fromSelf = %u, fromOther = %u, status = %u\n",
        other.c_str(),
        fromSelf,
        fromOther,
        status
    );
}

void OnInventoryItemUpdate(PacketReader &reader)
{
    auto id = reader.ReadString();
    auto licensee_label = reader.ReadString();
    auto licensor_label = reader.ReadString();
    auto compat_version = reader.ReadString();
    auto licensor_pid = reader.ReadString();
    auto creationTime = reader.ReadString();
    auto modificationTime = reader.ReadString();

    auto origin = reader.ReadUint32();
    auto originReference = reader.ReadString();
    auto revisionsLength = reader.ReadUint32();

    auto offset = reader.ReadUint64();
    auto state = reader.ReadUint8();

    printf("ClientKafkaMessages::InventoryItemUpdate:\n  id = %s\n  licensee_label = %s\n  licensor_label = %s\n  compat_version = %s\n  licensor_pid = %s\n  creationTime = %s\n  modificationTime = %s\n  origin = %u\n  originReference = %s\n  revisionsLength = %u\n  offset = %llu\n  state = %u\n",
        id.c_str(),
        licensee_label.c_str(),
        licensor_label.c_str(),
        compat_version.c_str(),
        licensor_pid.c_str(),
        creationTime.c_str(),
        modificationTime.c_str(),
        origin,
        originReference.c_str(),
        revisionsLength,
        offset,
        state
    );
}



void OnEditServerAddUser(PacketReader &reader)
{
    auto sessionId = reader.ReadUint32();
    auto userName = reader.ReadString();
    auto personaId = reader.ReadUUID();

    printf("EditServerMessages::AddUser\n  sessionId = %u\n  userName = %s\n  personaId = %s\n",
        sessionId,
        userName.c_str(),
        personaId.c_str()
    );
}

void OnEditServerUserLoginReply(PacketReader &reader)
{
    auto success = reader.ReadUint8();
    auto sessionId = reader.ReadUint32();
    auto editServerVersion = reader.ReadString();

    printf("EditServerMessages::LoginReply\n  success = %d\n  sessionId = %d\n  editServerVersion = %s\n",
        success,
        sessionId,
        editServerVersion.c_str()
    );
}

void OnEditServerWorkspaceReadyReply(PacketReader &reader)
{
    auto workspaceId = reader.ReadUint32();

    printf("EditServerMessages::WorkspaceReadyReply\n  workspaceId = %u\n",
        workspaceId
    );
}

void OnCharacterTransform(PacketReader &reader)
{
    auto componentId = reader.ReadUint64();
    auto serverFrame = reader.ReadUint64();
    auto groundComponentId = reader.ReadUint64();
    // auto position = ; // Weird packed float stuff again
    // auto orientationQuat = ; // Weird packed float stuff again

    printf("OnCharacterTransform:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
        componentId,
        serverFrame,
        groundComponentId
    );
}

void OnCharacterTransformPersistent(PacketReader &reader)
{
    auto componentId = reader.ReadUint64();
    auto serverFrame = reader.ReadUint64();
    auto groundComponentId = reader.ReadUint64();
    // auto position = ; // Weird packed float stuff again
    // auto orientationQuat = ; // Weird packed float stuff again

    printf("OnCharacterTransformPersistent:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
        componentId,
        serverFrame,
        groundComponentId
    );
}


void OnCreateWorld(PacketReader &reader)
{
    auto worldDefinition = reader.ReadUUID();
    auto startingClusterId = reader.ReadUint32();
    auto startingObjectId = reader.ReadUint32();

    printf("OnCreateWorld:\n  worldDefinition = %s\n  unk1 = %d\n  startingObjectId = %d\n",
        worldDefinition.c_str(),
        startingClusterId,
        startingObjectId
    );
}


void OnInitiateCluster(PacketReader &reader)
{
    auto clusterId = reader.ReadUint32();
    auto frame = reader.ReadUint64();
    auto rigidBodyInitialStatesLength = reader.ReadUint32();
    auto animationInitialStatesLength = reader.ReadUint32();

    printf("OnInitiateCluster:\n  clusterId = %d\n  frame = %llu\n  rigidBodyInitialStatesLength = %u\n  animationInitialStatesLength = %u\n",
        clusterId,
        frame,
        rigidBodyInitialStatesLength,
        animationInitialStatesLength
    );
}


void OnLoadClusterDefinition(PacketReader &reader)
{
    auto resourceId = reader.ReadUUID();
    auto clusterId = reader.ReadUint32();

    printf("OnLoadClusterDefinition:\n  resourceId = %s\n  clusterId = %d\n",
        resourceId.c_str(),
        clusterId
    );
}


void OnCreateClusterViaDefinition(PacketReader &reader)
{
    auto clusterId = reader.ReadUint32();
    auto startingObjectId = reader.ReadUint32();
    auto resourceId = reader.ReadUUID();

    /*
    // TODO: likely packed float crap again
    auto spawnPosition_x = reader.ReadFloat();
    auto spawnPosition_y = reader.ReadFloat();
    auto spawnPosition_z = reader.ReadFloat();

    // TODO: likely packed float crap again
    auto spawnRotation_x = reader.ReadFloat();
    auto spawnRotation_y = reader.ReadFloat();
    auto spawnRotation_z = reader.ReadFloat();
    auto spawnRotation_w = reader.ReadFloat();
    */

    printf("OnCreateClusterViaDefinition:\n  clusterId = %u\n  startingObjectId = %u\n  resourceId = %s\n",
        clusterId,
        startingObjectId,
        resourceId.c_str()
    );
}


void OnSetAgentController(PacketReader &reader)
{
    auto agentControllerId = reader.ReadUint32();
    auto frame = reader.ReadUint64();

    printf("OnSetAgentController:\n  agentControllerId = %u\n  frame = %llu\n",
        agentControllerId,
        frame
    );
}


void OnCreateAgentController(PacketReader &reader)
{
    auto sessionId = reader.ReadUint32();
    auto clusterId = reader.ReadUint32();
    auto agentControllerId = reader.ReadUint32();
    auto characterObjectId = reader.ReadUint32();
    auto characterNodesLength = reader.ReadUint32();

    // this might just be 3 * characterNodesLength of stuff, but it doesn't matter. skip it all
    auto nonFillerBytesLeft = 1 + 16 + 8;
    auto fillerBytes = reader.GetBytesRemaining() - nonFillerBytesLeft;
    reader.Skip(fillerBytes);

    auto frame = reader.ReadUint64();
    auto personaId = reader.ReadUUID();
    auto isRemoteAgent = reader.ReadUint8();

    printf("OnCreateAgentController:\n  sessionId = %d\n  clusterId = %u\n  agentControllerId = %u\n  characterObjectId = %u\n  characterNodesLength = %u\n  frame = %llu\n  personaId = %s\n  isRemoteAgent = %d\n",
        sessionId,
        clusterId,
        agentControllerId,
        characterObjectId,
        characterNodesLength,
        frame,
        personaId.c_str(),
        isRemoteAgent
    );
}


void OnInitialTimestamp(PacketReader &reader)
{
    auto nanoseconds = reader.ReadUint64();
    auto frame = reader.ReadUint64();

    printf("OnInitialTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
}


void OnTimestamp(PacketReader &reader)
{
    auto nanoseconds = reader.ReadUint64();
    auto frame = reader.ReadUint64();

    printf("OnTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
}

void OnKafkaRegionChat(PacketReader &reader)
{
    auto fromPersonaId = reader.ReadUUID();
    auto toPersonaId = reader.ReadUUID();
    auto instanceAddress = reader.ReadString();
    auto agentControllerId = reader.ReadUint32();
    auto message = reader.ReadString();
    auto timestamp = reader.ReadUint64();
    auto typing = reader.ReadUint8();
    auto offset = reader.ReadUint64();
    auto highwaterMarkOffset = reader.ReadUint64();

    auto fromPersonaIdButts = ClusterButt(fromPersonaId);
    auto fromPersonaIdFormatted = ToUUID(fromPersonaId);

    auto toPersonaIdButts = ClusterButt(toPersonaId);
    auto toPersonaIdFormatted = ToUUID(toPersonaId);

    printf("ClientKafkaMessages::RegionChat:\n  From = %s [%s]\n  To = %s [%s]\n  instanceAddress = %s\n  agentControllerId = %d\n  message = %s\n  timestamp = %llu\n  typing = %u\n  offset = %llu\n  highwaterMarkOffset = %llu\n",
        fromPersonaIdFormatted.c_str(),
        fromPersonaIdButts.c_str(),
        toPersonaIdFormatted.c_str(),
        toPersonaIdButts.c_str(),
        instanceAddress.c_str(),
        agentControllerId,
        message.c_str(),
        timestamp,
        typing,
        offset,
        highwaterMarkOffset
    );
}

void OnPrivateChat(PacketReader &reader)
{
    auto offset = reader.ReadUint64();
    auto fromPersonaId = reader.ReadUUID();
    auto toPersonaId = reader.ReadUUID();
    auto message = reader.ReadString();
    auto timestamp = reader.ReadUint64();

    auto fromPersonaIdButts = ClusterButt(fromPersonaId);
    auto fromPersonaIdFormatted = ToUUID(fromPersonaId);

    auto toPersonaIdButts = ClusterButt(toPersonaId);
    auto toPersonaIdFormatted = ToUUID(toPersonaId);

    printf("ClientKafkaMessages::OnPrivateChat:\n  From = %s [%s]\n  To = %s [%s]\n   message = %s\n  timestamp = %llu\n  offset = %llu\n",
        fromPersonaIdFormatted.c_str(),
        fromPersonaIdButts.c_str(),
        toPersonaIdFormatted.c_str(),
        toPersonaIdButts.c_str(),
        message.c_str(),
        timestamp,
        offset
    );
}

void OnPrivateChatUpdate(PacketReader &reader)
{
    auto offset = reader.ReadUint64();
    auto status = reader.ReadUint32();

    printf("ClientKafkaMessages::OnPrivateChatUpdateStatus:\n  offset = %llu\n  status = %u\n",
        offset,
        status
    );
}


void OnLoadSound(PacketReader &reader)
{
    auto resourceId = reader.ReadUUID();

    printf("AudioMessages::LoadSound:\n  resourceId = %s\n",
        resourceId.c_str()
    );
}


void OnPlaySound(PacketReader &reader)
{
    auto resourceId = reader.ReadUUID();
    auto createPlayHandleId = reader.ReadUint64();
    auto frame = reader.ReadUint64();
    auto componentId = reader.ReadUint64();
    auto positionX = reader.ReadFloat();
    auto positionY = reader.ReadFloat();
    auto positionZ = reader.ReadFloat();
    auto loudness = reader.ReadFloat();
    auto pitch = reader.ReadFloat();
    auto playOffset = reader.ReadUint32();
    auto flags = reader.ReadUint8();

    printf("AudioMessages::PlaySound:\n  resourceId = %s\n  createPlayHandleId = %llu\n  frame = %llu\n  componentId = %llu\n  position = <%f, %f, %f>\n  loudness = %f\n  pitch = %f\n  playOffset = %u\n  flags = %u\n",
        resourceId.c_str(),
        createPlayHandleId,
        frame,
        componentId,
        positionX, positionY, positionZ,
        loudness,
        pitch,
        playOffset,
        flags
    );
}

void OnRigidBodyDeactivated(PacketReader &reader)
{
    auto componentId = reader.ReadUint64();
    auto frame = reader.ReadUint64();
    auto ownershipWatermark = reader.ReadUint8();
    //position = reader.   78bit floats
    // orientationQuat  43bit floats

    printf("SimulationManager::OnRigidBodyDeactivated:\n  componentId = %llu\n  frame = %llu\n  ownershipWatermark = %u\n",
        componentId,
        frame,
        ownershipWatermark
    );
}


void OnBehaviorInitializationData(PacketReader &reader)
{
    auto behaviorStateUpdatesLength = reader.ReadUint32();
    auto animationUpdatesLength = reader.ReadUint32();

    printf("AnimationComponentMessages::BehaviorInitializationData:\n  behaviorStateUpdatesLength = %u\n  animationUpdatesLength = %u\n",
        behaviorStateUpdatesLength,
        animationUpdatesLength
    );
}

void OnObjectInteractionUpdate(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto clusterId = reader.ReadUint32();
    auto objectId = reader.ReadUint32();
    auto enabled = reader.ReadUint8();

    printf("AnimationComponentMessages::BehaviorInitializationData:\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  enabled = %u\n",
        frame,
        clusterId,
        objectId,
        enabled
    );
}

void OnLightStateChanged(PacketReader &reader)
{
    auto componentId = reader.ReadUint64();
    auto frame = reader.ReadUint64();
    auto rgb_r = reader.ReadFloat();
    auto rgb_g = reader.ReadFloat();
    auto rgb_b = reader.ReadFloat();
    auto range = reader.ReadFloat();
    auto shadowPriority = reader.ReadUint32();
    auto spotSinHalfAngle = reader.ReadFloat();
    auto spotAngularFalloff = reader.ReadFloat();
    auto spotNearClip = reader.ReadFloat();

    printf("RenderMessages::OnLightStateChanged:\n  componentId = %llu\n  frame = %llu\n  rgb = <%f, %f, %f>\n  range = %f\n  shadowPriority = %u\n  spotSinHalfAngle = %f\n  spotAngularFalloff = %f\n  spotNearClip = %f\n",
        componentId,
        frame,
        rgb_r, rgb_g, rgb_b,
        range,
        shadowPriority,
        spotSinHalfAngle,
        spotAngularFalloff,
        spotNearClip
    );
}

void OnActiveRigidBodyUpdate(PacketReader &reader)
{
    auto componentId = reader.ReadUint64();
    auto frame = reader.ReadUint64();
    auto ownerId = reader.ReadUint32();
    auto ownershipWatermark = reader.ReadUint8();
    auto authority = reader.ReadUint8();
    // auto position // 78bit
    // auto orientationQuat // 43bit
    // auto linearVeolcity // 39bit
    // auto angularVelocity  // 36bit

    printf("SimulationMessages::ActiveRigidBodyUpdate:\n  componentId = %llu\n  frame = %llu\n  ownerId = %u\n  ownershipWatermark = %u\n  authority = %u\n",
        componentId,
        frame,
        ownerId,
        ownershipWatermark,
        authority
    );
}


void OnMoveEntity(PacketReader &reader)
{
    auto startFrame = reader.ReadUint64();
    auto componentId = reader.ReadUint64();
    auto startPosition_x = reader.ReadFloat();
    auto startPosition_y = reader.ReadFloat();
    auto startPosition_z = reader.ReadFloat();
    auto targetPosition_x = reader.ReadFloat();
    auto targetPosition_y = reader.ReadFloat();
    auto targetPosition_z = reader.ReadFloat();
    auto time = reader.ReadUint32();
    //auto startOrientation = reader.ReadBits(46);
    //auto targetOrientation = reader.ReadBits(46);
    //auto positionInterpMode = reader.ReadBits(4);
    //auto rotationInterpMode = reader.ReadBits(4);

    printf("GameWorldMessages::MoveEntity:\n  startFrame = %llu\n  componentId = %llu\n startPosition = <%f, %f, %f>\n  targetPosition = <%f, %f, %f>\n  time = %u\n",
        startFrame,
        componentId,
        startPosition_x, startPosition_y, startPosition_z,
        targetPosition_x, targetPosition_y, targetPosition_z,
        time
    );
}

void OnCreateSpeechGraphicsPlayer(PacketReader &reader)
{
    auto agentControllerId = reader.ReadUint32();

    auto numElements = reader.ReadUint32();
    auto sinkConfigData = reader.ReadBytes(numElements);

    printf("OnCreateSpeechGraphicsPlayer:\n  sinkConfigData = Array[%u]\n",
        numElements
    );
}

void OnPlayAnimation(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto componentId = reader.ReadUint64();
    auto resourceId = reader.ReadUUID();
    auto playbackSpeed = reader.ReadBits(16);
    auto skeletonType = reader.ReadBits(2);
    auto animationType = reader.ReadBits(3);
    auto playbackMode = reader.ReadBits(3);

    printf("AgentControllerMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
        frame,
        componentId,
        resourceId.c_str()
    );
}

void OnCharacterIKPoseDelta(PacketReader &reader)
{
    auto agentControllerId = reader.ReadUint32();
    auto frame = reader.ReadUint64();

    auto boneRotationsLength = reader.ReadUint32();
    //for (size_t i = 0; i < boneRotationsLength; i++)
    //{
    //    auto boneIndex = reader.ReadBits(6);
    //    auto localOrientation = reader.ReadBits(25);
    //}

    //auto rootBoneTranslationDelta = reader.ReadBits(27);

    printf("AgentControllerMessages::CharacterIKPoseDelta:\n  agentControllerId = %u\n  frame = %llu\n  boneRotationsLength = Array[%u]\n  rootBoneTranslationDelta = Array[27]\n",
        agentControllerId,
        frame,
        boneRotationsLength
    );
}


void OnCharacterIKPose(PacketReader &reader)
{
    // TODO: Nope, not handling this one

    auto agentControllerId = reader.ReadUint32();
    auto frame = reader.ReadUint64();
    auto boneRotationsLength = reader.ReadUint32();

    /*
    for (size_t i = 0; i < boneRotationsLength; i++)
    {
        auto boneIndex = reader.ReadBits(6);
        auto localOrientation = reader.ReadBits(40);
    }

    auto rootBoneTranslation = reader.ReadBits(42);
    */

    printf("AgentControllerMessages::CharacterIKPose:\n  agentControllerId = %u\n  frame = %llu\n",
        agentControllerId,
        frame
    );
}

void OnCharacterControlPointInputReliable(PacketReader &reader)
{
    // Todo: no payload?
    printf("AgentControllerMessages::CharacterControlPointInputReliable\n");
}

void OnCharacterControllerInputReliable(PacketReader &reader)
{
    // Todo: no payload?
    printf("AgentControllerMessages::CharacterControllerInputReliable\n");
}

void OnDestroyAgentController(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto agentControllerId = reader.ReadUint32();

    printf("WorldStateMessages::DestroyAgentController:\n  frame = %llu\n  agentControllerId = %u\n",
        frame,
        agentControllerId
    );
}

void OnCharacterControlPointInput(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto agentControllerId = reader.ReadUint32();

    auto controlPointsLength = reader.ReadUint32();

    //auto leftIndexTrigger = reader.ReadBits(8);
    //auto rightIndexTrigger = reader.ReadBits(8);
    //auto leftGripTrigger = reader.ReadBits(8);
    //auto rightGripTrigger = reader.ReadBits(8);
    //auto leftTouches = reader.ReadBits(7);
    //auto rightTouches = reader.ReadBits(7);
    //auto indexTriggerControlsHand = reader.ReadBits(1);
    //auto leftHandIsHolding = reader.ReadBits(1);
    //auto rightHandIsHolding = reader.ReadBits(1);

    printf("AgentControllerMessages::CharacterControlPointInput:\n  frame = %llu\n  agentControllerId = %u\n",
        frame,
        agentControllerId
    );
}

void OnSetLoudness(PacketReader &reader)
{
    auto playHandleId = reader.ReadUint32();
    auto loudness = reader.ReadFloat();

    printf("AudioMessages::SetLoudness:\n  playHandleId = %u\n  loudness = %f\n",
        playHandleId,
        loudness
    );
}

void OnSetPitch(PacketReader &reader)
{
    auto playHandleId = reader.ReadUint32();
    auto pitch = reader.ReadFloat();

    printf("AudioMessages::SetPitch:\n  playHandleId = %u\n  pitch = %f\n",
        playHandleId,
        pitch
    );
}


void OnBehaviorInternalState(PacketReader &reader)
{
    // TODO: Nope, not handling this one

    auto componentId = reader.ReadUint64();
    auto frame = reader.ReadUint64();

    auto overridesLength = reader.ReadUint32();
    /* auto overridesData = reader.ReadBytes(overridesLength);

     auto numSlotStates = reader.ReadUint32();
     for (size_t i = 0; i < numSlotStates; i++)
     {
         auto slotState = reader.ReadUint8();
     }

     auto stateDataLength = reader.ReadUint32();
     auto stateData = reader.ReadBytes(stateDataLength);

     auto isPlaying = reader.ReadUint8();*/

    printf("AnimationComponentMessages::BehaviorInternalState:\n  componentId = %llu\n  frame = %llu\n  overridesLength = %u\n",
        componentId,
        frame,
        overridesLength
    );
}


void OnBehaviorStateUpdate(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto componentId = reader.ReadUint64();
    auto exceptAgentControllerId = reader.ReadUint32();

    printf("AnimationComponentMessages::BehaviorStateUpdate:\n  frame = %llu\n  componentId = %llu\n  exceptAgentControllerId = %u\n",
        frame,
        componentId,
        exceptAgentControllerId
    );

    return;
    /*
    // Nah not going to touch this one...

    auto floatsLength = 0;
    for (size_t i = 0; i < floatsLength; i++)
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    auto vectorsLength = 0;
    for (size_t i = 0; i < vectorsLength; i++)
    {
        auto internalId = reader.ReadUint16();
        auto value_x = reader.ReadFloat();
        auto value_y = reader.ReadFloat();
        auto value_z = reader.ReadFloat();
    }

    auto quaternionsLength = reader.ReadUint32();
    for (size_t i = 0; i < quaternionsLength; i++)
    {
        auto internalId = reader.ReadUint16();
        auto value_x = reader.ReadFloat();
        auto value_y = reader.ReadFloat();
        auto value_z = reader.ReadFloat();
        auto value_w = reader.ReadFloat();
    }

    auto int8sLength = reader.ReadUint32();
    for (size_t i = 0; i < int8sLength; i++)
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    auto boolsLength = reader.ReadUint32();
    for (size_t i = 0; i < boolsLength; i++)
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    auto internalEventIds = reader.ReadUint32();
    for (size_t i = 0; i < internalEventIds; i++)
    {
        // tbd
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    auto animationAction = reader.ReadUint8();

    auto nodeLocalTimesLength = reader.ReadUint32();
    for (size_t i = 0; i < nodeLocalTimesLength; i++)
    {
        auto nodeId = reader.ReadUint16();
        auto value = reader.ReadUint32();
    }

    auto nodeCropValuesLength = reader.ReadUint32();
    for (size_t i = 0; i < nodeCropValuesLength; i++)
    {
        auto nodeId = reader.ReadUint16();
        auto startValue = reader.ReadFloat();
        auto endValue = reader.ReadFloat();
    }

    */
}

void OnDestroyCluster(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto clusterId = reader.ReadUint32();

    printf("WorldStateMessages::DestroyCluster\n  frame = %llu\n  clusterId = %u\n",
        frame,
        clusterId
    );
}

void OnWarpCharacter(PacketReader &reader)
{
    auto frame = reader.ReadUint64();
    auto agentControllerId = reader.ReadUint32();
    auto position_x = reader.ReadFloat();
    auto position_y = reader.ReadFloat();
    auto position_z = reader.ReadFloat();
    auto rotation_x = reader.ReadFloat();
    auto rotation_y = reader.ReadFloat();
    auto rotation_z = reader.ReadFloat();
    auto rotation_w = reader.ReadFloat();

    printf("AgentControllerMessages::WarpCharacter:\n  frame = %llu\n  agentControllerId = %u\n  position = <%f, %f, %f>\n  rotation = <%f, %f, %f, %f>\n",
        frame,
        agentControllerId,
        position_x,
        position_y,
        position_z,
        rotation_x,
        rotation_y,
        rotation_z,
        rotation_w
    );
}


void ProcessPacketRecv(uint64_t messageId, uint8_t *packet, uint64_t length)
{
    // printf("ProcessPacketRecv: MessageId = %llX | Length = %lld | packet = %llX\n", messageId, length, (uint64_t)packet);

    try
    {
        PacketReader reader;
        reader.Add(0, packet, length);

        messageId = reader.ReadUint32();

        if (messageId == ClientRegionMessages::UserLoginReply)
        {
            OnUserLoginReply(reader);
        }
        else if (messageId == ClientRegionMessages::AddUser)
        {
            OnAddUser(reader);
        }
        else if (messageId == ClientRegionMessages::ChatMessageToClient)
        {
            OnChatMessageToClient(reader);
        }
        else if (messageId == ClientVoiceMessages::LoginReply)
        {
            OnClientVoiceLoginReply(reader);
        }
        else if (messageId == ClientRegionMessages::UserLoginReply)
        {
            OnUserLoginReply(reader);
        }
        else if (messageId == WorldStateMessages::CreateWorld) // 0x685B436C // 1B7FB00 // OK
        {
            OnCreateWorld(reader);
        }
        else if (messageId == WorldStateMessages::InitiateCluster) // 0x349AD257 // 1B7FEF0 // OK
        {
            // OnInitiateCluster(reader);
        }
        else if (messageId == WorldStateMessages::LoadClusterDefinition) // 0xA5C4FB23 // 1B7FE10 // OK
        {
            // OnLoadClusterDefinition(reader);
        }
        else if (messageId == WorldStateMessages::CreateClusterViaDefinition) // 0x73810D53 // 1B7FF60 // OK
        {
            // OnCreateClusterViaDefinition(reader);
        }
        else if (messageId == ClientRegionMessages::SetAgentController) // 0xD6F4CF23 // 1B659D0 // OK
        {
            OnSetAgentController(reader);
        }
        else if (messageId == WorldStateMessages::CreateAgentController) // 1BB6290
        {
            OnCreateAgentController(reader);
        }
        else if (messageId == EditServerMessages::AddUser) // 50155562 // 1984E40 // ok
        {
            OnEditServerAddUser(reader);
        }
        else if (messageId == EditServerMessages::UserLoginReply) //E227C3E2 //1984DD0 // ok
        {
            OnEditServerUserLoginReply(reader);
        }
        else if (messageId == EditServerMessages::WorkspaceReadyReply) // 7D87DBEA //19857A0 // ok
        {
            OnEditServerWorkspaceReadyReply(reader);
        }
        else if (messageId == ClientRegionMessages::InitialChunkSubscribed)
        {
            //printf("InitialChunkSubscribed\n");
        }
        else if (messageId == ClientRegionMessages::CreateRegionBroadcasted)
        {
            //printf("CreateRegionBroadcasted\n");
        }
        else if (messageId == ClientRegionMessages::RemoveUser)
        {
            OnRemoveUser(reader);
        }
        else if (messageId == ClientKafkaMessages::RegionChat)
        {
            OnKafkaRegionChat(reader);
        }
        else if (messageId == RenderMessages::LightStateChanged) // 0x6951DAEC // 1611170
        {
            // OnLightStateChanged(reader); 
        }
        else if (messageId == WorldStateMessages::DestroyAgentController) // 0x16406FB7  // 1BB6300
        {
            OnDestroyAgentController(reader);
        }
        else if (messageId == WorldStateMessages::DestroyCluster) // 0x2926D248  // 1BB5E30
        {
            //OnDestroyCluster(reader);
        }
        else if (messageId == ClientVoiceMessages::LocalAudioStreamState) // 0xF2FB6AD0  // 1DF9E00
        {
            OnLocalAudioStreamState(reader);
        }
        else if (messageId == AgentControllerMessages::PlayAnimation) // 0x009385A0    // 1581210
        {
            OnPlayAnimation(reader);
        }

        // Animation stuff
        else if (messageId == AnimationComponentMessages::FloatVariable)  // 15809C0
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadFloat();
        }
        else if (messageId == AnimationComponentMessages::FloatNodeVariable)  // 1580A30
        {
            auto nodeId = reader.ReadUint16();
            auto value = reader.ReadFloat();
        }
        else if (messageId == AnimationComponentMessages::FloatRangeNodeVariable)  // 1580AA0
        {
            auto nodeId = reader.ReadUint16();
            auto startValue = reader.ReadFloat();
            auto endValue = reader.ReadFloat();
        }
        else if (messageId == AnimationComponentMessages::VectorVariable)  // 1580B10
        {
            auto internalId = reader.ReadUint16();
            auto value_x = reader.ReadFloat();
            auto value_y = reader.ReadFloat();
            auto value_z = reader.ReadFloat();
        }
        else if (messageId == AnimationComponentMessages::QuaternionVariable)  // 1580B80
        {
            auto internalId = reader.ReadUint16();
            auto value_x = reader.ReadFloat();
            auto value_y = reader.ReadFloat();
            auto value_z = reader.ReadFloat();
            auto value_w = reader.ReadFloat();
        }
        else if (messageId == AnimationComponentMessages::Int8Variable)  // 1580BF0
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadUint8();
        }
        else if (messageId == AnimationComponentMessages::BoolVariable)  // 1580C60
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadUint8();
        }
        else if (messageId == AnimationComponentMessages::CharacterTransform) // 1580CD0
        {
            //OnCharacterTransform(reader);
        }
        else if (messageId == AnimationComponentMessages::CharacterTransformPersistent) // 1580D40
        {
            //OnCharacterTransformPersistent(reader);
        }
        else if (messageId == AnimationComponentMessages::CharacterAnimationDestroyed)  // 1580DB0
        {
            auto componentId = reader.ReadUint64();
        }
        else if (messageId == AnimationComponentMessages::AnimationOverride)  // 1580F70
        {
            auto slotIndex = reader.ReadUint8();
        }
        else if (messageId == AnimationComponentMessages::BehaviorInternalState) // 0xCE9B5148  // 1580FE0
        {
            //OnBehaviorInternalState(reader);
        }
        else if (messageId == AnimationComponentMessages::CharacterBehaviorInternalState)  // 1581050
        {
            // No payload?
        }
        else if (messageId == AnimationComponentMessages::BehaviorStateUpdate)  // 0x217192BE  // 15810C0
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto groundComponentId = reader.ReadUint64();
            auto position = reader.ReadBits(0x48);
        }
        else if (messageId == AnimationComponentMessages::BehaviorInitializationData) // 0x7846436E // 1581130
        {
            // OnBehaviorInitializationData(reader);
        }
        else if (messageId == AnimationComponentMessages::CharacterSetPosition)  // 15811A0
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto groundComponentId = reader.ReadUint64();
            auto position = reader.ReadBits(0x48);
        }
        else if (messageId == AnimationComponentMessages::PlayAnimation)  // 1581210
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto resourceId = reader.ReadUUID();

            auto playbackSpeed = reader.ReadBits(0x10);
            //v10 = _mm_cvtpd_ps((__m128d)COERCE_UNSIGNED_INT64((double)((signed int)v16 - 0x7FFF) * 0.00003051850947599719));
            //v10.m128_f32[0] = v10.m128_f32[0] * 10.0;
            //*(__m128 *)(v5 + 48) = _mm_shuffle_ps(v10, v10, 0);

            auto skeletonType = reader.ReadBits(2);
            auto animationType = reader.ReadBits(3);
            auto playbackMode = reader.ReadBits(3);

            printf("AnimationComponentMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
                frame,
                componentId,
                resourceId.c_str()
            );
        }
        else if (messageId == SimulationMessages::InitialTimestamp)  // 15733C0 
        {
            OnInitialTimestamp(reader);
        }
        else if (messageId == SimulationMessages::Timestamp) // 1573430 
        {
            //OnTimestamp(reader);
        }
        else if (messageId == SimulationMessages::SetWorldGravityMagnitude)  // 15734A0
        {
            auto frame = reader.ReadUint64();
            auto magnitude = reader.ReadFloat();
        }
        else if (messageId == SimulationMessages::ActiveRigidBodyUpdate)  // 1573510
        {
            // OnActiveRigidBodyUpdate(reader);
        }
        else if (messageId == SimulationMessages::RigidBodyDeactivated)  // 1573580
        {
            // OnRigidBodyDeactivated(reader);
        }
        else if (messageId == SimulationMessages::RigidBodyPropertyChanged)  // 15735F0
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto propertyData = reader.ReadBytes(16);
            //auto propertyType = reader.ReadBits(5);
        }
        else if (messageId == SimulationMessages::RigidBodyDestroyed)  // 1573660
        {
            auto componentId = reader.ReadUint64();
        }


        // Audio stuff
        else if (messageId == AudioMessages::LoadSound)  // 0x412484C4 // 15B6490
        {
            // OnLoadSound(reader);
        }
        else if (messageId == AudioMessages::PlaySound)  // 0x8FC77316 // 15B6620
        {
            OnPlaySound(reader);
        }
        else if (messageId == AudioMessages::PlayStream)  // 15B6690
        {
            auto streamChannel = reader.ReadUint8();
            auto createPlayHandleId = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto position_x = reader.ReadFloat();
            auto position_y = reader.ReadFloat();
            auto position_z = reader.ReadFloat();
            auto loudness = reader.ReadFloat();
            auto pitch = reader.ReadFloat();
            auto flags = reader.ReadUint8();
        }
        else if (messageId == AudioMessages::StopBroadcastingSound)  // 15B6700
        {
            auto playHandleId = reader.ReadUint64();
        }
        else if (messageId == AudioMessages::SetAudioStream)  // 15B68C0
        {
            auto url = reader.ReadString();
            auto rebroadcast = reader.ReadUint8();
        }
        else if (messageId == AudioMessages::SetMediaSource)  // 15B6930
        {
            auto url = reader.ReadString();
            auto mediaWidth = reader.ReadUint32();
            auto mediaHeight = reader.ReadUint32();
            auto rebroadcast = reader.ReadUint8();
        }
        else if (messageId == AudioMessages::PerformMediaAction)  // 15B69A0
        {
            auto mediaAction = reader.ReadUint32();
            auto rebroadcast = reader.ReadUint8();
        }
        else if (messageId == AudioMessages::StopSound)  // 15B6A10
        {
            auto playHandleId = reader.ReadUint64();
            auto immediate = reader.ReadUint8();
        }
        else if (messageId == AudioMessages::SetLoudness) // 0x20EDD0C4  // 15B6A80
        {
            //OnSetLoudness(reader);
        }
        else if (messageId == AudioMessages::SetPitch) // 0x7BB86A5B  // 15B6AF0
        {
            //OnSetPitch(reader);
        }



        // Agent controller stuff? dono
        else if (messageId == AgentControllerMessages::ControlPoint)  // 170F790
        {
            auto position = reader.ReadBits(0x30);
            auto orientation = reader.ReadBits(0x28);
            auto enabled = reader.ReadBits(1);
            auto controlPointType = reader.ReadBits(4);
        }
        else if (messageId == AgentControllerMessages::WarpCharacter) // 170F800
        {
            //OnWarpCharacter(reader);
        }
        else if (messageId == AgentControllerMessages::RequestWarpCharacter)  // 170F870
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::CharacterControlPointInput)  // 170F8E0
        {
            //OnCharacterControlPointInput(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControlPointInputReliable)  // 170F950
        {
            //OnCharacterControlPointInputReliable(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControllerInput) // 170F9C0
        {
            // OnCharacterControllerInput(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControllerInputReliable) // 170FA30
        {
            // OnCharacterControllerInputReliable(reader);
        }
        else if (messageId == AgentControllerMessages::AgentPlayanimation)  // 170FAA0
        {
            auto agentControllerId = reader.ReadUint32();
        }
        else if (messageId == AgentControllerMessages::RequestAgentPlayAnimation)  // 170FB10
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::RequestBehaviorStateUpdate)  // 170FB80
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::AttachToCharacterNode)  // 170FBF0
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto attachmentOffsetPosition = reader.ReadVectorF(3);
            auto attachmentOffsetOrientation = reader.ReadVectorF(4);
            auto nodeType = reader.ReadUint8();
            auto ownershipWatermark = reader.ReadUint8();
            auto broadcastToSelf = reader.ReadUint8();
        }
        else if (messageId == AgentControllerMessages::DetachFromCharacterNode)  // 170FC60
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto bodyPosition = reader.ReadVectorF(3);
            auto botyOrientation = reader.ReadVectorF(4);
            auto bodyVelocity = reader.ReadVectorF(3);
            auto bodyAngularVelocity = reader.ReadVectorF(3);
            auto nodeType = reader.ReadUint8();
        }
        else if (messageId == AgentControllerMessages::RequestDetachFromCharacterNode)  // 170FCD0
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::SetCharacterNodePhysics)  // 170FD40
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto nodeType = reader.ReadUint8();
            auto collisionsEnabled = reader.ReadUint8();
            auto broadcastToSelf = reader.ReadUint8();
        }
        else if (messageId == AgentControllerMessages::WarpCharacterNode)  // 170FDB0
        {
            auto agentControllerId = reader.ReadUint32();
            auto nodeType = reader.ReadUint32(); /// why is this node type 4bytes?
        }
        else if (messageId == AgentControllerMessages::CharacterIKBone)  // 170FE20
        {
            auto boneIndex = reader.ReadBits(6);
            auto localOrientation = reader.ReadBits(0x28);
        }
        else if (messageId == AgentControllerMessages::CharacterIKPose)  // 170FE90
        {
            //OnCharacterIKPose(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterIKBoneDelta)  // 1710040
        {
            auto boneIndex = reader.ReadBits(6);
            auto localOrientation = reader.ReadBits(0x19);
        }
        else if (messageId == AgentControllerMessages::CharacterIKPoseDelta)  // 17100B0
        {
            //OnCharacterIKPoseDelta(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteraction)  // 1710260
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto objectId = reader.ReadUint32();
            auto targetedPosition = reader.ReadVectorF(3);
            auto targetedNormal = reader.ReadVectorF(3);
            auto origin = reader.ReadVectorF(3);
            auto controlPointType = reader.ReadBits(4);
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionUpdate)  // 17102D0
        {
            OnObjectInteractionUpdate(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionPromptUpdate)  // 1710340
        {
            auto frame = reader.ReadUint64();
            auto clusterId = reader.ReadUint32();
            auto objectId = reader.ReadUint32();
            auto prompt = reader.ReadString();

            printf("AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n",
                frame,
                clusterId,
                objectId,
                prompt
            );
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionCreate)  // 17103B0
        {
            auto frame = reader.ReadUint64();
            auto clusterId = reader.ReadUint32();
            auto objectId = reader.ReadUint32();
            auto prompt = reader.ReadString();
            auto enabled = reader.ReadUint8();

            printf("AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n  enabled = %u\n",
                frame,
                clusterId,
                objectId,
                prompt,
                enabled
            );
        }
        else if (messageId == AgentControllerMessages::RequestSitOnObject)  // 1710420
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::SitOnObject)  // 1710490
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto componentId = reader.ReadUint64();
            auto ownershipWatermark = reader.ReadUint8();
            auto skipAnimation = reader.ReadUint8();

            printf("AgentControllerMessages::SitOnObject\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n  ownershipWatermark = %u\n  skipAnimation = %u\n",
                frame,
                agentControllerId,
                componentId,
                ownershipWatermark,
                skipAnimation
            );
        }
        else if (messageId == AgentControllerMessages::ExitSit)  // 1710500
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto componentId = reader.ReadUint64();
            auto skipAnimation = reader.ReadBits(1);
            auto skipExitTeleport = reader.ReadBits(1);

            printf("AgentControllerMessages::ExitSit\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n",
                frame,
                agentControllerId,
                componentId
            );
        }
        else if (messageId == AgentControllerMessages::SetAgentFiltersBody)  // 1710570
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto componentId = reader.ReadUint64();
            auto filterBody = reader.ReadUint8();

            printf("AgentControllerMessages::SetAgentFiltersBody\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n  filterBody = %u\n",
                frame,
                agentControllerId,
                componentId,
                filterBody
            );
        }
        else if (messageId == AgentControllerMessages::RequestSetAgentFiltersBody)  // 17105E0
        {
            // No palyload
        }
        else if (messageId == AgentControllerMessages::SetCharacterUserProperty)  // 1710650
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto value = reader.ReadFloat();
            auto propertyType = reader.ReadUint8();

            printf("AgentControllerMessages::SetCharacterUserProperty\n  frame = %llu\n  agentControllerId = %u\n  value = %f\n  propertyType = %u\n",
                frame,
                agentControllerId,
                value,
                propertyType
            );
        }
        else if (messageId == AgentControllerMessages::CreateSpeechGraphicsPlayer) // 0x158B2580  // 17106C0
        {
            //OnCreateSpeechGraphicsPlayer(reader);
        }
        else if (messageId == AgentControllerMessages::RequestSpawnItem)  // 1710730
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();
            auto resourceId = reader.ReadUUID();
            auto attachmentNode = reader.ReadUint8();
            auto spawnPosition = reader.ReadBits(0x4E);
            auto spawnOrientation = reader.ReadBits(0x2B);
        }
        else if (messageId == AgentControllerMessages::RequestDeleteLatestSpawn)  // 17107A0
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();

            printf("AgentControllerMessages::RequestDeleteLatestSpawn\n  frame = %llu\n  agentControllerId = %u\n",
                frame,
                agentControllerId
            );
        }
        else if (messageId == AgentControllerMessages::RequestDeleteAllSpawns)  // 1710810
        {
            auto frame = reader.ReadUint64();
            auto agentControllerId = reader.ReadUint32();

            printf("AgentControllerMessages::RequestDeleteAllSpawns\n  frame = %llu\n  agentControllerId = %u\n",
                frame,
                agentControllerId
            );
        }
        else if (messageId == GameWorldMessages::Timestamp)  // 16E9DC0
        {
            auto nanoseconds = reader.ReadUint64();
            auto frame = reader.ReadUint64();
        }
        else if (messageId == GameWorldMessages::MoveEntity) // 0xEFC20B7F  // 16E9E30
        {
            //OnMoveEntity(reader);
        }
        else if (messageId == GameWorldMessages::ChangeMaterialVectorParam)  // 16E9EA0
        {
            auto parameter = reader.ReadUint8();
            auto start = reader.ReadVectorF(3);
            auto end = reader.ReadVectorF(3);
        }
        else if (messageId == GameWorldMessages::ChangeMaterialFloatParam)  // 16E9F10
        {
            auto parameter = reader.ReadUint8();
            auto start = reader.ReadFloat();
            auto end = reader.ReadFloat();
        }
        else if (messageId == GameWorldMessages::ChangeMaterial)  // 16E9F80
        {
            auto startFrame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto materialIndex = reader.ReadUint8();
            auto time = reader.ReadUint32();

            /*
            auto interpMode = reader.ReadBits(4); // ???

            auto vectorParametersLength = reader.ReadUint32();
            for (size_t i = 0; i < vectorParametersLength; i++)
            {
                auto parameter = reader.ReadUint8();
                auto start = reader.ReadVectorF(3);
                auto end = reader.ReadVectorF(3);
            }

            auto floatParametersLength = reader.ReadUint32();
            for (size_t i = 0; i < floatParametersLength; i++)
            {
                auto parameter = reader.ReadUint8();
                auto start = reader.ReadFloat();
                auto end = reader.ReadFloat();
            }
            */
        }
        else if (messageId == GameWorldMessages::StaticMeshFlagsChanged)  // 16E9FF0
        {
            auto componentid = reader.ReadUint64();
            auto frame = reader.ReadUint64();
            auto flags = reader.ReadUint8();
        }
        else if (messageId == GameWorldMessages::StaticMeshScaleChanged)  // 16EA060
        {
            auto componentid = reader.ReadUint64();
            auto frame = reader.ReadUint64();
            auto scale = reader.ReadFloat();
        }
        else if (messageId == GameWorldMessages::RiggedMeshFlagsChange)  // 16EA0D0
        {
            auto componentid = reader.ReadUint64();
            auto frame = reader.ReadUint64();
            auto flags = reader.ReadUint8();
        }
        else if (messageId == GameWorldMessages::RiggedMeshScaleChanged)  // 16EA140
        {
            auto componentid = reader.ReadUint64();
            auto frame = reader.ReadUint64();
            auto scale = reader.ReadFloat();
        }
        else if (messageId == GameWorldMessages::ScriptCameraMessage)  // 16EA1B0
        {
            auto componentid = reader.ReadUint64();
            auto frame = reader.ReadUint64();
            auto controlMode = reader.ReadBits(4);
        }
        else if (messageId == GameWorldMessages::UpdateRuntimeInventorySettings)  // 16EA220
        {
            auto spawnSource = reader.ReadUint8();
            auto spawnLifetimePolicy = reader.ReadUint8();
            auto totalSpawnLimit = reader.ReadUint16();
            auto perUserSpawnLimit = reader.ReadUint16();
            auto spawnTimeout = reader.ReadUint32();
        }


        // KAFKA STUFF
        else if (messageId == ClientKafkaMessages::Login) // 0C0C9D81 // 17A1290 ?
        {
            auto accountId = reader.ReadUUID();
            auto personaId = reader.ReadUUID();
            auto secret = reader.ReadUint32();
            auto inventoryOffset = reader.ReadUint64();

            printf(" ClientKafkaMessages::Login:\n  accountId = %s\n  personaId = %s\n  secret = %u\n  inventoryOffset = %llu\n",
                accountId.c_str(),
                personaId.c_str(),
                secret,
                inventoryOffset
            );
        }
        else if (messageId == ClientKafkaMessages::LoginReply)
        {
            OnClientKafkaMessageLoginReply(reader);
        }
        else if (messageId == ClientKafkaMessages::EnterRegion) // // 17A1370 ? 
        {
            auto regionAddress = reader.ReadString();

            printf("ClientKafkaMessages::EnterRegion '%s'\n", regionAddress.c_str());
        }
        else if (messageId == ClientKafkaMessages::LeaveRegion) // // 17A1520 ? 
        {
            auto regionAddress = reader.ReadString();

            printf("ClientKafkaMessages::LeaveRegion '%s'\n", regionAddress.c_str());
        }
        else if (messageId == ClientKafkaMessages::PrivateChat) // // 17A1740 ? 
        {
            OnPrivateChat(reader);
        }
        else if (messageId == ClientKafkaMessages::PrivateChatStatus) // // 17A17B0 ? 
        {
            OnPrivateChatUpdate(reader);
        }
        else if (messageId == ClientKafkaMessages::PresenceUpdate)
        {
            OnClientKafkaMessagePresenceUpdate(reader);
        }
        else if (messageId == ClientKafkaMessages::FriendRequest) // // 17A1890 ? 
        {
            auto offset = reader.ReadUint64();;
            auto fromPersonaId = reader.ReadUUID();
            auto toPersonaId = reader.ReadUUID();
            auto timestamp = reader.ReadString(); // WHAT
            auto fromSignature = reader.ReadString();

            printf("ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n",
                offset,
                fromPersonaId.c_str(),
                toPersonaId.c_str(),
                timestamp.c_str(),
                fromSignature.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::FriendRequestStatus) // // 17A1900 ? 
        {
            auto offset = reader.ReadUint64();
            auto status = reader.ReadUint32();

            printf("ClientKafkaMessages::FriendRequestStatus:\n  offset = %llu\n  status = %u\n",
                offset,
                status
            );
        }
        else if (messageId == ClientKafkaMessages::FriendResponse) // // 17A1970 ? 
        {
            auto offset = reader.ReadUint64();;
            auto fromPersonaId = reader.ReadUUID();
            auto toPersonaId = reader.ReadUUID();
            auto timestamp = reader.ReadString(); // WHAT
            auto fromSignature = reader.ReadString();
            auto response = reader.ReadUint32();
            auto toSignature = reader.ReadString();

            printf("ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n  response = %u\n  toSignature = %s\n",
                offset,
                fromPersonaId.c_str(),
                toPersonaId.c_str(),
                timestamp.c_str(),
                fromSignature.c_str(),
                response,
                toSignature.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::FriendResponseStatus) // // 17A19E0 ? 
        {
            auto offset = reader.ReadUint64();
            auto status = reader.ReadUint32();

            printf("ClientKafkaMessages::FriendResponseStatus:\n  offset = %llu\n  status = %u\n",
                offset,
                status
            );
        }
        else if (messageId == ClientKafkaMessages::FriendTable) // // 17A1A50 ? 
        {
            auto fromPersonaId = reader.ReadUUID();
            auto toPersonaId = reader.ReadUUID();
            auto status = reader.ReadUint32();

            printf("ClientKafkaMessages::FriendTable:\n  fromPersonaId = %s\n  toPersonaId = %s\n  status = %u\n",
                fromPersonaId.c_str(),
                toPersonaId.c_str(),
                status
            );
        }
        else if (messageId == ClientKafkaMessages::RelationshipOperation) // // 17A1AC0 ? 
        {
            auto other = reader.ReadUUID();
            auto operation = reader.ReadUint32();

            printf("ClientKafkaMessages::RelationshipOperation:\n  other = %s\n  operation = %u\n",
                other.c_str(),
                operation
            );
        }
        else if (messageId == ClientKafkaMessages::RelationshipTable) // 17A1B30
        {
            OnClientKafkaMessageRelationshipTable(reader);
        }
        else if (messageId == ClientKafkaMessages::InventoryItemCapabilities) // 17A1BA0
        {
            auto capabilitiesLength = reader.ReadUint32();
            std::vector<std::string> capabilities(capabilitiesLength);
            for (size_t i = 0; i < capabilitiesLength; i++)
            {
                auto capability = reader.ReadString();
                capabilities.push_back(capability);
            }

            printf("ClientKafkaMessages::InventoryItemCapabilities:\n");
            for(auto &item: capabilities)
            {
                printf("  %s\n", item.c_str());
            }
        }
        else if (messageId == ClientKafkaMessages::InventoryItemRevision) // 17A1D80
        {
            auto asset_id = reader.ReadString();
            auto asset_type = reader.ReadString();
            auto asset_hint = reader.ReadUint32();
            auto thumbnail_asset_id = reader.ReadString();
            auto license_asset_id = reader.ReadString();

            auto capabilitiesLength = reader.ReadUint32();
            auto capabilities = std::vector<std::string>(capabilitiesLength);
            for (size_t i = 0; i < capabilitiesLength; i++)
            {
                auto capability = reader.ReadString();
                capabilities.push_back(capability);
            }

            printf("ClientKafkaMessages::InventoryItemRevision:\n  asset_id = %s\n  asset_type = %s\n  asset_hint = %u\n  thumbnail_asset_id = %s\n  license_asset_id = %s\n  capabilities =",
                asset_id.c_str(),
                asset_type.c_str(),
                asset_hint,
                thumbnail_asset_id.c_str(),
                license_asset_id.c_str()
            );
            for (auto &item : capabilities)
            {
                printf("    %s\n", item.c_str());
            }
        }
        else if (messageId == ClientKafkaMessages::InventoryItemUpdate) // 17A1DF0
        {
            OnInventoryItemUpdate(reader);
        }
        else if (messageId == ClientKafkaMessages::InventoryItemDelete) // 17A1E60
        {
            auto id = reader.ReadString();
            auto offset = reader.ReadUint64();

            printf("ClientKafkaMessages::InventoryItemDelete:\n  id = %s\n  offset = %llu\n",
                id.c_str(),
                offset
            );
        }
        else if (messageId == ClientKafkaMessages::InventoryLoaded) // 17A1ED0
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::FriendRequestLoaded) // 17A2090
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::FriendResponseLoaded) // 17A2250
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::PresenceUpdateFanoutLoaded) // 17A2410
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::FriendTableLoaded) // 17A25D0
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::RelationshipTableLoaded) // 17A2790
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::PrivateChatLoaded) // 17A2950
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::PrivateChatStatusLoaded) // 17A2B10
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::ScriptRegionConsoleLoaded) // 17A2CD0
        {
            auto instanceId = reader.ReadString();
            auto offset = reader.ReadUint64();

            printf("ClientKafkaMessages::ScriptRegionConsoleLoaded:\n  instanceId = %s\n  offset = %llu\n",
                instanceId.c_str(),
                offset
            );
        }
        else if (messageId == ClientKafkaMessages::ClientMetric) // 17A2D40
        {
            auto jsonString = reader.ReadString();

            printf("ClientKafkaMessages::ClientMetric:\n  instanceId = %s\n",
                jsonString.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::RegionHeartbeatMetric) // 17A2EF0
        {
            auto ownerPersonaID = reader.ReadUUID();
            auto averageFrameRate = reader.ReadFloat();
            auto minFrameRate = reader.ReadFloat();
            auto maxFrameRate = reader.ReadFloat();
            auto headcount = reader.ReadUint32();
            auto grid = reader.ReadString();
            auto ownerPersonaHandle = reader.ReadString();
            auto experienceHandle = reader.ReadString();
            auto instanceId = reader.ReadString();
            auto buildID = reader.ReadString();
            auto locationHandle = reader.ReadString();
            auto sansarURI = reader.ReadString();
            auto compatVersion = reader.ReadString();
            auto protoVersion = reader.ReadString();
            auto accessGroup = reader.ReadString();
            auto configuration = reader.ReadString();
            auto worldId = reader.ReadString();

            printf("ClientKafkaMessages::RegionHeartbeatMetric:\n  ownerPersonaID = %s\n  averageFrameRate = %f\n  minFrameRate = %f\n  maxFrameRate = %f\n  headcount = %u\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
                ownerPersonaID.c_str(),
                averageFrameRate,
                minFrameRate,
                maxFrameRate,
                headcount,
                grid.c_str(),
                ownerPersonaHandle.c_str(),
                experienceHandle.c_str(),
                instanceId.c_str(),
                buildID.c_str(),
                locationHandle.c_str(),
                sansarURI.c_str(),
                compatVersion.c_str(),
                protoVersion.c_str(),
                accessGroup.c_str(),
                configuration.c_str(),
                worldId.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::RegionEventMetric) // 17A2F60
        {
            auto ownerPersonaID = reader.ReadUUID();
            auto eventName = reader.ReadString();
            auto headcount = reader.ReadUint32();
            auto grid = reader.ReadString();
            auto ownerPersonaHandle = reader.ReadString();
            auto experienceHandle = reader.ReadString();
            auto instanceId = reader.ReadString();
            auto buildID = reader.ReadString();
            auto locationHandle = reader.ReadString();
            auto sansarURI = reader.ReadString();
            auto compatVersion = reader.ReadString();
            auto protoVersion = reader.ReadString();
            auto accessGroup = reader.ReadString();
            auto configuration = reader.ReadString();
            auto worldId = reader.ReadString();

            printf("ClientKafkaMessages::RegionEventMetric:\n  ownerPersonaID = %s\n  eventName = %s\n  headcount = %u\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
                ownerPersonaID.c_str(),
                eventName.c_str(),
                headcount,
                grid.c_str(),
                ownerPersonaHandle.c_str(),
                experienceHandle.c_str(),
                instanceId.c_str(),
                buildID.c_str(),
                locationHandle.c_str(),
                sansarURI.c_str(),
                compatVersion.c_str(),
                protoVersion.c_str(),
                accessGroup.c_str(),
                configuration.c_str(),
                worldId.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::SubscribeScriptRegionConsole) // 17A2FD0
        {
            auto instanceId = reader.ReadString();
            auto offset = reader.ReadUint64();

            printf("ClientKafkaMessages::SubscribeScriptRegionConsole:\n  instanceId = %s\n  offset = %llu\n",
                instanceId.c_str(),
                offset
            );
        }
        else if (messageId == ClientKafkaMessages::UnsubscriptScriptRegionConsole) // 17A3040
        {
            auto instanceId = reader.ReadString();

            printf("ClientKafkaMessages::UnsubscriptScriptRegionConsole:\n  instanceId = %s\n",
                instanceId.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::ScriptConsoleLog) // 17A31F0
        {
            OnScriptConsoleLog(reader);
        }
        else if (messageId == ClientKafkaMessages::LongLivedNotification) // 17A3260
        {
            auto id = reader.ReadUUID();
            auto type = reader.ReadUint32();
            auto message = reader.ReadString();
            auto timestamp = reader.ReadUint64();

            printf("ClientKafkaMessages::LongLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
                id.c_str(),
                type,
                message.c_str(),
                timestamp
            );
        }
        else if (messageId == ClientKafkaMessages::LongLivedNotificationDelete) // 17A32D0 
        {
            auto id = reader.ReadUUID();

            printf("ClientKafkaMessages::LongLivedNotificationDelete:\n  id = %s\n",
                id.c_str()
            );
        }
        else if (messageId == ClientKafkaMessages::LongLivedNotificationsLoaded) // 17A3460
        {
            auto offset = reader.ReadUint64();
        }
        else if (messageId == ClientKafkaMessages::ShortLivedNotification) // 17A3620
        {
            auto id = reader.ReadUUID();
            auto type = reader.ReadUint32();
            auto message = reader.ReadString();
            auto timestamp = reader.ReadUint64();

            printf("ClientKafkaMessages::ShortLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
                id.c_str(),
                type,
                message.c_str(),
                timestamp
            );
        }


        /*

        else if (messageId ==ClientVoiceMessages::LocalAudioData)  // 0x0D50D087 //
        {
            OnLocalAudioData(reader);
        }

        */

        else
        {
            printf("ProcessPacketRecv: Unhandled message %llX\n", messageId);
        }


        // "Illegally formatted message received. You probably failed to bind a callback for this message. Message was: %s (id: 0x%x)\n"'

        reader.Reset();
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



        auto base = GetBaseAddress();

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
                call RDX
                nop
                nop
                nop
                nop
            */
            uint8_t hijack_ProcessPacketRecv[] = {
                0x48, 0xBA, 0xC1, 0x25, 0x85, 0xAF, 0xF6, 0x7F, 0x00, 0x00, 0xFF, 0xE2,
                0x90, 0x90, 0x90, 0x90
            };

            *((uint64_t *)&hijack_ProcessPacketRecv[2]) = (uint64_t)intercept_ProcessPacketRecv;
            RewriteCode(base + 0x14DAB1A, hijack_ProcessPacketRecv, sizeof(hijack_ProcessPacketRecv));

            // Return point will not be directly after our injected code, but instead follow the existing jmp that we overwrote
            ReturnPoint_ProcessPacketRecv = (uint64_t)(base + 0x14DAED1);
        }


        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        // DetourAttach(&(PVOID &)original_recvfrom, Hooked_Recvfrom); // This is too much to handle. A million different edge cases in packets and ordering. Just going to let the program take care of all the work for us and hook the end calls...
       //  DetourAttach(&(PVOID &)original_recv, Hooked_Recv); // need to hard hook into ssl functions or post-decrypted recv handler...
        DetourAttach(&(PVOID &)original_sendto, Hooked_Sendto);
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


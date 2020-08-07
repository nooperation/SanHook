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

uint16_t last_chat_server_sequence = 0;

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

    auto checksum = Utils::ShortCRC((uint8_t *)buffer, buffer_length - 2, checksumSeed_send);

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

    auto checksum = Utils::ShortCRC((uint8_t *)buffer, buffer_length - 2, seed);
    *((uint16_t *)&buffer[buffer_length - 2]) = checksum;

    printf("------------\n");
    Utils::DumpPacket(buffer, buffer_length, true);
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


    auto checksum = Utils::ShortCRC((uint8_t *)rawData, sizeof(rawData) - 2, checksumSeed_first_cvc100);
    *((uint16_t *)&rawData[sizeof(rawData) - 2]) = checksum;

    printf("*****************\n");
    Utils::DumpPacket((const char *)&rawData[0], sizeof(rawData), true);
    auto result = original_sendto(s, (const char *)&rawData[0], sizeof(rawData), flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1)
    {
        printf("Last error = %d\n", WSAGetLastError());
    }

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

    //Utils::DumpPacket(buff, result, false);


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
                ClientRegion::OnUserLoginReply(reader);
            }
            else if (messageId == ClientRegionMessages::AddUser)
            {
                ClientRegion::OnAddUser(reader);
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

int WINAPI Hooked_Sendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
    if (s == last_chat_sendto_socket)
    {
        last_chat_server_sequence = *((uint16_t *)&buf[1]);
        //printf("** last_chat_server_sequence = %X\n", last_chat_server_sequence);
    }
    //  Utils::DumpPacket(buf, len, true);

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
        Utils::DumpPacket(buf, len, true);
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
        Utils::DumpPacket(buf, len, true);
    }
    else if (messageId == ClientRegionMessages::SubscribeCommand)
    {
        printf("ClientRegionMessages::SubscribeCommand --> \n");
        Utils::DumpPacket(buf, len, true);
    }
    else if (messageId == ClientRegionMessages::RequestDropPortal)
    {
        printf("ClientRegionMessages::RequestDropPortal --> \n");
        Utils::DumpPacket(buf, len, true);
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


            auto myChecksum = Utils::ShortCRC((uint8_t *)buf, len - 2, checksumSeed_recv); // yes this is the correct checksum to use here despite its name...
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
                auto base = Utils::GetBaseAddress();

                // We got this constant from memory.
                // Search for "no-input-source".
                // Between "room scale" and "no-input-source"
                // Right below a call to RotMatrix, enter that call
                // rcx+30 = our pointer
                // Function above it with a bunch of xmm stuff going on (see screenshots)
                //const static auto kCameraPositionOffset = 0x4AAB1C0;
                const static auto kCameraPositionOffset = 0x4AAB1C0;

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
            //Utils::DumpPacket(buf, len, true);
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

            auto myChecksum = Utils::ShortCRC((uint8_t *)buf, len - 2, checksumSeed_send);
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

            //auto myChecksum = Utils::ShortCRC((uint8_t*)buf, len - 2, checksumSeed_recv); // yes this is the correct checksum to use here despite its name...
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
            auto myChecksum = Utils::ShortCRC((uint8_t *)buf, len - 2, checksumSeed_send);

            //printf("Chat: [%d] %s\n", unknownB, text.c_str());
        }
    }

    //printf("Socket: %08X | sendto: %08X | tolen: %d\n", s, to, tolen);
    //printf("^^: GetAddressFromAddr -> %s\n", GetAddressFromAddr(to).c_str());

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





void ProcessPacketRecv(uint64_t messageId, uint8_t *packet, uint64_t length)
{
    // printf("ProcessPacketRecv: MessageId = %llX | Length = %lld | packet = %llX\n", messageId, length, (uint64_t)packet);

    
    std::vector<std::unique_ptr<MessageHandler>> messageHandlers;
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

    try
    {
        PacketReader reader;
        reader.Add(0, packet, length);
        messageId = reader.ReadUint32();

        auto handled_packet = false;

        for (auto &item : messageHandlers)
        {
            if (item->OnMessage(messageId, reader))
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
            RewriteCode(base + 0x14DADAA, hijack_ProcessPacketRecv, sizeof(hijack_ProcessPacketRecv));

            // Return point will not be directly after our injected code, but instead follow the existing jmp that we overwrote
            ReturnPoint_ProcessPacketRecv = (uint64_t)(base + 0x14DB161);
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


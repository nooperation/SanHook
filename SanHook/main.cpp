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

const struct sockaddr* last_chat_sendto;
int last_chat_sendto_length;
int last_chat_sendto_flags;
int last_sent_sequence = 0;
SOCKET last_chat_sendto_socket;

void SendPacket(const char* buff, int len) 
{
    
}

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

int SpecialChat(const std::string& newMessage, int sequence, SOCKET s, int flags, const struct sockaddr* to, int tolen) {
    ++last_sent_sequence;

    auto text_length = newMessage.size();

    uint8_t payloadLength = 0;

    auto buffer_length = 79 + text_length;
    auto buff_ptr = std::make_unique<char[]>(buffer_length);
    auto buffer = buff_ptr.get();

    buffer[0] = 0x07;
    *((uint16_t*)&buffer[1]) = sequence;
    buffer[3] = buffer_length - 6;
    *((uint32_t*)&buffer[4]) = 0x304D3746;
    *((uint64_t*)&buffer[8]) = 0x1c3aad2b02584c90;
    *((uint64_t*)&buffer[16]) = 0xa6040da35a9743f9;

    *((uint64_t*)&buffer[24]) = 0;
    *((uint64_t*)&buffer[32]) = 0;
    *((uint64_t*)&buffer[40]) = 0;

    *((uint32_t*)&buffer[48]) = text_length;
    memcpy(&buffer[52], newMessage.c_str(), text_length);

    *((uint64_t*)&buffer[52 + text_length]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 8]) = 0;
    *((uint64_t*)&buffer[52 + text_length + 16]) = 0;
    *((uint8_t*)&buffer[52 + text_length + 24]) = 0;

    auto checksum = Sansar_ShortCRC((uint8_t*)buffer, buffer_length - 2, checksumSeed_send);
    *((uint16_t*)&buffer[52 + text_length + 25]) = 0xEFFF;// checksum;

    auto result = original_sendto(s, buffer, buffer_length, flags, to, tolen);

    printf("Result = %d\n", result);
    if (result == -1) {
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
    
    for(int i = 0; i < len; ++i)
    {
        auto low = ((uint8_t)buff[i] & 0xF0) >> 4;
        auto high = (uint8_t)buff[i] & 0x0F;

        output_buffer[output_index++] = kHexLookup[low];
        output_buffer[output_index++] = kHexLookup[high];
        output_buffer[output_index++] = ' ';
    }

    if(!output_buffer)
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
std::map<SOCKET, std::string> sock_lookup;

std::string GetAddressFromAddr(const sockaddr *addr)
{
    static std::map<uint64_t, std::string> addr_lookup;
    auto cached = addr_lookup.find((uint64_t)addr);
    if(cached != addr_lookup.end())
    {
        return cached->second;
    }

    int port = 0;
    char address[INET6_ADDRSTRLEN];

    if(addr->sa_family == AF_INET)
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
    addr_lookup[(uint64_t)addr] = result;

    return result;
}

std::string GetAddressFromSocket(SOCKET s)
{
    static std::map<SOCKET, std::string> addr_lookup;
    auto cached = addr_lookup.find(s);
    if(cached != addr_lookup.end())
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




int WINAPI Hooked_Recvfrom(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen)
{
    auto result = original_recvfrom(s, buf, len, flags, from, fromlen);
    if(result != SOCKET_ERROR)
    {
        //printf("hooked recvfrom [%s] -> %d\n", GetAddressFromAddr(from).c_str(), result);
       // DumpPacket(buf, result, false);

        if(buf[0] == 0x07)
        {
            
        }
    }

    return result;
}





int WINAPI Hooked_Sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
{
    if (buf == nullptr || len < 8) {
        return original_sendto(s, buf, len, flags, to, tolen);
    }

    last_sent_sequence = *((uint16_t*)&buf[1]);
    auto messageId = *((uint32_t*)&buf[4]);

    //printf("Hooked sendto [%s] -> %d\n", GetAddressFromAddr(to).c_str(), result);
    if(buf[0] == 0x01)
    {
        auto seed = *(uint32_t *)&buf[5];
        auto seedNameLength = *(uint32_t*)&buf[13];
        auto seedName = std::string((const char*)&buf[17], (std::size_t)seedNameLength);

        if (seedName == "CRC_102") {
            printf("SEND checksum [%s] Seed = %08X\n", seedName.c_str(), seed);
            checksumSeed_send = seed;
        }
        else if (seedName == "CVC_100") {
            printf("RECV checksum [%s] Seed = %08X\n", seedName.c_str(), seed);
            checksumSeed_recv = seed;
        }
        else {
            printf("UNKNOWN checksum [%s] Seed = %08X\n", seedName.c_str(), seed);
        }
    }
    else if(buf[0] == 0x0a)
    {
        //  printf("C -> S: Pong\n");
    }
    else if (buf[0] == 0x07)
    {
        if (messageId == 0x00AC2B81)
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
        else if (messageId == 0x304d3746)
        {
            last_chat_sendto = to;
            last_chat_sendto_length = tolen;
            last_chat_sendto_flags = flags;
            last_chat_sendto_socket = s;

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
            auto unknownE = *((uint8_t*)&payload[44 + textLength + 17]);

            printf("SpecialChat:\n    PersonaID = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n    unknownB = %llu\n    UnknownC = %llu\n    unknownD = %llu\n    unknownE = %d\n    unknownF = %llu\n    unknownG = %llu\n    unknownH = %llu\n    CHAT = %s | ",
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
                unknownE,
                unknownF,
                unknownG,
                unknownH,
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

            if (enabled || text == trigger) {
                
                /*static unsigned char otherPersona[16] = {
                    0x96, 0x45, 0xD3, 0x64, 0xB7, 0x9C, 0x1F, 0xD6, 0x54, 0x83, 0x78, 0xF7,
                    0x8F, 0x42, 0x69, 0xA3
                };*/

                memcpy(personaIdBytes, otherPersona, sizeof(otherPersona) / sizeof(otherPersona[0]));

               // return SpecialChat("XXXX", *((uint16_t*)&buf[1]), s, flags, to, tolen);
            }

            //return;
        }
        else if (messageId == 0xDDDEC199 )
        {
            last_chat_sendto = to;
            last_chat_sendto_length = tolen;
            last_chat_sendto_flags = flags;
            last_chat_sendto_socket = s;

            auto payload = (uint8_t*)&buf[8];

            auto unknownB = *((uint32_t*)&payload[0]);
            auto textLength = *((uint32_t*)&payload[4]);
            auto text = std::string((const char*)&payload[8], (std::size_t)textLength);

            auto checksum = *((uint16_t*)&buf[len - 2]);
            auto myChecksum = Sansar_ShortCRC((uint8_t*)buf, len-2, checksumSeed_send);

            printf("Chat: [%d] %s\n", unknownB, text.c_str());
            printf("Expected checksum: %d | calculated checksum: %d  -> %s", checksum, myChecksum, myChecksum == checksum ? "OK" : "FAIL");

            static bool x = false;
            if (x == true || text == "test") {
                return Chat("XXXX", *((uint16_t*)&buf[1]), s, flags, to, tolen);
            }
        }
    }

    DumpPacket(buf, len, true);

    auto result = original_sendto(s, buf, len, flags, to, tolen);
    return result;
}


int WINAPI Hooked_Recv(SOCKET s, char *buf, int len, int flags)
{
    auto result = original_recv(s, buf, len, flags);
    if(result != SOCKET_ERROR)
    {
        printf("hooked recv [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
      //  DumpPacket(buf, result, false);
    }

    return result;
}


int WINAPI Hooked_Send(SOCKET s, const char *buf, int len, int flags) 
{
    auto result = original_send(s, buf, len, flags);
    if(result != SOCKET_ERROR)
    {
        printf("Hooked send [%s] -> %d\n", GetAddressFromSocket(s).c_str(), result);
        DumpPacket(buf, len, true);
    }

    return result;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if(DetourIsHelperProcess())
    {
        return TRUE;
    }

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            FILE *pFile = nullptr;
            AllocConsole();
            freopen_s(&pFile, "CONOUT$", "w", stdout);

            DetourRestoreAfterWith();

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID &)original_recvfrom, Hooked_Recvfrom);
            //DetourAttach(&(PVOID &)original_recv, Hooked_Recv);
            DetourAttach(&(PVOID &)original_sendto, Hooked_Sendto);
            //DetourAttach(&(PVOID &)original_send, Hooked_Send);
         
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


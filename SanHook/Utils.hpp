#pragma once

#include <string>
#include <cstdio>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

#include <windows.h>

namespace Utils
{
    void DumpPacket(const char *buff, int len, bool is_sending);
    std::string ClusterButt(std::string uuidBytes);
    std::string ToUUID(std::string id);
    uint16_t ShortCRC(uint8_t *packet, int64_t packetLength, uint64_t initialChecksum);


    std::unordered_map<std::string, std::string> _key2NameDb;

    void InitKeyToNameDatabase()
    {
        // needs to be able to be speedy-enough in debug mode.
        // fstreams are the worst thing in the world in debug mode : (

        printf("Reading Key2Name DB...\n");

        FILE *inFile = nullptr;
        fopen_s(&inFile, "r:\\dec\\new_sansar_dec\\personas.csv", "r");
        if (inFile == nullptr || feof(inFile))
        {
            printf("FAILED TO READ KEY2NAME DB\n");
            return;
        }

        fseek(inFile, 0, SEEK_END);
        auto fileLength = ftell(inFile);
        fseek(inFile, 0, SEEK_SET);

        std::vector<char> buffer(fileLength);

        fread(&buffer[0], 1, fileLength, inFile);
        fclose(inFile);

        auto contents = std::string(buffer.begin(), buffer.end());

        std::size_t currentIndex = 0;
        while (currentIndex < fileLength)
        {
            auto commaIndex = contents.find_first_of(',', currentIndex);
            if (commaIndex == std::string::npos)
            {
                break;
            }

            auto lineBreakIndex = contents.find_first_of('\n', currentIndex);
            if (lineBreakIndex == std::string::npos)
            {
                lineBreakIndex = fileLength;
            }

            auto key = contents.substr(currentIndex, commaIndex - currentIndex);
            auto value = contents.substr(commaIndex + 1, lineBreakIndex - commaIndex - 1);

            currentIndex = lineBreakIndex + 1;
            _key2NameDb[key] = value;
        }

        printf("Read %d keys\n", _key2NameDb.size());
    }

    std::string KeyToName(const std::string &key)
    {
        auto entry = _key2NameDb.find(key);
        if (entry == _key2NameDb.end())
        {
            return key;
        }

        return entry->second;
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



    #include <intrin.h>
    uint16_t ShortCRC(uint8_t *packet, int64_t packetLength, uint64_t initialChecksum)
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
    
    #include <tlhelp32.h>
    uint8_t *GetBaseAddress()
    {
        static uint8_t *kBaseAddress = nullptr;

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

}


#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <functional>

class PacketReader
{
private:
    uint8_t *buffer;
    uint64_t bufferSize;
    std::size_t bufferIndex = 0;


public:
    PacketReader()
    {

    }

    PacketReader(uint8_t *packet, uint64_t size) :
        buffer(packet),
        bufferSize(size)
    {
     
    }

    void Dump() const
    {
        static const char kHexLookup[] = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
        };

        char output_buffer[65536 * 3 + 1] = {};
        std::size_t output_index = 0;

        for (int i = 0; i < bufferSize; ++i)
        {
            auto low = ((uint8_t)this->buffer[i] & 0xF0) >> 4;
            auto high = (uint8_t)this->buffer[i] & 0x0F;

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
            printf("DUMP [%zd] %s\n", bufferSize, output_buffer);
        }
    }

    uint8_t *GetBuffer()
    {
        return buffer;
    }
    uint64_t GetBufferSize()
    {
        return bufferSize;
    }
    void Reset()
    {
        bufferIndex = 0;
    }

    void CheckOutOfBoundsRead(std::size_t numBytes) const
    {
        if (bufferIndex + numBytes > bufferSize)
        {
            printf("\n--------------------------\nAttempt to read out of bounds\n--------------------------\n");
            throw std::exception("Attempt to read out of bounds");
        }
    }

    template<typename T>
    T ReadGeneric() {
        const std::size_t numBytes = sizeof(T);

        CheckOutOfBoundsRead(numBytes);

        T result = *((T *)&buffer[bufferIndex]);
        bufferIndex += numBytes;

        return result;
    }

    uint64_t ReadUint64()
    {
        auto result = ReadGeneric<uint64_t>();
        return result;
    }

    uint8_t ReadUint8()
    {
        auto result = ReadGeneric<uint8_t>();
        return result;
    }

    int8_t Readint8()
    {
        auto result = ReadGeneric<uint8_t>();
        return result;
    }

    uint16_t ReadUint16()
    {
        auto result = ReadGeneric<uint16_t>();
        return result;
    }

    int16_t ReadInt16()
    {
        auto result = ReadGeneric<int16_t>();
        return result;
    }

    uint32_t ReadUint32()
    {
        auto result = ReadGeneric<uint32_t>();
        return result;
    }

    int32_t ReadInt32()
    {
        auto result = ReadGeneric<int32_t>();
        return result;
    }

    int64_t ReadInt64()
    {
        auto result = ReadGeneric<int64_t>();
        return result;
    }

    float ReadFloat()
    {
        auto result = ReadGeneric<float>();
        return result;
    }

    std::vector<float> ReadVectorF(uint32_t dimensions)
    {
        std::vector<float> result(dimensions);

        for (size_t i = 0; i < dimensions; i++)
        {
            auto val = ReadFloat();
            result.push_back(val);
        }

        return result;
    }

    std::vector<uint8_t> ReadBits(uint32_t numBits)
    {
        return std::vector<uint8_t>();
    }

    //template<typename T>
    //std::vector<T> ReadList(std::function<T(PacketReader *)> func)
    //{
    //    auto numStrings = ReadUint32();
    //    std::vector<T> result(numStrings);

    //    for (size_t i = 0; i < numStrings; i++)
    //    {
    //        auto item = func();
    //        result.push_back(item);
    //    }

    //    return result;
    //}

    std::vector<std::string> ReadStringList()
    {
        auto numStrings = ReadUint32();
        std::vector<std::string> result(numStrings);

        for (size_t i = 0; i < numStrings; i++)
        {
            auto item = ReadString();
            result.push_back(item);
        }

        return result;
    }

    std::vector<uint8_t> ReadArray()
    {
        auto numBytes = ReadUint32();

        CheckOutOfBoundsRead(numBytes);

        auto result = std::vector<uint8_t>(&buffer[bufferIndex], &buffer[bufferIndex + numBytes]);
        bufferIndex += numBytes;

        return result;
    }

    std::vector<uint8_t> ReadBytes(uint32_t numBytes)
    {
        CheckOutOfBoundsRead(numBytes);

        auto result = std::vector<uint8_t>(&buffer[bufferIndex], &buffer[bufferIndex + numBytes]);
        bufferIndex += numBytes;

        return result;
    }

    std::string ReadString()
    {
        uint32_t length = ReadUint32();

        CheckOutOfBoundsRead(length);

        std::string result = "";
        if (length > 0)
        {
            result = std::string((const char *)&buffer[bufferIndex], length);
            bufferIndex += length;
        }

        return result;
    }

    std::string ReadUUID()
    {
        char uuid[33];
        auto upper = ReadUint64();
        auto lower = ReadUint64();
        sprintf_s(uuid, "%016llx%016llx", upper, lower);

        auto result = std::string(uuid);

        return result;
    }

    void Skip(std::size_t numBytes)
    {
        //printf("Skip %d\n", numBytes);
        CheckOutOfBoundsRead(numBytes);

        bufferIndex += numBytes;
    }

    int64_t GetBytesRemaining() const
    {
        //printf("GetBytesRemaining --> Buffer size = %lld | bufferIndex = %lld\n", bufferSize, bufferIndex);
        return (bufferSize - bufferIndex);
    }

    uint8_t *GetCurrentPointer() const
    {
        //printf("GetBytesRemaining --> Buffer size = %lld | bufferIndex = %lld\n", bufferSize, bufferIndex);
        return &buffer[bufferIndex];
    }

};

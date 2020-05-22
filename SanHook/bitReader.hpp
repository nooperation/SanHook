#pragma once

#include <vector>
#include <iostream>

class BitReader
{
public:
    BitReader()
    {
    }

    BitReader(uint8_t* data, std::size_t length) : data(data), dataLength(length)
    {
    }

    void Reset() {
        this->bitIndex = 0;
        this->byteIndex = 0;
    }

    void Dump()
    {
        printf("DUMP: ");
        for (size_t i = 0; i < dataLength; i++)
        {
            printf("%02X ", data[i]);
        }
        printf("ByteIndex = %d | BitIndex = %d\n", byteIndex, bitIndex);
        printf("\n");
    }

    uint32_t ReadUint32(int numBits)
    {
        uint32_t result = 0;

        for (size_t i = 0; i < numBits; i++)
        {
            auto currentByte = this->data[this->byteIndex];
            auto currentBit = (currentByte >> this->bitIndex) & 1;

            result |= (currentBit << i);

            this->bitIndex++;
            if (bitIndex == 8)
            {
                this->bitIndex = 0;
                this->byteIndex++;
            }
        }

        return result;
    }


    void WriteFloat(float val, uint32_t numbits, bool fuckItAllUp)
    {
        int32_t newVal = packFloat26(val);

        if (fuckItAllUp)
        {
            newVal = (newVal & 0b00000000000000111111111111) |
                ((newVal & 0b00000011111111000000000000) << 6) |
                ((newVal & 0b11111100000000000000000000) >> 8);
        }

        this->WriteInt32(newVal, numbits);
    }

    float ReadFloat(uint32_t numbits, bool fuckItAllUp)
    {
        auto newVal = ReadUInt64(numbits);

        if (fuckItAllUp)
        {
            newVal = (newVal & 0b00000000000000111111111111) |
                ((newVal & 0b11111111000000000000000000) >> 6) |
                ((newVal & 0b00000000111111000000000000) << 8);
        }

        auto result = unpackFloat26(newVal);
        return result;
    }

    float unpackFloat26(int32_t val)
    {
        auto rawValue = (val - 0x1FFFFFF);
        double result = rawValue;

        result *= (2048.0 / 0x1FFFFFF);

        return (float)result;
    }

    int32_t packFloat26(float val)
    {
        double newVal = val * (0x1FFFFFF / 2048.0);  // 16383.99951171875
        newVal = round(newVal);

        auto result = (int32_t)(newVal);
        result += 0x1FFFFFF;

        return result;
    }

    void SkipToEndOfByte() {
        if (this->bitIndex > 0) {
            this->bitIndex = 0;
            this->byteIndex++;
        }
    }

    void WriteInt32(int32_t val, uint32_t numbits)
    {
        if (numbits > 8 * sizeof(val)) {
            throw new std::exception("numbits exceed maximum limit of val");
        }

        uint8_t* srcData = (uint8_t*)&val;

        for (size_t i = 0; i < numbits; i++)
        {
            if (this->byteIndex == this->dataLength) {
                printf("ERROR: Attempted to exceed data buffer length\n");
                return;
            }

            auto srcByte = i / 8;
            auto srcBitMask = i % 8;

            auto srcBit = (srcData[srcByte] >> srcBitMask) & 1;

            this->data[this->byteIndex] &= ~(1 << this->bitIndex); // clear old bit
            this->data[this->byteIndex] |= (srcBit << this->bitIndex); // write new bit

            this->bitIndex++;
            if (bitIndex == 8)
            {
                this->bitIndex = 0;
                this->byteIndex++;
            }
        }
    }

    int64_t ReadInt64(uint64_t numBits)
    {
        uint64_t result = 0;
        uint64_t lastBit = 0;

        for (size_t i = 0; i < numBits; i++)
        {
            uint64_t currentByte = this->data[this->byteIndex];
            uint64_t currentBit = (currentByte >> this->bitIndex) & 1;
            lastBit = currentBit;

            result |= (currentBit << i);

            this->bitIndex++;
            if (bitIndex == 8)
            {
                this->bitIndex = 0;
                this->byteIndex++;
            }
        }

        // sign-extend
        if (lastBit == 1)
        {
            auto numPaddingBits = (64 - numBits);

            auto mask = ((uint64_t)1 << numPaddingBits) - 1;
            mask <<= numBits;
            result |= mask;
        }

        return result;
    }

    uint64_t ReadUInt64(uint64_t numBits)
    {
        uint64_t result = 0;
        uint64_t lastBit = 0;

        for (size_t i = 0; i < numBits; i++)
        {
            uint64_t currentByte = this->data[this->byteIndex];
            uint64_t currentBit = (currentByte >> this->bitIndex) & 1;
            lastBit = currentBit;

            result |= (currentBit << i);

            this->bitIndex++;
            if (bitIndex == 8)
            {
                this->bitIndex = 0;
                this->byteIndex++;
            }
        }

        return result;
    }

private:
    uint8_t* data;
    std::size_t dataLength;
    std::size_t bitIndex = 0;
    std::size_t byteIndex = 0;
};

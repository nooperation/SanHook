#pragma once

#include <vector>
#include <iostream>

class BitReader
{
public:
    BitReader() : data(nullptr), dataLength(0)
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
        printf("ByteIndex = %lld | BitIndex = %lld\n", byteIndex, bitIndex);
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
        int32_t newVal = packFloat26(val, numbits);

        if (fuckItAllUp)
        {
            newVal = (newVal & 0b00000000000000111111111111) |
                ((newVal & 0b00000011111111000000000000) << 6) |
                ((newVal & 0b11111100000000000000000000) >> 8);
        }

        this->WriteInt32(newVal, numbits);
    }

    float ReadFloatX(uint32_t numbits, float modifier)
    {
        auto value = ReadUInt64(numbits);

        if (numbits == 16) {
            value = (
                (value & 0b0000000011111111 << 8) | 
                (value & 0b1111111100000000 >> 8)
            );
        }

        auto mask = pow(2, numbits - 1) - 1;
        auto result = (value - mask) * (1.0f / mask) * modifier;

        return (float)result;
    }
    void WriteFloatX(float value, int numBits, float modifier)
    {
        auto mask = (int32_t)pow(2, numBits - 1) - 1;

        auto newVal = value * (mask / modifier);
        auto packedFloat = (int32_t)round(mask + (value * (mask / modifier)));

        if (numBits == 16) {
            packedFloat = (
                (packedFloat & 0b0000000011111111 << 8) | 
                (packedFloat & 0b1111111100000000 >> 8)
            );
        }
        this->WriteInt32(packedFloat, numBits);
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

        auto result = unpackFloat26(newVal, numbits);
        return result;
    }

    float unpackFloat26(int32_t val, int32_t numbits)
    {
        //auto maxValue = pow(2, numbits - 1) - 1;
        auto maxValue = 0xFFFFFFFFFFFFFFFFul >> (64 - (numbits - 1));
        

        // 0x1FFFFFF = 0b1111111111111111111111111

        auto rawValue = (val - maxValue);
        double result = rawValue;

        result *= (2048.0 / maxValue);

        return (float)result;
    }

    int32_t packFloat26(float val, int32_t numbits)
    {
        auto maxValue = pow(2, numbits - 1) - 1;


        double newVal = val * (maxValue / 2048.0);
        newVal = round(newVal);

        auto result = (int32_t)(newVal);
        result += maxValue;

        return result;
    }

    float unpackFloat24(int32_t val)
    {

        // 0x7FFFFF = 8388607 = 0b11111111111111111111111 
        auto rawValue = (val - 0x7FFFFF);
        double result = rawValue;

        result *= (2048.0 / 0x7FFFFF);

        // exponent = 115 = 2^(115-127) = 0.000244140625
        // mantissa = 1.0

        return (float)result;
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

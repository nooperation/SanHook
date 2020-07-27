#pragma once

#include <cstdint>
#include <string>

class PacketReader
{
private:
	std::vector<uint8_t> buffer;
	std::size_t bufferIndex = 0;

public:
	PacketReader()
	{
		
	}

	PacketReader(const uint8_t *packet, uint32_t size)
	{
		Add(packet, size);
	}

	void Reset() 
	{
		printf("PacketReader::Reset()\n");
		buffer.clear();
		bufferIndex = 0;
	}

	void Add(const uint8_t* packet, uint32_t size)
	{
		buffer.insert(buffer.end(), packet, packet + size);

		/*
		printf("ADD: Buffer dump (size = %d):\n", buffer.size());
		for (auto& item : buffer) {
			printf(" %02X", item);
		}
		printf("\n\n");
		printf("bufferIndex = %d\n", bufferIndex);
		*/
	}

	void CheckOutOfBoundsRead(std::size_t numBytes) const
	{
		if (bufferIndex + numBytes > buffer.size()) {
			printf("\n--------------------------\nAttempt to read out of bounds\n--------------------------\n");
			throw std::exception("Attempt to read out of bounds");
		}


	}

	template<typename T>
	T ReadGeneric() {
		const std::size_t numBytes = sizeof(T);

		CheckOutOfBoundsRead(numBytes);

		T result = *((T*)&buffer[bufferIndex]);
		bufferIndex += numBytes;

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
	uint64_t ReadUint64()
	{
		auto result = ReadGeneric<uint64_t>();
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

	std::string ReadString()
	{
		uint32_t length = ReadUint32();

		CheckOutOfBoundsRead(length);

		std::string result((const char*)&buffer[bufferIndex], length);
		bufferIndex += length;

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

	void Skip(int numBytes)
	{
		//printf("Skip %d\n", numBytes);
		CheckOutOfBoundsRead(numBytes);

		bufferIndex += numBytes;
	}

	int64_t GetBytesRemaining() const 
	{
		//printf("GetBytesRemaining --> Buffer size = %lld | bufferIndex = %lld\n", buffer.size(), bufferIndex);
		return (buffer.size() - bufferIndex);
	}

};

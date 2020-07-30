#pragma once

#include <cstdint>
#include <string>
#include <map>


class PacketReader
{
private:
	std::vector<uint8_t> buffer;
	std::size_t bufferIndex = 0;

	bool previousSequenceIsValid = false;
	uint16_t previousSequence = 0;

	std::map<uint16_t, std::vector<uint8_t> > outOfOrderPackets;

	void ProcessOutOfOrderPackets()
	{
		while(!outOfOrderPackets.empty())
		{
			auto nextPacket = outOfOrderPackets.find(previousSequence + 1);
			if (nextPacket == outOfOrderPackets.end())
			{
				break;
			}

			auto& outOfOrderBuffer = nextPacket->second;
			auto& outOfOrderSequence = nextPacket->first;

			buffer.insert(buffer.end(), outOfOrderBuffer.begin(), outOfOrderBuffer.end());

			previousSequenceIsValid = true;
			previousSequence = outOfOrderSequence;

			//printf("Next packet %d found in out of order packet collection.\n", outOfOrderSequence);

			outOfOrderPackets.erase(nextPacket);
		}
	}

public:
	PacketReader()
	{
		
	}

	PacketReader(uint16_t sequence, const uint8_t *packet, uint32_t size)
	{
		Add(sequence, packet, size);
	}

	uint16_t getPreviousSequence() const {
		return previousSequence;
	}

	void Reset() 
	{
		//printf("PacketReader::Reset()\n");
		buffer.clear();
		bufferIndex = 0;
		previousSequenceIsValid = false;
		previousSequence = 0;

		outOfOrderPackets.clear();
	}

	void Add(uint16_t sequence, const uint8_t* packet, uint32_t size)
	{
		if (previousSequenceIsValid)
		{
			if (sequence != previousSequence + 1)
			{
				//printf("Packet %d found, expected %d. Added it to our out of order packet collection.", sequence, previousSequence + 1);
				outOfOrderPackets[sequence] = std::vector<uint8_t>(packet, packet + size);
				return;
			}
		}

		buffer.insert(buffer.end(), packet, packet + size);

		previousSequenceIsValid = true;
		previousSequence = sequence;
		//printf("Added packet %d to buffer\n", sequence);

		ProcessOutOfOrderPackets();
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

		std::string result = "";
		if (length > 0) {
			result = std::string((const char*)&buffer[bufferIndex], length);
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

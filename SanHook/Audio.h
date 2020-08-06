#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//
//void OnAudioMessageSetPitch(PacketReader &reader);
//void OnAudioMessageSetLoudness(PacketReader &reader);
//void OnAudioMessagePlaySound(PacketReader &reader);
//void OnAudioMessageLoadSound(PacketReader &reader);
//void OnAudioMessagePlayStream(PacketReader &reader);
//void OnAudioMessageStopBroadcastingSound(PacketReader &reader);
//void OnAudioMessageSetAudioStream(PacketReader &reader);
//void OnAudioMessageSetMediaSource(PacketReader &reader);
//void OnAudioMessagePerformMediaAction(PacketReader &reader);
//void OnAudioMessageStopSound(PacketReader &reader);

class Audio
{
public:

    static void OnLoadSound(PacketReader &reader) // 15B6490
    {
        auto resourceId = reader.ReadUUID();

        printf("AudioMessages::LoadSound:\n  resourceId = %s\n",
            resourceId.c_str()
        );
    }

    static void OnPlaySound(PacketReader &reader) // 15B6620
    {
        auto resourceId = reader.ReadUUID();
        auto createPlayHandleId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto position = reader.ReadVectorF(3);
        auto loudness = reader.ReadFloat();
        auto pitch = reader.ReadFloat();
        auto playOffset = reader.ReadUint32();
        auto flags = reader.ReadUint8();

        printf("AudioMessages::PlaySound:\n  resourceId = %s\n  createPlayHandleId = %llu\n  frame = %llu\n  componentId = %llu\n  position = <%f, %f, %f>\n  loudness = %f\n  pitch = %f\n  playOffset = %u\n  flags = %u\n",
            resourceId.c_str(),
            createPlayHandleId,
            frame,
            componentId,
            position[0], position[1], position[2],
            loudness,
            pitch,
            playOffset,
            flags
        );
    }

    static void OnPlayStream(PacketReader &reader) // 15B6690
    {
        auto streamChannel = reader.ReadUint8();
        auto createPlayHandleId = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto position = reader.ReadVectorF(3);
        auto loudness = reader.ReadFloat();
        auto pitch = reader.ReadFloat();
        auto flags = reader.ReadUint8();
    }

    static void OnStopBroadcastingSound(PacketReader &reader) // 15B6700
    {
        auto playHandleId = reader.ReadUint64();
    }

    static void OnSetAudioStream(PacketReader &reader) // 15B68C0
    {
        auto url = reader.ReadString();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnSetMediaSource(PacketReader &reader)  // 15B6930
    {
        auto url = reader.ReadString();
        auto mediaWidth = reader.ReadUint32();
        auto mediaHeight = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnPerformMediaAction(PacketReader &reader)  // 15B69A0
    {
        auto mediaAction = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnStopSound(PacketReader &reader) // 15B6A10
    {
        auto playHandleId = reader.ReadUint64();
        auto immediate = reader.ReadUint8();
    }

    static void OnSetLoudness(PacketReader &reader) // 15B6A80
    {
        auto playHandleId = reader.ReadUint32();
        auto loudness = reader.ReadFloat();

        printf("AudioMessages::SetLoudness:\n  playHandleId = %u\n  loudness = %f\n",
            playHandleId,
            loudness
        );
    }

    static void OnSetPitch(PacketReader &reader) // 15B6AF0
    {
        auto playHandleId = reader.ReadUint32();
        auto pitch = reader.ReadFloat();

        printf("AudioMessages::SetPitch:\n  playHandleId = %u\n  pitch = %f\n",
            playHandleId,
            pitch
        );
    }
};

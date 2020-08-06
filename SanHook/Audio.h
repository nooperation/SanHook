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

    static void OnLoadSound(PacketReader &reader)
    {
        auto resourceId = reader.ReadUUID();

        printf("AudioMessages::LoadSound:\n  resourceId = %s\n",
            resourceId.c_str()
        );
    }


    static void OnPlaySound(PacketReader &reader)
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


    static void OnSetLoudness(PacketReader &reader)
    {
        auto playHandleId = reader.ReadUint32();
        auto loudness = reader.ReadFloat();

        printf("AudioMessages::SetLoudness:\n  playHandleId = %u\n  loudness = %f\n",
            playHandleId,
            loudness
        );
    }

    static void OnSetPitch(PacketReader &reader)
    {
        auto playHandleId = reader.ReadUint32();
        auto pitch = reader.ReadFloat();

        printf("AudioMessages::SetPitch:\n  playHandleId = %u\n  pitch = %f\n",
            playHandleId,
            pitch
        );
    }

    static void OnPlayStream(PacketReader &reader)
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

    static void OnStopBroadcastingSound(PacketReader &reader)
    {

        auto playHandleId = reader.ReadUint64();
    }

    static void OnSetAudioStream(PacketReader &reader)
    {

        auto url = reader.ReadString();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnSetMediaSource(PacketReader &reader)
    {

        auto url = reader.ReadString();
        auto mediaWidth = reader.ReadUint32();
        auto mediaHeight = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnPerformMediaAction(PacketReader &reader)
    {

        auto mediaAction = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    static void OnStopSound(PacketReader &reader)
    {

        auto playHandleId = reader.ReadUint64();
        auto immediate = reader.ReadUint8();
    }
};


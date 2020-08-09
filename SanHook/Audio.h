#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
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

class Audio : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case AudioMessages::LoadSound:  // TAG: 15B6490
            {
                this->OnLoadSound(reader);
                break;
            }
            case AudioMessages::PlaySound:  // TAG: 15B6620
            {
                this->OnPlaySound(reader);
                break;
            }
            case AudioMessages::PlayStream:  // TAG: 15B6690
            {
                this->OnPlayStream(reader);
                break;
            }
            case AudioMessages::StopBroadcastingSound:  // TAG: 15B6700
            {
                this->OnStopBroadcastingSound(reader);
                break;
            }
            case AudioMessages::SetAudioStream:  // TAG: 15B68C0
            {
                this->OnSetAudioStream(reader);
                break;
            }
            case AudioMessages::SetMediaSource:  // TAG: 15B6930
            {
                this->OnSetMediaSource(reader);
                break;
            }
            case AudioMessages::PerformMediaAction:  // TAG: 15B69A0
            {
                this->OnPerformMediaAction(reader);
                break;
            }
            case AudioMessages::StopSound:  // TAG: 15B6A10
            {
                this->OnStopSound(reader);
                break;
            }
            case AudioMessages::SetLoudness: // TAG: 15B6A80
            {
                this->OnSetLoudness(reader);
                break;
            }
            case AudioMessages::SetPitch: // TAG: 15B6AF0
            {
                this->OnSetPitch(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }


    void OnLoadSound(PacketReader &reader) // TAG: 15B6490
    {
        auto resourceId = reader.ReadUUID();

        //printf("[%s] AudioMessages::LoadSound:\n  resourceId = %s\n",
        //_isSender ? "OUT" : "IN",
        //    resourceId.c_str()
        //);
    }

    void OnPlaySound(PacketReader &reader) // TAG: 15B6620
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

        //printf("[%s] AudioMessages::PlaySound:\n  resourceId = %s\n  createPlayHandleId = %llu\n  frame = %llu\n  componentId = %llu\n  position = <%f, %f, %f>\n  loudness = %f\n  pitch = %f\n  playOffset = %u\n  flags = %u\n",
        //_isSender ? "OUT" : "IN",
        //    resourceId.c_str(),
        //    createPlayHandleId,
        //    frame,
        //    componentId,
        //    position[0], position[1], position[2],
        //    loudness,
        //    pitch,
        //    playOffset,
        //    flags
        //);
    }

    void OnPlayStream(PacketReader &reader) // TAG: 15B6690
    {
        auto streamChannel = reader.ReadUint8();
        auto createPlayHandleId = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto position = reader.ReadVectorF(3);
        auto loudness = reader.ReadFloat();
        auto pitch = reader.ReadFloat();
        auto flags = reader.ReadUint8();
    }

    void OnStopBroadcastingSound(PacketReader &reader) // TAG: 15B6700
    {
        auto playHandleId = reader.ReadUint64();
    }

    void OnSetAudioStream(PacketReader &reader) // TAG: 15B68C0
    {
        auto url = reader.ReadString();
        auto rebroadcast = reader.ReadUint8();
    }

    void OnSetMediaSource(PacketReader &reader)  // TAG: 15B6930
    {
        auto url = reader.ReadString();
        auto mediaWidth = reader.ReadUint32();
        auto mediaHeight = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    void OnPerformMediaAction(PacketReader &reader)  // TAG: 15B69A0
    {
        auto mediaAction = reader.ReadUint32();
        auto rebroadcast = reader.ReadUint8();
    }

    void OnStopSound(PacketReader &reader) // TAG: 15B6A10
    {
        auto playHandleId = reader.ReadUint64();
        auto immediate = reader.ReadUint8();
    }

    void OnSetLoudness(PacketReader &reader) // TAG: 15B6A80
    {
        auto playHandleId = reader.ReadUint32();
        auto loudness = reader.ReadUint32();

        //printf("[%s] AudioMessages::SetLoudness:\n  playHandleId = %u\n  loudness = %d\n",
        //_isSender ? "OUT" : "IN",
        //    playHandleId,
        //    loudness
        //);
    }

    void OnSetPitch(PacketReader &reader) // TAG: 15B6AF0
    {
        auto playHandleId = reader.ReadUint32();
        auto pitch = reader.ReadUint32();

        //printf("[%s] AudioMessages::SetPitch:\n  playHandleId = %u\n  pitch = %d\n",
        //_isSender ? "OUT" : "IN",
        //    playHandleId,
        //    pitch
        //);
    }
};

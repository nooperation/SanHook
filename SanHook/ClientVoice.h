#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnClientVoiceLogin(PacketReader &reader);
//void OnClientVoiceLoginReply(PacketReader &reader);
//void OnClientVoiceAudioData(PacketReader &reader);
//void OnClientVoiceSpeechGraphicsData(PacketReader &reader);
//void OnClientVoiceLocalAudioData(PacketReader &reader);
//void OnClientVoiceLocalAudioStreamState(PacketReader &reader);
//void OnClientVoiceLocalAudioPosition(PacketReader &reader);
//void OnClientVoiceLocalAudioMute(PacketReader &reader);
//void OnClientVoiceLocalSetRegionBroadcasted(PacketReader &reader);
//void OnClientVoiceLocalSetMuteAll(PacketReader &reader);
//void OnClientVoiceGroupAudioData(PacketReader &reader);
//void OnClientVoiceLocalTextData(PacketReader &reader);
//void OnClientVoiceMasterInstance(PacketReader &reader);
//void OnClientVoiceVoiceModerationCommand(PacketReader &reader);
//void OnClientVoiceVoiceModerationCommandResponse(PacketReader &reader);
//void OnClientVoiceVoiceNotification(PacketReader &reader);

class ClientVoice 
{
public:
    static void OnLogin(PacketReader &reader)
    {
        auto instance = reader.ReadString();
        auto secret = reader.ReadUint32();
        auto personaId = reader.ReadUUID();
        auto slave = reader.ReadUint8();
    }

    static void OnLoginReply(PacketReader &reader)
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("OnClientVoiceLoginReply: Success=%d: %s\n", success, message.c_str());
    }

    static void OnAudioData(PacketReader &reader)
    {
        auto volume = reader.ReadUint16();
        auto data = reader.ReadArray();
    }

    static void OnSpeechGraphicsData(PacketReader &reader)
    {
        auto data = reader.ReadArray();
    }

    static void OnLocalAudioData(PacketReader &reader)
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto broadcast = reader.ReadUint8();
    }

    static void OnLocalAudioStreamState(PacketReader &reader)
    {
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto broadcast = reader.ReadUint8();
        auto mute = reader.ReadUint8();

        printf("OnClientVoiceLoginReply:\n  instance=%s\n  agentControllerId = %u\n  broacast = %u\n  mute = %u\n",
            instance.c_str(),
            agentControllerId,
            broadcast,
            mute
        );
    }

    static void OnLocalAudioPosition(PacketReader &reader)
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto position = reader.ReadVectorF(3);
        auto agentControllerId = reader.ReadUint32();
    }

    static void OnLocalAudioMute(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();
        auto shouldMute = reader.ReadUint8();
    }

    static void OnLocalSetRegionBroadcasted(PacketReader &reader)
    {
        auto broadcasted = reader.ReadUint8();
    }

    static void OnLocalSetMuteAll(PacketReader &reader)
    {
        auto muteAll = reader.ReadUint8();
    }

    static void OnGroupAudioData(PacketReader &reader)
    {
        auto group = reader.ReadString();
        auto user = reader.ReadString();
    }

    static void OnLocalTextData(PacketReader &reader)
    {
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto data = reader.ReadString();
    }

    static void OnMasterInstance(PacketReader &reader)
    {
        auto instance = reader.ReadUUID();
    }

    static void OnVoiceModerationCommand(PacketReader &reader)
    {
        auto commandLine = reader.ReadString();
    }

    static void OnVoiceModerationCommandResponse(PacketReader &reader)
    {
        auto message = reader.ReadString();
        auto success = reader.ReadUint8();
    }

    static void OnVoiceNotification(PacketReader &reader)
    {
        auto notification = reader.ReadString();
    }
};
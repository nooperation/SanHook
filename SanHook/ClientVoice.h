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
    static void OnLogin(PacketReader &reader)  // 1DF9BD0
    {
        auto instance = reader.ReadString();
        auto secret = reader.ReadUint32();
        auto personaId = reader.ReadUUID();
        auto slave = reader.ReadUint8();
    }

    static void OnLoginReply(PacketReader &reader)  // 1DF9C40
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("OnClientVoiceLoginReply: Success=%d: %s\n", success, message.c_str());
    }

    static void OnAudioData(PacketReader &reader)  // 1DF9CB0
    {
        auto volume = reader.ReadUint16();
        auto data = reader.ReadArray();
    }

    static void OnSpeechGraphicsData(PacketReader &reader) // 1DF9D20
    {
        auto data = reader.ReadArray();
    }

    static void OnLocalAudioData(PacketReader &reader) // 1DF9D90
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto broadcast = reader.ReadUint8();
    }

    static void OnLocalAudioStreamState(PacketReader &reader) // 1DF9E00
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

    static void OnLocalAudioPosition(PacketReader &reader) // 1DF9E70
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto position = reader.ReadVectorF(3);
        auto agentControllerId = reader.ReadUint32();
    }

    static void OnLocalAudioMute(PacketReader &reader) // 1DF9EE0
    {
        auto agentControllerId = reader.ReadUint32();
        auto shouldMute = reader.ReadUint8();
    }

    static void OnLocalSetRegionBroadcasted(PacketReader &reader) // 1DF9F50
    {
        auto broadcasted = reader.ReadUint8();
    }

    static void OnLocalSetMuteAll(PacketReader &reader)  // 1DFA110
    {
        auto muteAll = reader.ReadUint8();
    }

    static void OnGroupAudioData(PacketReader &reader) // 1DFA2D0
    {
        auto group = reader.ReadString();
        auto user = reader.ReadString();
    }

    static void OnLocalTextData(PacketReader &reader)  // 1DFA340
    {
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto data = reader.ReadString();
    }

    static void OnMasterInstance(PacketReader &reader) // 1DFA3B0
    {
        auto instance = reader.ReadUUID();
    }

    static void OnVoiceModerationCommand(PacketReader &reader)  // 1DFA540
    {
        auto commandLine = reader.ReadString();
    }

    static void OnVoiceModerationCommandResponse(PacketReader &reader)  // 1DFA6F0
    {
        auto message = reader.ReadString();
        auto success = reader.ReadUint8();
    }

    static void OnVoiceNotification(PacketReader &reader)  // 1DFA760
    {
        auto notification = reader.ReadString();
    }
};

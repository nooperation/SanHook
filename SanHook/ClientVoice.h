#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
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

class ClientVoice : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case ClientVoiceMessages::Login:  // TAG: 1DF9BD0
            {
                ClientVoice::OnLogin(reader);
                break;
            }
            case ClientVoiceMessages::LoginReply:  // TAG: 1DF9C40
            {
                ClientVoice::OnLoginReply(reader);
                break;
            }
            case ClientVoiceMessages::AudioData:  // TAG: 1DF9CB0
            {
                ClientVoice::OnAudioData(reader);
                break;
            }
            case ClientVoiceMessages::SpeechGraphicsData:  // TAG: 1DF9D20
            {
                ClientVoice::OnSpeechGraphicsData(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioData:  // TAG: 1DF9D90
            {
                ClientVoice::OnLocalAudioData(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioStreamState:  // TAG: 1DF9E00
            {
                ClientVoice::OnLocalAudioStreamState(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioPosition:  // TAG: 1DF9E70
            {
                ClientVoice::OnLocalAudioPosition(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioMute:  // TAG: 1DF9EE0
            {
                ClientVoice::OnLocalAudioMute(reader);
                break;
            }
            case ClientVoiceMessages::LocalSetRegionBroadcasted:  // TAG: 1DF9F50
            {
                ClientVoice::OnLocalSetRegionBroadcasted(reader);
                break;
            }
            case ClientVoiceMessages::LocalSetMuteAll:  // TAG: 1DFA110
            {
                ClientVoice::OnLocalSetMuteAll(reader);
                break;
            }
            case ClientVoiceMessages::GroupAudioData:  // TAG: 1DFA2D0
            {
                ClientVoice::OnGroupAudioData(reader);
                break;
            }
            case ClientVoiceMessages::LocalTextData:  // TAG: 1DFA340
            {
                ClientVoice::OnLocalTextData(reader);
                break;
            }
            case ClientVoiceMessages::MasterInstance:  // TAG: 1DFA3B0
            {
                ClientVoice::OnMasterInstance(reader);
                break;
            }
            case ClientVoiceMessages::VoiceModerationCommand:  // TAG: 1DFA540
            {
                ClientVoice::OnVoiceModerationCommand(reader);
                break;
            }
            case ClientVoiceMessages::VoiceModerationCommandResponse:  // TAG: 1DFA6F0
            {
                ClientVoice::OnVoiceModerationCommandResponse(reader);
                break;
            }
            case ClientVoiceMessages::VoiceNotification:  // TAG: 1DFA760
            {
                ClientVoice::OnVoiceNotification(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnLogin(PacketReader &reader)  // TAG: 1DF9BD0
    {
        auto instance = reader.ReadString();
        auto secret = reader.ReadUint32();
        auto personaId = reader.ReadUUID();
        auto slave = reader.ReadUint8();
    }

    static void OnLoginReply(PacketReader &reader)  // TAG: 1DF9C40
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("OnClientVoiceLoginReply: Success=%d: %s\n", success, message.c_str());
    }

    static void OnAudioData(PacketReader &reader)  // TAG: 1DF9CB0
    {
        auto volume = reader.ReadUint16();
        auto data = reader.ReadArray();
    }

    static void OnSpeechGraphicsData(PacketReader &reader) // TAG: 1DF9D20
    {
        auto data = reader.ReadArray();
    }

    static void OnLocalAudioData(PacketReader &reader) // TAG: 1DF9D90
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto broadcast = reader.ReadUint8();
    }

    static void OnLocalAudioStreamState(PacketReader &reader) // TAG: 1DF9E00
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

    static void OnLocalAudioPosition(PacketReader &reader) // TAG: 1DF9E70
    {
        auto sequence = reader.ReadUint64();
        auto instance = reader.ReadString();
        auto position = reader.ReadVectorF(3);
        auto agentControllerId = reader.ReadUint32();
    }

    static void OnLocalAudioMute(PacketReader &reader) // TAG: 1DF9EE0
    {
        auto agentControllerId = reader.ReadUint32();
        auto shouldMute = reader.ReadUint8();
    }

    static void OnLocalSetRegionBroadcasted(PacketReader &reader) // TAG: 1DF9F50
    {
        auto broadcasted = reader.ReadUint8();
    }

    static void OnLocalSetMuteAll(PacketReader &reader)  // TAG: 1DFA110
    {
        auto muteAll = reader.ReadUint8();
    }

    static void OnGroupAudioData(PacketReader &reader) // TAG: 1DFA2D0
    {
        auto group = reader.ReadString();
        auto user = reader.ReadString();
    }

    static void OnLocalTextData(PacketReader &reader)  // TAG: 1DFA340
    {
        auto instance = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto data = reader.ReadString();
    }

    static void OnMasterInstance(PacketReader &reader) // TAG: 1DFA3B0
    {
        auto instance = reader.ReadUUID();
    }

    static void OnVoiceModerationCommand(PacketReader &reader)  // TAG: 1DFA540
    {
        auto commandLine = reader.ReadString();
    }

    static void OnVoiceModerationCommandResponse(PacketReader &reader)  // TAG: 1DFA6F0
    {
        auto message = reader.ReadString();
        auto success = reader.ReadUint8();
    }

    static void OnVoiceNotification(PacketReader &reader)  // TAG: 1DFA760
    {
        auto notification = reader.ReadString();
    }
};

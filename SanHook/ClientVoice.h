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
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case ClientVoiceMessages::Login:  // TAG: 1DF9BD0
            {
               // this->OnLogin(reader);
                break;
            }
            case ClientVoiceMessages::LoginReply:  // TAG: 1DF9C40
            {
               // this->OnLoginReply(reader);
                break;
            }
            case ClientVoiceMessages::AudioData:  // TAG: 1DF9CB0
            {
                //this->OnAudioData(reader);
                break;
            }
            case ClientVoiceMessages::SpeechGraphicsData:  // TAG: 1DF9D20
            {
                //this->OnSpeechGraphicsData(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioData:  // TAG: 1DF9D90
            {
                this->OnLocalAudioData(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioStreamState:  // TAG: 1DF9E00
            {
               // this->OnLocalAudioStreamState(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioPosition:  // TAG: 1DF9E70
            {
               // this->OnLocalAudioPosition(reader);
                break;
            }
            case ClientVoiceMessages::LocalAudioMute:  // TAG: 1DF9EE0
            {
               // this->OnLocalAudioMute(reader);
                break;
            }
            case ClientVoiceMessages::LocalSetRegionBroadcasted:  // TAG: 1DF9F50
            {
               // this->OnLocalSetRegionBroadcasted(reader);
                break;
            }
            case ClientVoiceMessages::LocalSetMuteAll:  // TAG: 1DFA110
            {
               // this->OnLocalSetMuteAll(reader);
                break;
            }
            case ClientVoiceMessages::GroupAudioData:  // TAG: 1DFA2D0
            {
               // this->OnGroupAudioData(reader);
                break;
            }
            case ClientVoiceMessages::LocalTextData:  // TAG: 1DFA340
            {
               // this->OnLocalTextData(reader);
                break;
            }
            case ClientVoiceMessages::MasterInstance:  // TAG: 1DFA3B0
            {
               // this->OnMasterInstance(reader);
                break;
            }
            case ClientVoiceMessages::VoiceModerationCommand:  // TAG: 1DFA540
            {
                //this->OnVoiceModerationCommand(reader);
                break;
            }
            case ClientVoiceMessages::VoiceModerationCommandResponse:  // TAG: 1DFA6F0
            {
               // this->OnVoiceModerationCommandResponse(reader);
                break;
            }
            case ClientVoiceMessages::VoiceNotification:  // TAG: 1DFA760
            {
               // this->OnVoiceNotification(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnLogin(PacketReader &reader)  // TAG: 1DF9BD0
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - instance changed from String to UUID

        auto instance = reader.ReadUUID();
        auto secret = reader.ReadUint32();
        auto personaId = reader.ReadUUID();
        auto slave = reader.ReadUint8();

        printf("[%s] ClientVoice::Login\n  secret = %u\n  personaId = %s\n  slave = %u\n",
            _isSender ? "OUT" : "IN",
            secret,
            personaId.c_str(),
            slave
        );
    }

    void OnLoginReply(PacketReader &reader)  // TAG: 1DF9C40
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("[%s] OnClientVoiceLoginReply: Success=%u: message='%s'\n",
            _isSender ? "OUT" : "IN",
            success,
            message.c_str()
        );
    }

    void OnAudioData(PacketReader &reader)  // TAG: 1DF9CB0
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - Added 'sequence'

        auto sequence = reader.ReadUint64();
        auto volume = reader.ReadUint16();
        auto data = reader.ReadArray();

        //printf("[%s] OnAudioData: Sequence=%u: Volume='%u'\n",
        //    _isSender ? "OUT" : "IN",
        //    sequence,
        //    volume
        //);
    }

    void OnSpeechGraphicsData(PacketReader &reader) // TAG: 1DF9D20
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - Added 'sequence'

        auto sequence = reader.ReadUint64();
        auto data = reader.ReadArray();
    }

    bool forceBroadcast = false;
    void OnLocalAudioData(PacketReader &reader) // TAG: 1DF9D90
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - Removed 'sequence'
        //    - instance is now a UUID

        auto instance = reader.ReadUUID();
        auto agentControllerId = reader.ReadUint32();

        auto sequence = reader.ReadUint64();
        auto volume = reader.ReadUint16();
        auto data = reader.ReadArray();

        auto sgSequence = reader.ReadUint64();
        auto sgData = reader.ReadArray();

        if (_isSender) {
            auto pBroadcast = (bool *)reader.GetCurrentPointer();
            if (forceBroadcast) {
                *pBroadcast = 1;
            }
        }

        auto broadcast = reader.ReadUint8();

        //printf("[%s] OnLocalAudioData: Inst=%s agent=%u Sequence=%u: Volume='%u' sgSequence=%u broadcast=%d\n",
        //    _isSender ? "OUT" : "IN",
        //    instance,
        //    agentControllerId,
        //    sequence,
        //    volume,
        //    sgSequence,
        //    broadcast
        //);
    }   

    void OnLocalAudioStreamState(PacketReader &reader) // TAG: 1DF9E00
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - instance changed from string to UUID

        auto instance = reader.ReadUUID();
        auto agentControllerId = reader.ReadUint32();
        auto broadcast = reader.ReadUint8();   // TODO: Maybe an okay place to set everyone to broadcast locally, would this work though?
        auto mute = reader.ReadUint8();

        std::ifstream inFile(sanhookConfigBrodcast.string().c_str());
        if (inFile.is_open())
        {
            inFile >> forceBroadcast;
            printf("forceBroadcast=%d\n", forceBroadcast);
            inFile.close();
        }

        //printf("[%s] OnLocalAudioStreamState:\n  instance=%s\n  agentControllerId = %u\n  broacast = %u\n  mute = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    instance.c_str(),
        //    agentControllerId,
        //    broadcast,
        //    mute
        //);
    }

    void OnLocalAudioPosition(PacketReader &reader) // TAG: 1DF9E70
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - instance changed from string to UUID

        auto sequence = reader.ReadUint32();
        auto instance = reader.ReadUUID();
        auto position = reader.ReadVectorF(3);
        auto agentControllerId = reader.ReadUint32();

        //printf("[%s] OnLocalAudioPosition\n  sequence = %u\n position = <%f, %f, %f>\n  agentControllerId = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    sequence,
        //    position[0], position[1], position[2],
        //    agentControllerId
        //);
    }

    void OnLocalAudioMute(PacketReader &reader) // TAG: 1DF9EE0
    {
        auto agentControllerId = reader.ReadUint32();
        auto shouldMute = reader.ReadUint8();
    }

    void OnLocalSetRegionBroadcasted(PacketReader &reader) // TAG: 1DF9F50
    {
        auto broadcasted = reader.ReadUint8();
    }

    void OnLocalSetMuteAll(PacketReader &reader)  // TAG: 1DFA110
    {
        auto muteAll = reader.ReadUint8();
    }

    void OnGroupAudioData(PacketReader &reader) // TAG: 1DFA2D0
    {
        auto group = reader.ReadString();
        auto user = reader.ReadString();

        printf("[%s] OnGroupAudioData\n  group = %s\n  user = %s\n",
            _isSender ? "OUT" : "IN",
            group.c_str(),
            user.c_str()
        );
    }

    void OnLocalTextData(PacketReader &reader)  // TAG: 1DFA340
    {
        // MODIFIED 40.11.0.1810696  (2020-08-13)
        //    - instance changed from string to UUID

        auto instance = reader.ReadUUID();
        auto agentControllerId = reader.ReadUint32();
        auto data = reader.ReadString();

        printf("[%s] OnLocalTextData\n  agentControllerId = %u\n  data = %s\n",
            _isSender ? "OUT" : "IN",
            agentControllerId,
            data.c_str()
        );
    }

    void OnMasterInstance(PacketReader &reader) // TAG: 1DFA3B0
    {
        auto instance = reader.ReadUUID();

        printf("[%s] OnMasterInstance\n  instance = %s\n",
            _isSender ? "OUT" : "IN",
            instance.c_str()
        );
    }

    void OnVoiceModerationCommand(PacketReader &reader)  // TAG: 1DFA540
    {
        auto commandLine = reader.ReadString();

        printf("[%s] OnVoiceModerationCommand\n  commandLine = %s\n",
            _isSender ? "OUT" : "IN",
            commandLine.c_str()
        );
    }

    void OnVoiceModerationCommandResponse(PacketReader &reader)  // TAG: 1DFA6F0
    {
        auto message = reader.ReadString();
        auto success = reader.ReadUint8();

        printf("[%s] OnVoiceNotification\n  message = %s\n  success = %u\n",
            _isSender ? "OUT" : "IN",
            message.c_str(),
            success
        );
    }

    void OnVoiceNotification(PacketReader &reader)  // TAG: 1DFA760
    {
        auto notification = reader.ReadString();

        printf("[%s] OnVoiceNotification\n  notification = %s\n",
            _isSender ? "OUT" : "IN",
            notification.c_str()
        );
    }
};

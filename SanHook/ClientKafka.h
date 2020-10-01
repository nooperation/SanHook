#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnClientKafkaMessageScriptConsoleLog(PacketReader &reader);
//void OnClientKafkaMessagePrivateChatUpdate(PacketReader &reader);
//void OnClientKafkaMessagePrivateChat(PacketReader &reader);
//void OnClientKafkaMessageRegionChat(PacketReader &reader);
//void OnClientKafkaMessageInventoryItemUpdate(PacketReader &reader);
//void OnClientKafkaMessageRelationshipTable(PacketReader &reader);
//void OnClientKafkaMessagePresenceUpdate(PacketReader &reader);
//void OnClientKafkaMessageLoginReply(PacketReader &reader);
//void OnClientKafkaMessageShortLivedNotification(PacketReader &reader);
//void OnClientKafkaMessageLongLivedNotificationsLoaded(PacketReader &reader);
//void OnClientKafkaMessageLongLivedNotificationDelete(PacketReader &reader);
//void OnClientKafkaMessageLongLivedNotification(PacketReader &reader);
//void OnClientKafkaMessageUnsubscribeScriptRegionConsole(PacketReader &reader);
//void OnClientKafkaMessageSubscribeScriptRegionConsole(PacketReader &reader);
//void OnClientKafkaMessageRegionEventMetric(PacketReader &reader);
//void OnClientKafkaMessageRegionHeartbeatMetric(PacketReader &reader);
//void OnClientKafkaMessageClientMetric(PacketReader &reader);
//void OnClientKafkaMessageScriptRegionConsoleLoaded(PacketReader &reader);
//void OnClientKafkaMessagePrivateChatStatusLoaded(PacketReader &reader);
//void OnClientKafkaMessagePrivateChatLoaded(PacketReader &reader);
//void OnClientKafkaMessageRelationshipTableLoaded(PacketReader &reader);
//void OnClientKafkaMessageFriendTableLoaded(PacketReader &reader);
//void OnClientKafkaMessagePresenceUpdateFanoutLoaded(PacketReader &reader);
//void OnClientKafkaMessageFriendResponseLoaded(PacketReader &reader);
//void OnClientKafkaMessageFriendRequestLoaded(PacketReader &reader);
//void OnClientKafkaMessageInventoryLoaded(PacketReader &reader);
//void OnClientKafkaMessageInventoryItemDelete(PacketReader &reader);
//void OnClientKafkaMessageInventoryItemRevision(PacketReader &reader);
//void OnClientKafkaMessageInventoryItemCapabilities(PacketReader &reader);
//void OnClientKafkaMessageRelationshipOperation(PacketReader &reader);
//void OnClientKafkaMessageFriendTable(PacketReader &reader);
//void OnClientKafkaMessageFriendResponseStatus(PacketReader &reader);
//void OnClientKafkaMessageFriendResponse(PacketReader &reader);
//void OnClientKafkaMessageFriendRequestStatus(PacketReader &reader);
//void OnClientKafkaMessageFriendRequest(PacketReader &reader);
//void OnClientKafkaMessageLeaveRegion(PacketReader &reader);
//void OnClientKafkaMessageEnterRegion(PacketReader &reader);
//void OnClientKafkaMessageLogin(PacketReader &reader);

class ClientKafka : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case ClientKafkaMessages::RegionChat: // TAG: 17A16D0
            {
                this->OnRegionChat(reader);
                break;
            }
            case ClientKafkaMessages::Login: // TAG: 17A1290
            {
                this->OnLogin(reader);
                break;
            }
            case ClientKafkaMessages::LoginReply: // TAG: 17A1300
            {
                this->OnLoginReply(reader);
                break;
            }
            case ClientKafkaMessages::EnterRegion: // TAG: 17A1370
            {
                this->OnEnterRegion(reader);
                break;
            }
            case ClientKafkaMessages::LeaveRegion: // TAG: 17A1520
            {
                this->OnLeaveRegion(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChat: // TAG: 17A1740
            {
                this->OnPrivateChat(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatStatus: // TAG: 17A17B0
            {
                this->OnPrivateChatStatus(reader);
                break;
            }
            case ClientKafkaMessages::PresenceUpdate: // TAG: 17A1820
            {
                this->OnPresenceUpdate(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequest: // TAG: 17A1890
            {
                this->OnFriendRequest(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequestStatus: // TAG: 17A1900
            {
                this->OnFriendRequestStatus(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponse: // TAG: 17A1970
            {
                this->OnFriendResponse(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponseStatus: // TAG: 17A19E0
            {
                this->OnFriendResponseStatus(reader);
                break;
            }
            case ClientKafkaMessages::FriendTable: // TAG: 17A1A50
            {
                this->OnFriendTable(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipOperation: // TAG: 17A1AC0
            {
                this->OnRelationshipOperation(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipTable: // TAG: 17A1B30
            {
                this->OnRelationshipTable(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemCapabilities: // TAG: 17A1BA0
            {
                this->OnInventoryItemCapabilities(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemRevision: // TAG: 17A1D80
            {
                this->OnInventoryItemRevision(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemUpdate: // TAG: 17A1DF0
            {
                this->OnInventoryItemUpdate(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemDelete: // TAG: 17A1E60
            {
                this->OnInventoryItemDelete(reader);
                break;
            }
            case ClientKafkaMessages::InventoryLoaded: // TAG: 17A1ED0
            {
                this->OnInventoryLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequestLoaded: // TAG: 17A2090
            {
                this->OnFriendRequestLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponseLoaded: // TAG: 17A2250
            {
                this->OnFriendResponseLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PresenceUpdateFanoutLoaded: // TAG: 17A2410
            {
                this->OnPresenceUpdateFanoutLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendTableLoaded: // TAG: 17A25D0
            {
                this->OnFriendTableLoaded(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipTableLoaded: // TAG: 17A2790
            {
                this->OnRelationshipTableLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatLoaded: // TAG: 17A2950
            {
                this->OnPrivateChatLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatStatusLoaded: // TAG: 17A2B10
            {
                this->OnPrivateChatStatusLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ScriptRegionConsoleLoaded: // TAG: 17A2CD0
            {
                this->OnScriptRegionConsoleLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ClientMetric: // TAG: 17A2D40
            {
                this->OnClientMetric(reader);
                break;
            }
            case ClientKafkaMessages::RegionHeartbeatMetric: // TAG: 17A2EF0
            {
                this->OnRegionHeartbeatMetric(reader);
                break;
            }
            case ClientKafkaMessages::RegionEventMetric: // TAG: 17A2F60
            {
                this->OnRegionEventMetric(reader);
                break;
            }
            case ClientKafkaMessages::SubscribeScriptRegionConsole: // TAG: 17A2FD0
            {
                this->OnSubscribeScriptRegionConsole(reader);
                break;
            }
            case ClientKafkaMessages::UnsubscribeScriptRegionConsole: // TAG: 17A3040
            {
                this->OnUnsubscribeScriptRegionConsole(reader);
                break;
            }
            case ClientKafkaMessages::ScriptConsoleLog: // TAG: 17A31F0
            {
                this->OnScriptConsoleLog(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotification: // TAG: 17A3260
            {
                this->OnLongLivedNotification(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotificationDelete: // TAG: 17A32D0
            {
                this->OnLongLivedNotificationDelete(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotificationsLoaded: // TAG: 17A3460
            {
                this->OnLongLivedNotificationsLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ShortLivedNotification: // TAG: 17A3620
            {
                this->OnShortLivedNotification(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnScriptConsoleLog(PacketReader &reader) // TAG: 17A31F0
    {
        auto logLevel = reader.ReadUint32();
        auto tag = reader.ReadString();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();
        auto scriptId = reader.ReadUint32();
        auto scriptClassName = reader.ReadString();
        auto worldId = reader.ReadString();
        auto instanceId = reader.ReadString();
        auto ownerPersonaId = reader.ReadUUID();
        auto offset = reader.ReadUint64();

        printf("[%s] OnScriptConsoleLog: logLevel = %u\n  tag = %s\n  message = %s\n  timestamp = %llu\n  scriptId = %u\n  scriptClassName = %s\n  worldId = %s\n  instanceId = %s\n  ownerPersonaId = %s\n  offset = %llu\n",
            _isSender ? "OUT" : "IN",
            logLevel,
            tag.c_str(),
            message.c_str(),
            timestamp,
            scriptId,
            scriptClassName.c_str(),
            worldId.c_str(),
            instanceId.c_str(),
            ownerPersonaId.c_str(),
            offset
        );
    }

    void OnLoginReply(PacketReader &reader)  // TAG: 17A1300
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("[%s] ClientKafkaMessages::LoginReply: Success = %u (Message = '%s')\n",
            _isSender ? "OUT" : "IN",
            success, message.c_str()
        );
    }

    void OnPresenceUpdate(PacketReader &reader)  // TAG: 17A1820
    {
        auto personaId = reader.ReadUUID();
        auto present = reader.ReadUint8();
        auto sessionId = reader.ReadUUID();
        auto state = reader.ReadString();
        auto sansarUri = reader.ReadString();

        personaId = Utils::ToUUID(personaId);
        auto handle = Utils::KeyToName(personaId);

        printf("[%s] OnPresenceUpdate:\n  personaId = %s [%s]\n  present = %u\n  sessionId = %s\n  state = %s\n  sansarUri = %s\n",
            _isSender ? "OUT" : "IN",
            handle.c_str(),
            personaId.c_str(),
            present,
            sessionId.c_str(),
            state.c_str(),
            sansarUri.c_str()
        );
    }

    void OnRelationshipTable(PacketReader &reader) // TAG: 17A1B30
    {
        auto other = reader.ReadUUID();
        auto fromSelf = reader.ReadUint8();
        auto fromOther = reader.ReadUint8();
        auto status = reader.ReadUint32();

        printf("[%s] ClientKafkaMessages::RelationshipTable: other = %s, fromSelf = %u, fromOther = %u, status = %u\n",
            _isSender ? "OUT" : "IN",
            other.c_str(),
            fromSelf,
            fromOther,
            status
        );
    }

    void OnInventoryItemUpdate(PacketReader &reader) // TAG: 17A1DF0
    {
        auto id = reader.ReadString();
        auto licensee_label = reader.ReadString();
        auto licensor_label = reader.ReadString();
        auto compat_version = reader.ReadString();
        auto licensor_pid = reader.ReadString();
        auto creationTime = reader.ReadString();
        auto modificationTime = reader.ReadString();

        auto origin = reader.ReadUint32();
        auto originReference = reader.ReadString();
        auto revisionsLength = reader.ReadUint32();

        // TODO: Test ot make sure this is still valid...
        for (size_t i = 0; i < revisionsLength; i++)
        {
            OnInventoryItemRevision(reader);
        }

        auto offset = reader.ReadUint64();
        auto state = reader.ReadUint8();

        printf("[%s] ClientKafkaMessages::InventoryItemUpdate:\n  id = %s\n  licensee_label = %s\n  licensor_label = %s\n  compat_version = %s\n  licensor_pid = %s\n  creationTime = %s\n  modificationTime = %s\n  origin = %u\n  originReference = %s\n  revisionsLength = %u\n  offset = %llu\n  state = %u\n",
            _isSender ? "OUT" : "IN",
            id.c_str(),
            licensee_label.c_str(),
            licensor_label.c_str(),
            compat_version.c_str(),
            licensor_pid.c_str(),
            creationTime.c_str(),
            modificationTime.c_str(),
            origin,
            originReference.c_str(),
            revisionsLength,
            offset,
            state
        );
    }

    std::vector<std::vector<uint8_t>> autochatIds = {
        {0x90, 0x4c, 0x58, 0x02, 0x2b, 0xad, 0x3a, 0x1c, 0xf9, 0x43, 0x97, 0x5a, 0xa3, 0x0d, 0x04, 0xa6}, // Nop
        {0x91, 0x48, 0x57, 0x11, 0x17, 0x90, 0x8a, 0x39, 0x78, 0x63, 0xfb, 0xc5, 0x08, 0x81, 0xcb, 0xaa}, // - sansar studios
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // - System
        {0x5e, 0x46, 0x37, 0xe7, 0xff, 0xa9, 0x63, 0xf5, 0x8d, 0xe1, 0xc5, 0x4c, 0xdd, 0xdf, 0x17, 0x89}, // - wally-sansar
        {0x80, 0x42, 0x0e, 0x76, 0x76, 0xb9, 0xb6, 0x08, 0x90, 0x63, 0xdc, 0x5f, 0x0b, 0xd8, 0xa9, 0x90}, // - Lacie sansar
        {0x80, 0x4c, 0x5e, 0xf8, 0x25, 0xbe, 0x51, 0xdc, 0x33, 0x83, 0x5a, 0x63, 0xa4, 0x4f, 0xce, 0x8b}, // - Binah sansar
        {0x40, 0x48, 0x74, 0xf4, 0x2a, 0x27, 0x2e, 0xbb, 0xe9, 0x49, 0x75, 0x61, 0x0f, 0xd4, 0x21, 0x80}, // - aleks sansar
        {0x5e, 0x46, 0x37, 0xe7, 0xff, 0xa9, 0x63, 0xf5, 0x8d, 0xe1, 0xc5, 0x4c, 0xdd, 0xdf, 0x17, 0x89}, // - wally sansar
        {0xc0, 0x42, 0xcb, 0x98, 0x44, 0xec, 0x53, 0xdc, 0xd9, 0x6e, 0xb2, 0x7e, 0x81, 0xe3, 0x8d, 0x8a}, // - cowboyninja-sansar
        {0x59, 0x4c, 0x11, 0x4f, 0x02, 0xd4, 0x3f, 0x27, 0xb7, 0x76, 0xb9, 0x4f, 0x88, 0xcb, 0xaf, 0xa1}, // - coho sansar
        {0x59, 0x43, 0xbe, 0x2e, 0x5e, 0x11, 0x31, 0xcf, 0x56, 0xa4, 0xa9, 0x84, 0x20, 0xf9, 0x94, 0xa8}, // - seant sansar
        {0x27, 0x47, 0x71, 0xd0, 0xb1, 0xe4, 0x7c, 0x2c, 0xac, 0x33, 0x17, 0xd7, 0x11, 0x7f, 0xc5, 0x91}, // - anne sansar
        {0x4e, 0x45, 0x2d, 0x7d, 0x73, 0xdd, 0xfc, 0x97, 0xac, 0xe2, 0x1c, 0xbf, 0xb7, 0x2e, 0x35, 0xae}, // - grant sansar
        {0x2b, 0x42, 0xf0, 0x70, 0x6a, 0x6d, 0x4c, 0xbe, 0x9e, 0x26, 0x6c, 0xe4, 0x0f, 0x3e, 0xb9, 0x91}, // - sebastian sansar
        {0xef, 0x47, 0x02, 0xb7, 0x3d, 0x42, 0xf4, 0x0e, 0xcd, 0x2a, 0x8e, 0xe9, 0xcb, 0x45, 0xe1, 0xa3}, // - rego sansar
        {0x93, 0x4c, 0xd7, 0x5b, 0x58, 0x89, 0xae, 0x3d, 0x59, 0x0b, 0x6c, 0x81, 0x8a, 0x68, 0x1d, 0xb1}, // - mitchell sansar
        {0xcb, 0x43, 0x0d, 0x59, 0xce, 0xf8, 0x2b, 0xb4, 0xde, 0x88, 0x1c, 0xa1, 0x16, 0x09, 0xea, 0xaa}, // - samd - sansar
        {0x3f, 0x4f, 0x94, 0xc4, 0x86, 0xd4, 0xfe, 0xb4, 0x18, 0x64, 0x8a, 0xa8, 0xda, 0xae, 0x10, 0xa0}, // - ross sansar
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // - Voice System
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // - Unknown User
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // - Default Local User
        {0x16, 0x40, 0x99, 0x7d, 0x9e, 0x62, 0xf1, 0x3f, 0x38, 0x1c, 0x96, 0x1e, 0x98, 0xa2, 0xde, 0x8a}, // - Camera1 sansar (?)
        {0xb1, 0x4c, 0x99, 0xc5, 0x3d, 0xe1, 0x81, 0xe3, 0x88, 0x5c, 0xcb, 0xac, 0x5a, 0x4e, 0x35, 0x88}, // - Camera2 sansar (?)
        {0x77, 0x48, 0xb3, 0xc2, 0xae, 0xa9, 0x9c, 0x9f, 0x89, 0x9e, 0x56, 0x4b, 0xee, 0x27, 0x47, 0xa0}, // - Camera3 sansar (?)
        {0x18, 0x4b, 0x72, 0x33, 0xf2, 0xbf, 0x57, 0xa1, 0x00, 0x2f, 0x50, 0xe3, 0xfa, 0x2e, 0x07, 0x92}, // - Camera4 sansar (?)
    };
    uint32_t autochatId_index = 0;

    void OnRegionChat(PacketReader &reader)  // TAG: 17A16D0
    {
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto instanceAddress = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();
        auto typing = reader.ReadUint8();
        auto offset = reader.ReadUint64();
        auto highwaterMarkOffset = reader.ReadUint64();

        if (_isSender)
        {
            auto buffer = reader.GetBuffer();
            auto fromPersonaIdptr = &buffer[4];

            FILE *inFile = nullptr;
            fopen_s(&inFile, "u:\\sanhook_config.txt", "rb");

            if (inFile != nullptr)
            {
                uint8_t personaId[16] = {};
                bool enabled = fgetc(inFile);
                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                fclose(inFile);

                if (enabled)
                {
                    if (bytesRead == 16)
                    {
                        memcpy(fromPersonaIdptr, personaId, sizeof(personaId) / sizeof(personaId[0]));

                        /*
                        memcpy(fromPersonaIdptr, &autochatIds[autochatId_index][0], sizeof(personaId) / sizeof(personaId[0]));

                        if (message.length() > 0)
                        {
                            autochatId_index++;
                            if (autochatId_index >= autochatIds.size())
                            {
                                autochatId_index = 0;
                            }
                        }*/
                    }
                }
            }
        }

        /*
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto instanceAddress = reader.ReadString();
        auto agentControllerId = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();
        auto typing = reader.ReadUint8();
        auto offset = reader.ReadUint64();
        auto highwaterMarkOffset = reader.ReadUint64();
        */

        auto fromPersonaIdButts = Utils::ClusterButt(fromPersonaId);
        auto fromPersonaIdFormatted = Utils::ToUUID(fromPersonaId);

        auto toPersonaIdButts = Utils::ClusterButt(toPersonaId);
        auto toPersonaIdFormatted = Utils::ToUUID(toPersonaId);

        //if (message.length() > 0)
        //{
        //    printf("[%s] ClientKafkaMessages::RegionChat:\n  From = %s [%s]\n  To = %s [%s]\n  instanceAddress = %s\n  agentControllerId = %d\n  message = %s\n  timestamp = %llu\n  typing = %u\n  offset = %llu\n  highwaterMarkOffset = %llu\n",
        //        _isSender ? "OUT" : "IN",
        //        fromPersonaIdFormatted.c_str(),
        //        fromPersonaIdButts.c_str(),
        //        toPersonaIdFormatted.c_str(),
        //        toPersonaIdButts.c_str(),
        //        instanceAddress.c_str(),
        //        agentControllerId,
        //        message.c_str(),
        //        timestamp,
        //        typing,
        //        offset,
        //        highwaterMarkOffset
        //    );
        //}
    }

    void OnPrivateChat(PacketReader &reader)  // TAG: 17A1740
    {
        if (_isSender)
        {
            auto buffer = reader.GetBuffer();
            auto fromPersonaIdptr = &buffer[12];
            auto toPersonaIdptr = &buffer[28];

            FILE *inFile = nullptr;
            fopen_s(&inFile, "u:\\sanhook_config_im.txt", "rb");

            if (inFile != nullptr)
            {
                uint8_t personaId[16] = {};
                bool enabled = fgetc(inFile);
                auto bytesRead = fread_s(personaId, sizeof(personaId), sizeof(personaId[0]), 16, inFile);
                fclose(inFile);


                //if (bytesRead == 16)
                //{
                //    //memcpy(fromPersonaIdptr, personaId, sizeof(personaId) / sizeof(personaId[0]));

                //    memcpy(fromPersonaIdptr, &autochatIds[autochatId_index][0], sizeof(personaId) / sizeof(personaId[0]));

                //    autochatId_index++;
                //    if (autochatId_index >= autochatIds.size())
                //    {
                //        autochatId_index = 0;
                //    }
                //}
                /*

                if (bytesRead == 16)
                {
                    if (enabled == 1)
                    {
                        memcpy(fromPersonaIdptr, personaId, sizeof(personaId) / sizeof(personaId[0]));
                    }
                    else if (enabled == 2)
                    {
                        memcpy(toPersonaIdptr, personaId, sizeof(personaId) / sizeof(personaId[0]));
                    }
                }*/
            }
        }


        auto offset = reader.ReadUint64();
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        auto fromPersonaIdButts = Utils::ClusterButt(fromPersonaId);
        auto fromPersonaIdFormatted = Utils::ToUUID(fromPersonaId);

        auto toPersonaIdButts = Utils::ClusterButt(toPersonaId);
        auto toPersonaIdFormatted = Utils::ToUUID(toPersonaId);

        printf("[%s] ClientKafkaMessages::PrivateChat:\n  From = %s [%s]\n  To = %s [%s]\n   message = %s\n  timestamp = %llu\n  offset = %llu\n",
            _isSender ? "OUT" : "IN",
            fromPersonaIdFormatted.c_str(),
            fromPersonaIdButts.c_str(),
            toPersonaIdFormatted.c_str(),
            toPersonaIdButts.c_str(),
            message.c_str(),
            timestamp,
            offset
        );
    }

    void OnPrivateChatStatus(PacketReader &reader)  // TAG: 17A17B0
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        //printf("[%s] ClientKafkaMessages::PrivateChatStatus:\n  offset = %llu\n  status = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    offset,
        //    status
        //);
    }

    void OnShortLivedNotification(PacketReader &reader)  // TAG: 17A3620
    {
        auto id = reader.ReadUUID();
        auto type = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        printf("[%s] ClientKafkaMessages::ShortLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
            _isSender ? "OUT" : "IN",
            id.c_str(),
            type,
            message.c_str(),
            timestamp
        );

        std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\shortlived.csv";
        FILE *outFile = nullptr;
        for (size_t i = 0; i < 10; i++)
        {
            fopen_s(&outFile, userdumpPath.string().c_str(), "a");
            if (outFile != nullptr)
            {
                fprintf(outFile, "%s,\n",
                    message.c_str()
                );
                fclose(outFile);

                break;
            }
        }
    }

    void OnLongLivedNotificationsLoaded(PacketReader &reader) // TAG: 17A3460
    {
        auto offset = reader.ReadUint64();
    }

    void OnLongLivedNotificationDelete(PacketReader &reader)  // TAG: 17A32D0
    {
        auto id = reader.ReadUUID();

        printf("[%s] ClientKafkaMessages::LongLivedNotificationDelete:\n  id = %s\n",
            _isSender ? "OUT" : "IN",
            id.c_str()
        );
    }

    void OnLongLivedNotification(PacketReader &reader) // TAG: 17A3260
    {
        auto id = reader.ReadUUID();
        auto type = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        printf("[%s] ClientKafkaMessages::LongLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
            _isSender ? "OUT" : "IN",
            id.c_str(),
            type,
            message.c_str(),
            timestamp
        );

        std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\longlived.csv";
        FILE *outFile = nullptr;
        for (size_t i = 0; i < 10; i++)
        {
            fopen_s(&outFile, userdumpPath.string().c_str(), "a");
            if (outFile != nullptr)
            {
                fprintf(outFile, "%s,\n",
                    message.c_str()
                );
                fclose(outFile);

                break;
            }
        }
    }

    void OnUnsubscribeScriptRegionConsole(PacketReader &reader) // TAG: 17A3040
    {
        auto instanceId = reader.ReadString();

        printf("[%s] ClientKafkaMessages::UnsubscriptScriptRegionConsole:\n  instanceId = %s\n",
            _isSender ? "OUT" : "IN",
            instanceId.c_str()
        );
    }

    void OnSubscribeScriptRegionConsole(PacketReader &reader) // TAG: 17A2FD0
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("[%s] ClientKafkaMessages::SubscribeScriptRegionConsole:\n  instanceId = %s\n  offset = %llu\n",
            _isSender ? "OUT" : "IN",
            instanceId.c_str(),
            offset
        );
    }

    void OnRegionEventMetric(PacketReader &reader) // TAG: 17A2F60
    {
        auto ownerPersonaID = reader.ReadUUID();
        auto eventName = reader.ReadString();
        auto headcount = reader.ReadUint32();
        auto grid = reader.ReadString();
        auto ownerPersonaHandle = reader.ReadString();
        auto experienceHandle = reader.ReadString();
        auto instanceId = reader.ReadString();
        auto buildID = reader.ReadString();
        auto locationHandle = reader.ReadString();
        auto sansarURI = reader.ReadString();
        auto compatVersion = reader.ReadString();
        auto protoVersion = reader.ReadString();
        auto accessGroup = reader.ReadString();
        auto configuration = reader.ReadString();
        auto worldId = reader.ReadString();

        //printf("ClientKafkaMessages::RegionEventMetric:\n  ownerPersonaID = %s\n  eventName = %s\n  headcount = %u\n  grid = %s\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
        //    ownerPersonaID.c_str(),
        //    eventName.c_str(),
        //    headcount,
        //    grid.c_str(),
        //    ownerPersonaHandle.c_str(),
        //    experienceHandle.c_str(),
        //    instanceId.c_str(),
        //    buildID.c_str(),
        //    locationHandle.c_str(),
        //    sansarURI.c_str(),
        //    compatVersion.c_str(),
        //    protoVersion.c_str(),
        //    accessGroup.c_str(),
        //    configuration.c_str(),
        //    worldId.c_str()
        //);
    }

    void OnRegionHeartbeatMetric(PacketReader &reader) // TAG: 17A2EF0
    {
        auto ownerPersonaID = reader.ReadUUID();
        auto averageFrameRate = reader.ReadFloat();
        auto minFrameRate = reader.ReadFloat();
        auto maxFrameRate = reader.ReadFloat();
        auto headcount = reader.ReadUint32();
        auto grid = reader.ReadString();
        auto ownerPersonaHandle = reader.ReadString();
        auto experienceHandle = reader.ReadString();
        auto instanceId = reader.ReadString();
        auto buildID = reader.ReadString();
        auto locationHandle = reader.ReadString();
        auto sansarURI = reader.ReadString();
        auto compatVersion = reader.ReadString();
        auto protoVersion = reader.ReadString();
        auto accessGroup = reader.ReadString();
        auto configuration = reader.ReadString();
        auto worldId = reader.ReadString();

        //printf("ClientKafkaMessages::RegionHeartbeatMetric:\n  ownerPersonaID = %s\n  averageFrameRate = %f\n  minFrameRate = %f\n  maxFrameRate = %f\n  headcount = %u\n  grid = %s\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
        //    ownerPersonaID.c_str(),
        //    averageFrameRate,
        //    minFrameRate,
        //    maxFrameRate,
        //    headcount,
        //    grid.c_str(),
        //    ownerPersonaHandle.c_str(),
        //    experienceHandle.c_str(),
        //    instanceId.c_str(),
        //    buildID.c_str(),
        //    locationHandle.c_str(),
        //    sansarURI.c_str(),
        //    compatVersion.c_str(),
        //    protoVersion.c_str(),
        //    accessGroup.c_str(),
        //    configuration.c_str(),
        //    worldId.c_str()
        //);
    }

    void OnClientMetric(PacketReader &reader) // TAG: 17A2D40
    {
        auto jsonString = reader.ReadString();

        //printf("ClientKafkaMessages::ClientMetric:\n  jsonString = %s\n",
        //    jsonString.c_str()
        //);
    }

    void OnScriptRegionConsoleLoaded(PacketReader &reader) // TAG: 17A2CD0
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("[%s] ClientKafkaMessages::ScriptRegionConsoleLoaded:\n  instanceId = %s\n  offset = %llu\n",
            _isSender ? "OUT" : "IN",
            instanceId.c_str(),
            offset
        );
    }

    void OnPrivateChatStatusLoaded(PacketReader &reader) // TAG: 17A2B10
    {
        auto offset = reader.ReadUint64();
    }

    void OnPrivateChatLoaded(PacketReader &reader) // TAG: 17A2950
    {
        auto offset = reader.ReadUint64();
    }

    void OnRelationshipTableLoaded(PacketReader &reader) // TAG: 17A2790
    {
        auto offset = reader.ReadUint64();
    }

    void OnFriendTableLoaded(PacketReader &reader) // TAG: 17A25D0
    {
        auto offset = reader.ReadUint64();
    }

    void OnPresenceUpdateFanoutLoaded(PacketReader &reader) // TAG: 17A2410
    {
        auto offset = reader.ReadUint64();
    }

    void OnFriendResponseLoaded(PacketReader &reader) // TAG: 17A2250
    {
        auto offset = reader.ReadUint64();
    }

    void OnFriendRequestLoaded(PacketReader &reader) // TAG: 17A2090
    {
        auto offset = reader.ReadUint64();
    }

    void OnInventoryLoaded(PacketReader &reader) // TAG: 17A1ED0
    {
        auto offset = reader.ReadUint64();
    }

    void OnInventoryItemDelete(PacketReader &reader) // TAG: 17A1E60
    {
        auto id = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("[%s] ClientKafkaMessages::InventoryItemDelete:\n  id = %s\n  offset = %llu\n",
            _isSender ? "OUT" : "IN",
            id.c_str(),
            offset
        );
    }

    void OnInventoryItemRevision(PacketReader &reader) // TAG: 17A1D80
    {
        auto asset_id = reader.ReadString();
        auto asset_type = reader.ReadString();
        auto asset_hint = reader.ReadUint32();
        auto thumbnail_asset_id = reader.ReadString();
        auto license_asset_id = reader.ReadString();
        auto capabilities = reader.ReadStringList();

        printf("[%s] ClientKafkaMessages::InventoryItemRevision:\n  asset_id = %s\n  asset_type = %s\n  asset_hint = %u\n  thumbnail_asset_id = %s\n  license_asset_id = %s\n  capabilities =",
            _isSender ? "OUT" : "IN",
            asset_id.c_str(),
            asset_type.c_str(),
            asset_hint,
            thumbnail_asset_id.c_str(),
            license_asset_id.c_str()
        );
        for (auto &item : capabilities)
        {
            printf("    %s\n", item.c_str());
        }
    }

    void OnInventoryItemCapabilities(PacketReader &reader)  // TAG: 17A1BA0
    {
        auto capabilities = reader.ReadStringList();

        printf("[%s] ClientKafkaMessages::InventoryItemCapabilities:\n",
            _isSender ? "OUT" : "IN"
        );
        for (auto &item : capabilities)
        {
            printf("  %s\n", item.c_str());
        }
    }

    void OnRelationshipOperation(PacketReader &reader)  // TAG: 17A1AC0
    {
        auto other = reader.ReadUUID();
        auto operation = reader.ReadUint32();

        // 0 = friend request / friend accept
        // 1 = ignore / remove friend
        // 2 = block
        // 3 = unblock

        printf("[%s] ClientKafkaMessages::RelationshipOperation:\n  other = %s\n  operation = %u\n",
            _isSender ? "OUT" : "IN",
            other.c_str(),
            operation
        );
    }

    void OnFriendTable(PacketReader &reader)  // TAG: 17A1A50
    {
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto status = reader.ReadUint32();

        printf("[%s] ClientKafkaMessages::FriendTable:\n  fromPersonaId = %s\n  toPersonaId = %s\n  status = %u\n",
            _isSender ? "OUT" : "IN",
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            status
        );
    }

    void OnFriendResponseStatus(PacketReader &reader)  // TAG: 17A19E0
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("[%s] ClientKafkaMessages::FriendResponseStatus:\n  offset = %llu\n  status = %u\n",
            _isSender ? "OUT" : "IN",
            offset,
            status
        );
    }

    void OnFriendResponse(PacketReader &reader)  // TAG: 17A1970
    {
        auto offset = reader.ReadUint64();;
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto timestamp = reader.ReadString(); // WHAT
        auto fromSignature = reader.ReadString();
        auto response = reader.ReadUint32();
        auto toSignature = reader.ReadString();

        printf("[%s] ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n  response = %u\n  toSignature = %s\n",
            _isSender ? "OUT" : "IN",
            offset,
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            timestamp.c_str(),
            fromSignature.c_str(),
            response,
            toSignature.c_str()
        );
    }

    void OnFriendRequestStatus(PacketReader &reader)  // TAG: 17A1900
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("[%s] ClientKafkaMessages::FriendRequestStatus:\n  offset = %llu\n  status = %u\n",
            _isSender ? "OUT" : "IN",
            offset,
            status
        );
    }

    void OnFriendRequest(PacketReader &reader)  // TAG: 17A1890
    {
        auto offset = reader.ReadUint64();;
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto timestamp = reader.ReadString(); // WHAT
        auto fromSignature = reader.ReadString();

        printf("[%s] ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n",
            _isSender ? "OUT" : "IN",
            offset,
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            timestamp.c_str(),
            fromSignature.c_str()
        );
    }

    void OnLeaveRegion(PacketReader &reader)  // TAG: 17A1520
    {
        auto regionAddress = reader.ReadString();

        printf("[%s] ClientKafkaMessages::LeaveRegion '%s'\n",
            _isSender ? "OUT" : "IN",
            regionAddress.c_str()
        );
    }

    void OnEnterRegion(PacketReader &reader)  // TAG: 17A1370
    {
        auto regionAddress = reader.ReadString();

        printf("[%s] ClientKafkaMessages::EnterRegion '%s'\n",
            _isSender ? "OUT" : "IN",
            regionAddress.c_str()
        );
    }

    void OnLogin(PacketReader &reader)  // TAG: 17A1290
    {
        auto accountId = reader.ReadUUID();
        auto personaId = reader.ReadUUID();
        auto secret = reader.ReadUint32();
        auto inventoryOffset = reader.ReadUint64();

        printf("[%s] ClientKafka::Login:\n  accountId = %s\n  personaId = %s\n  secret = %u\n  inventoryOffset = %llu\n",
            _isSender ? "OUT" : "IN",
            accountId.c_str(),
            personaId.c_str(),
            secret,
            inventoryOffset
        );
    }
};

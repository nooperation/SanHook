#pragma once

#include <cstdio>
#include <string>
#include <vector>

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
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case ClientKafkaMessages::RegionChat: // TAG: 17A16D0
            {
                ClientKafka::OnRegionChat(reader);
                break;
            }
            case ClientKafkaMessages::Login: // TAG: 17A1290
            {
                ClientKafka::OnLogin(reader);
                break;
            }
            case ClientKafkaMessages::LoginReply: // TAG: 17A1300
            {
                ClientKafka::OnLoginReply(reader);
                break;
            }
            case ClientKafkaMessages::EnterRegion: // TAG: 17A1370
            {
                ClientKafka::OnEnterRegion(reader);
                break;
            }
            case ClientKafkaMessages::LeaveRegion: // TAG: 17A1520
            {
                ClientKafka::OnLeaveRegion(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChat: // TAG: 17A1740
            {
                ClientKafka::OnPrivateChat(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatStatus: // TAG: 17A17B0
            {
                ClientKafka::OnPrivateChatUpdate(reader);
                break;
            }
            case ClientKafkaMessages::PresenceUpdate: // TAG: 17A1820
            {
                ClientKafka::OnPresenceUpdate(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequest: // TAG: 17A1890
            {
                ClientKafka::OnFriendRequest(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequestStatus: // TAG: 17A1900
            {
                ClientKafka::OnFriendRequestStatus(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponse: // TAG: 17A1970
            {
                ClientKafka::OnFriendResponse(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponseStatus: // TAG: 17A19E0
            {
                ClientKafka::OnFriendResponseStatus(reader);
                break;
            }
            case ClientKafkaMessages::FriendTable: // TAG: 17A1A50
            {
                ClientKafka::OnFriendTable(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipOperation: // TAG: 17A1AC0
            {
                ClientKafka::OnRelationshipOperation(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipTable: // TAG: 17A1B30
            {
                ClientKafka::OnRelationshipTable(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemCapabilities: // TAG: 17A1BA0
            {
                ClientKafka::OnInventoryItemCapabilities(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemRevision: // TAG: 17A1D80
            {
                ClientKafka::OnInventoryItemRevision(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemUpdate: // TAG: 17A1DF0
            {
                ClientKafka::OnInventoryItemUpdate(reader);
                break;
            }
            case ClientKafkaMessages::InventoryItemDelete: // TAG: 17A1E60
            {
                ClientKafka::OnInventoryItemDelete(reader);
                break;
            }
            case ClientKafkaMessages::InventoryLoaded: // TAG: 17A1ED0
            {
                ClientKafka::OnInventoryLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendRequestLoaded: // TAG: 17A2090
            {
                ClientKafka::OnFriendRequestLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendResponseLoaded: // TAG: 17A2250
            {
                ClientKafka::OnFriendResponseLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PresenceUpdateFanoutLoaded: // TAG: 17A2410
            {
                ClientKafka::OnPresenceUpdateFanoutLoaded(reader);
                break;
            }
            case ClientKafkaMessages::FriendTableLoaded: // TAG: 17A25D0
            {
                ClientKafka::OnFriendTableLoaded(reader);
                break;
            }
            case ClientKafkaMessages::RelationshipTableLoaded: // TAG: 17A2790
            {
                ClientKafka::OnRelationshipTableLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatLoaded: // TAG: 17A2950
            {
                ClientKafka::OnPrivateChatLoaded(reader);
                break;
            }
            case ClientKafkaMessages::PrivateChatStatusLoaded: // TAG: 17A2B10
            {
                ClientKafka::OnPrivateChatStatusLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ScriptRegionConsoleLoaded: // TAG: 17A2CD0
            {
                ClientKafka::OnScriptRegionConsoleLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ClientMetric: // TAG: 17A2D40
            {
                ClientKafka::OnClientMetric(reader);
                break;
            }
            case ClientKafkaMessages::RegionHeartbeatMetric: // TAG: 17A2EF0
            {
                ClientKafka::OnRegionHeartbeatMetric(reader);
                break;
            }
            case ClientKafkaMessages::RegionEventMetric: // TAG: 17A2F60
            {
                ClientKafka::OnRegionEventMetric(reader);
                break;
            }
            case ClientKafkaMessages::SubscribeScriptRegionConsole: // TAG: 17A2FD0
            {
                ClientKafka::OnSubscribeScriptRegionConsole(reader);
                break;
            }
            case ClientKafkaMessages::UnsubscribeScriptRegionConsole: // TAG: 17A3040
            {
                ClientKafka::OnUnsubscribeScriptRegionConsole(reader);
                break;
            }
            case ClientKafkaMessages::ScriptConsoleLog: // TAG: 17A31F0
            {
                ClientKafka::OnScriptConsoleLog(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotification: // TAG: 17A3260
            {
                ClientKafka::OnLongLivedNotification(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotificationDelete: // TAG: 17A32D0
            {
                ClientKafka::OnLongLivedNotificationDelete(reader);
                break;
            }
            case ClientKafkaMessages::LongLivedNotificationsLoaded: // TAG: 17A3460
            {
                ClientKafka::OnLongLivedNotificationsLoaded(reader);
                break;
            }
            case ClientKafkaMessages::ShortLivedNotification: // TAG: 17A3620
            {
                ClientKafka::OnShortLivedNotification(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnScriptConsoleLog(PacketReader &reader) // TAG: 17A31F0
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

        printf("OnScriptConsoleLog: logLevel = %u\n  tag = %s\n  message = %s\n  timestamp = %llu\n  scriptId = %u\n  scriptClassName = %s\n  worldId = %s\n  instanceId = %s\n  ownerPersonaId = %s\n  offset = %llu\n",
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

    static void OnLoginReply(PacketReader &reader)  // TAG: 17A1300
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("ClientKafkaMessages::LoginReply: Success = %u (Message = '%s')\n", success, message.c_str());
    }

    static void OnPresenceUpdate(PacketReader &reader)  // TAG: 17A1820
    {
        auto personaId = reader.ReadUUID();
        auto present = reader.ReadUint8();
        auto sessionId = reader.ReadUUID();
        auto state = reader.ReadString();
        auto sansarUri = reader.ReadString();

        printf("ClientKafkaMessages::PresenceUpdate:\n  personaId = %s\n  present = %u\n  sessionId = %s\n  state = %s\n  sansarUri = %s\n",
            personaId.c_str(),
            present,
            sessionId.c_str(),
            state.c_str(),
            sansarUri.c_str()
        );
    }

    static void OnRelationshipTable(PacketReader &reader) // TAG: 17A1B30
    {
        auto other = reader.ReadUUID();
        auto fromSelf = reader.ReadUint8();
        auto fromOther = reader.ReadUint8();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::RelationshipTable: other = %s, fromSelf = %u, fromOther = %u, status = %u\n",
            other.c_str(),
            fromSelf,
            fromOther,
            status
        );
    }

    static void OnInventoryItemUpdate(PacketReader &reader) // TAG: 17A1DF0
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

        auto offset = reader.ReadUint64();
        auto state = reader.ReadUint8();

        printf("ClientKafkaMessages::InventoryItemUpdate:\n  id = %s\n  licensee_label = %s\n  licensor_label = %s\n  compat_version = %s\n  licensor_pid = %s\n  creationTime = %s\n  modificationTime = %s\n  origin = %u\n  originReference = %s\n  revisionsLength = %u\n  offset = %llu\n  state = %u\n",
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

    static void OnRegionChat(PacketReader &reader)  // TAG: 17A16D0
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

        auto fromPersonaIdButts = Utils::ClusterButt(fromPersonaId);
        auto fromPersonaIdFormatted = Utils::ToUUID(fromPersonaId);

        auto toPersonaIdButts = Utils::ClusterButt(toPersonaId);
        auto toPersonaIdFormatted = Utils::ToUUID(toPersonaId);

        printf("ClientKafkaMessages::RegionChat:\n  From = %s [%s]\n  To = %s [%s]\n  instanceAddress = %s\n  agentControllerId = %d\n  message = %s\n  timestamp = %llu\n  typing = %u\n  offset = %llu\n  highwaterMarkOffset = %llu\n",
            fromPersonaIdFormatted.c_str(),
            fromPersonaIdButts.c_str(),
            toPersonaIdFormatted.c_str(),
            toPersonaIdButts.c_str(),
            instanceAddress.c_str(),
            agentControllerId,
            message.c_str(),
            timestamp,
            typing,
            offset,
            highwaterMarkOffset
        );
    }

    static void OnPrivateChat(PacketReader &reader)  // TAG: 17A1740
    {
        auto offset = reader.ReadUint64();
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        auto fromPersonaIdButts = Utils::ClusterButt(fromPersonaId);
        auto fromPersonaIdFormatted = Utils::ToUUID(fromPersonaId);

        auto toPersonaIdButts = Utils::ClusterButt(toPersonaId);
        auto toPersonaIdFormatted = Utils::ToUUID(toPersonaId);

        printf("ClientKafkaMessages::OnPrivateChat:\n  From = %s [%s]\n  To = %s [%s]\n   message = %s\n  timestamp = %llu\n  offset = %llu\n",
            fromPersonaIdFormatted.c_str(),
            fromPersonaIdButts.c_str(),
            toPersonaIdFormatted.c_str(),
            toPersonaIdButts.c_str(),
            message.c_str(),
            timestamp,
            offset
        );
    }

    static void OnPrivateChatUpdate(PacketReader &reader)  // TAG: 17A17B0
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::OnPrivateChatUpdateStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnShortLivedNotification(PacketReader &reader)  // TAG: 17A3620
    {
        auto id = reader.ReadUUID();
        auto type = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        printf("ClientKafkaMessages::ShortLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
            id.c_str(),
            type,
            message.c_str(),
            timestamp
        );
    }

    static void OnLongLivedNotificationsLoaded(PacketReader &reader) // TAG: 17A3460
    {
        auto offset = reader.ReadUint64();
    }

    static void OnLongLivedNotificationDelete(PacketReader &reader)  // TAG: 17A32D0
    {
        auto id = reader.ReadUUID();

        printf("ClientKafkaMessages::LongLivedNotificationDelete:\n  id = %s\n",
            id.c_str()
        );
    }

    static void OnLongLivedNotification(PacketReader &reader) // TAG: 17A3260
    {
        auto id = reader.ReadUUID();
        auto type = reader.ReadUint32();
        auto message = reader.ReadString();
        auto timestamp = reader.ReadUint64();

        printf("ClientKafkaMessages::LongLivedNotification:\n  id = %s\n  type = %u\n  message = %s\n  timestamp = %llu\n",
            id.c_str(),
            type,
            message.c_str(),
            timestamp
        );
    }

    static void OnUnsubscribeScriptRegionConsole(PacketReader &reader) // TAG: 17A3040
    {
        auto instanceId = reader.ReadString();

        printf("ClientKafkaMessages::UnsubscriptScriptRegionConsole:\n  instanceId = %s\n",
            instanceId.c_str()
        );
    }

    static void OnSubscribeScriptRegionConsole(PacketReader &reader) // TAG: 17A2FD0
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::SubscribeScriptRegionConsole:\n  instanceId = %s\n  offset = %llu\n",
            instanceId.c_str(),
            offset
        );
    }

    static void OnRegionEventMetric(PacketReader &reader) // TAG: 17A2F60
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

        printf("ClientKafkaMessages::RegionEventMetric:\n  ownerPersonaID = %s\n  eventName = %s\n  headcount = %u\n  grid = %s\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
            ownerPersonaID.c_str(),
            eventName.c_str(),
            headcount,
            grid.c_str(),
            ownerPersonaHandle.c_str(),
            experienceHandle.c_str(),
            instanceId.c_str(),
            buildID.c_str(),
            locationHandle.c_str(),
            sansarURI.c_str(),
            compatVersion.c_str(),
            protoVersion.c_str(),
            accessGroup.c_str(),
            configuration.c_str(),
            worldId.c_str()
        );
    }

    static void OnRegionHeartbeatMetric(PacketReader &reader) // TAG: 17A2EF0
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

        printf("ClientKafkaMessages::RegionHeartbeatMetric:\n  ownerPersonaID = %s\n  averageFrameRate = %f\n  minFrameRate = %f\n  maxFrameRate = %f\n  headcount = %u\n  grid = %s\n  ownerPersonaHandle = %s\n  experienceHandle = %s\n  instanceId = %s\n  buildID = %s\n  locationHandle = %s\n  sansarURI = %s\n  compatVersion = %s\n  protoVersion = %s\n  accessGroup = %s\n  configuration = %s\n  worldId = %s\n",
            ownerPersonaID.c_str(),
            averageFrameRate,
            minFrameRate,
            maxFrameRate,
            headcount,
            grid.c_str(),
            ownerPersonaHandle.c_str(),
            experienceHandle.c_str(),
            instanceId.c_str(),
            buildID.c_str(),
            locationHandle.c_str(),
            sansarURI.c_str(),
            compatVersion.c_str(),
            protoVersion.c_str(),
            accessGroup.c_str(),
            configuration.c_str(),
            worldId.c_str()
        );
    }

    static void OnClientMetric(PacketReader &reader) // TAG: 17A2D40
    {
        auto jsonString = reader.ReadString();

        printf("ClientKafkaMessages::ClientMetric:\n  instanceId = %s\n",
            jsonString.c_str()
        );
    }

    static void OnScriptRegionConsoleLoaded(PacketReader &reader) // TAG: 17A2CD0
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::ScriptRegionConsoleLoaded:\n  instanceId = %s\n  offset = %llu\n",
            instanceId.c_str(),
            offset
        );
    }

    static void OnPrivateChatStatusLoaded(PacketReader &reader) // TAG: 17A2B10
    {
        auto offset = reader.ReadUint64();
    }

    static void OnPrivateChatLoaded(PacketReader &reader) // TAG: 17A2950
    {
        auto offset = reader.ReadUint64();
    }

    static void OnRelationshipTableLoaded(PacketReader &reader) // TAG: 17A2790
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendTableLoaded(PacketReader &reader) // TAG: 17A25D0
    {
        auto offset = reader.ReadUint64();
    }

    static void OnPresenceUpdateFanoutLoaded(PacketReader &reader) // TAG: 17A2410
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendResponseLoaded(PacketReader &reader) // TAG: 17A2250
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendRequestLoaded(PacketReader &reader) // TAG: 17A2090
    {
        auto offset = reader.ReadUint64();
    }

    static void OnInventoryLoaded(PacketReader &reader) // TAG: 17A1ED0
    {
        auto offset = reader.ReadUint64();
    }

    static void OnInventoryItemDelete(PacketReader &reader) // TAG: 17A1E60
    {
        auto id = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::InventoryItemDelete:\n  id = %s\n  offset = %llu\n",
            id.c_str(),
            offset
        );
    }

    static void OnInventoryItemRevision(PacketReader &reader) // TAG: 17A1D80
    {
        auto asset_id = reader.ReadString();
        auto asset_type = reader.ReadString();
        auto asset_hint = reader.ReadUint32();
        auto thumbnail_asset_id = reader.ReadString();
        auto license_asset_id = reader.ReadString();
        auto capabilities = reader.ReadStringList();

        printf("ClientKafkaMessages::InventoryItemRevision:\n  asset_id = %s\n  asset_type = %s\n  asset_hint = %u\n  thumbnail_asset_id = %s\n  license_asset_id = %s\n  capabilities =",
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

    static void OnInventoryItemCapabilities(PacketReader &reader)  // TAG: 17A1BA0
    {
        auto capabilities = reader.ReadStringList();

        printf("ClientKafkaMessages::InventoryItemCapabilities:\n");
        for (auto &item : capabilities)
        {
            printf("  %s\n", item.c_str());
        }
    }

    static void OnRelationshipOperation(PacketReader &reader)  // TAG: 17A1AC0
    {
        auto other = reader.ReadUUID();
        auto operation = reader.ReadUint32();

        printf("ClientKafkaMessages::RelationshipOperation:\n  other = %s\n  operation = %u\n",
            other.c_str(),
            operation
        );
    }

    static void OnFriendTable(PacketReader &reader)  // TAG: 17A1A50
    {
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::FriendTable:\n  fromPersonaId = %s\n  toPersonaId = %s\n  status = %u\n",
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            status
        );
    }

    static void OnFriendResponseStatus(PacketReader &reader)  // TAG: 17A19E0
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::FriendResponseStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnFriendResponse(PacketReader &reader)  // TAG: 17A1970
    {
        auto offset = reader.ReadUint64();;
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto timestamp = reader.ReadString(); // WHAT
        auto fromSignature = reader.ReadString();
        auto response = reader.ReadUint32();
        auto toSignature = reader.ReadString();

        printf("ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n  response = %u\n  toSignature = %s\n",
            offset,
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            timestamp.c_str(),
            fromSignature.c_str(),
            response,
            toSignature.c_str()
        );
    }

    static void OnFriendRequestStatus(PacketReader &reader)  // TAG: 17A1900
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::FriendRequestStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnFriendRequest(PacketReader &reader)  // TAG: 17A1890
    {
        auto offset = reader.ReadUint64();;
        auto fromPersonaId = reader.ReadUUID();
        auto toPersonaId = reader.ReadUUID();
        auto timestamp = reader.ReadString(); // WHAT
        auto fromSignature = reader.ReadString();

        printf("ClientKafkaMessages::FriendRequest:\n  offset = %llu\n  fromPersonaId = %s\n  toPersonaId = %s\n  timestamp = %s\n  fromSignature = %s\n",
            offset,
            fromPersonaId.c_str(),
            toPersonaId.c_str(),
            timestamp.c_str(),
            fromSignature.c_str()
        );
    }

    static void OnLeaveRegion(PacketReader &reader)  // TAG: 17A1520
    {
        auto regionAddress = reader.ReadString();

        printf("ClientKafkaMessages::LeaveRegion '%s'\n", regionAddress.c_str());
    }

    static void OnEnterRegion(PacketReader &reader)  // TAG: 17A1370
    {
        auto regionAddress = reader.ReadString();

        printf("ClientKafkaMessages::EnterRegion '%s'\n", regionAddress.c_str());
    }

    static void OnLogin(PacketReader &reader)  // TAG: 17A1290
    {
        auto accountId = reader.ReadUUID();
        auto personaId = reader.ReadUUID();
        auto secret = reader.ReadUint32();
        auto inventoryOffset = reader.ReadUint64();

        printf(" ClientKafkaMessages::Login:\n  accountId = %s\n  personaId = %s\n  secret = %u\n  inventoryOffset = %llu\n",
            accountId.c_str(),
            personaId.c_str(),
            secret,
            inventoryOffset
        );
    }
};

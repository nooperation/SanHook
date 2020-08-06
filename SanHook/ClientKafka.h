#pragma once

#include <cstdio>
#include <string>
#include <vector>

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

class ClientKafka
{
public:
    static void OnScriptConsoleLog(PacketReader &reader)
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

    static void OnLoginReply(PacketReader &reader)
    {
        auto success = reader.ReadUint8();
        auto message = reader.ReadString();

        printf("ClientKafkaMessages::LoginReply: Success = %u (Message = '%s')\n", success, message.c_str());
    }

    static void OnPresenceUpdate(PacketReader &reader)
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

    static void OnRelationshipTable(PacketReader &reader)
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

    static void OnInventoryItemUpdate(PacketReader &reader)
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

    static void OnRegionChat(PacketReader &reader)
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

    static void OnPrivateChat(PacketReader &reader)
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

    static void OnPrivateChatUpdate(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::OnPrivateChatUpdateStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnShortLivedNotification(PacketReader &reader)
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

    static void OnLongLivedNotificationsLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnLongLivedNotificationDelete(PacketReader &reader)
    {
        auto id = reader.ReadUUID();

        printf("ClientKafkaMessages::LongLivedNotificationDelete:\n  id = %s\n",
            id.c_str()
        );
    }

    static void OnLongLivedNotification(PacketReader &reader)
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

    static void OnUnsubscribeScriptRegionConsole(PacketReader &reader)
    {
        auto instanceId = reader.ReadString();

        printf("ClientKafkaMessages::UnsubscriptScriptRegionConsole:\n  instanceId = %s\n",
            instanceId.c_str()
        );
    }

    static void OnSubscribeScriptRegionConsole(PacketReader &reader)
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::SubscribeScriptRegionConsole:\n  instanceId = %s\n  offset = %llu\n",
            instanceId.c_str(),
            offset
        );
    }

    static void OnRegionEventMetric(PacketReader &reader)
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

    static void OnRegionHeartbeatMetric(PacketReader &reader)
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

    static void OnClientMetric(PacketReader &reader)
    {
        auto jsonString = reader.ReadString();

        printf("ClientKafkaMessages::ClientMetric:\n  instanceId = %s\n",
            jsonString.c_str()
        );
    }

    static void OnScriptRegionConsoleLoaded(PacketReader &reader)
    {
        auto instanceId = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::ScriptRegionConsoleLoaded:\n  instanceId = %s\n  offset = %llu\n",
            instanceId.c_str(),
            offset
        );
    }

    static void OnPrivateChatStatusLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnPrivateChatLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnRelationshipTableLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendTableLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnPresenceUpdateFanoutLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendResponseLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnFriendRequestLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnInventoryLoaded(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
    }

    static void OnInventoryItemDelete(PacketReader &reader)
    {
        auto id = reader.ReadString();
        auto offset = reader.ReadUint64();

        printf("ClientKafkaMessages::InventoryItemDelete:\n  id = %s\n  offset = %llu\n",
            id.c_str(),
            offset
        );
    }

    static void OnInventoryItemRevision(PacketReader &reader)
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

    static void OnInventoryItemCapabilities(PacketReader &reader)
    {
        auto capabilities = reader.ReadStringList();

        printf("ClientKafkaMessages::InventoryItemCapabilities:\n");
        for (auto &item : capabilities)
        {
            printf("  %s\n", item.c_str());
        }
    }

    static void OnRelationshipOperation(PacketReader &reader)
    {
        auto other = reader.ReadUUID();
        auto operation = reader.ReadUint32();

        printf("ClientKafkaMessages::RelationshipOperation:\n  other = %s\n  operation = %u\n",
            other.c_str(),
            operation
        );
    }

    static void OnFriendTable(PacketReader &reader)
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

    static void OnFriendResponseStatus(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::FriendResponseStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnFriendResponse(PacketReader &reader)
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

    static void OnFriendRequestStatus(PacketReader &reader)
    {
        auto offset = reader.ReadUint64();
        auto status = reader.ReadUint32();

        printf("ClientKafkaMessages::FriendRequestStatus:\n  offset = %llu\n  status = %u\n",
            offset,
            status
        );
    }

    static void OnFriendRequest(PacketReader &reader)
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

    static void OnLeaveRegion(PacketReader &reader)
    {
        auto regionAddress = reader.ReadString();

        printf("ClientKafkaMessages::LeaveRegion '%s'\n", regionAddress.c_str());
    }

    static void OnEnterRegion(PacketReader &reader)
    {
        auto regionAddress = reader.ReadString();

        printf("ClientKafkaMessages::EnterRegion '%s'\n", regionAddress.c_str());
    }

    static void OnLogin(PacketReader &reader)
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

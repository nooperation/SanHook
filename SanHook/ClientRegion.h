#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <filesystem>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnClientRegionUserLogin(PacketReader &reader);
//void OnClientRegionUserLoginReply(PacketReader &reader);
//void OnClientRegionAddUser(PacketReader &reader);
//void OnClientRegionRemoveUser(PacketReader &reader);
//void OnClientRegionRenameUser(PacketReader &reader);
//void OnClientRegionChatMessageToServer(PacketReader &reader);
//void OnClientRegionChatMessageToClient(PacketReader &reader);
//void OnClientRegionVibrationPulseToClient(PacketReader &reader);
//void OnClientRegionSetAgentController(PacketReader &reader);
//void OnClientRegionTeleportTo(PacketReader &reader);
//void OnClientRegionTeleportToUri(PacketReader &reader);
//void OnClientRegionTeleportToEditMode(PacketReader &reader);
//void OnClientRegionDebugTimeChangeToServer(PacketReader &reader);
//void OnClientRegionDebugTimeChangeToClient(PacketReader &reader);
//void OnClientRegionVisualDebuggerCaptureToServer(PacketReader &reader);
//void OnClientRegionVisualDebuggerCaptureToClient(PacketReader &reader);
//void OnClientRegionScriptModalDialog(PacketReader &reader);
//void OnClientRegionScriptModalDialogResponse(PacketReader &reader);
//void OnClientRegionTwitchEventSubscription(PacketReader &reader);
//void OnClientRegionTwitchEvent(PacketReader &reader);
//void OnClientRegionClientStaticReady(PacketReader &reader);
//void OnClientRegionClientDynamicReady(PacketReader &reader);
//void OnClientRegionInitialChunkSubscribed(PacketReader &reader);
//void OnClientRegionClientRegionCommandMessage(PacketReader &reader);
//void OnClientRegionClientKickNotification(PacketReader &reader);
//void OnClientRegionClientSmiteNotification(PacketReader &reader);
//void OnClientRegionClientMuteNotification(PacketReader &reader);
//void OnClientRegionClientvoiceBroadcastStartNotification(PacketReader &reader);
//void OnClientRegionClientVoiceBroadcastStopNotification(PacketReader &reader);
//void OnClientRegionClientRuntimeInventoryUpdatedNotification(PacketReader &reader);
//void OnClientRegionClientSetRegionBroadcasted(PacketReader &reader);
//void OnClientRegionSubscribeCommand(PacketReader &reader);
//void OnClientRegionUnsubscribeCommand(PacketReader &reader);
//void OnClientRegionClientCommand(PacketReader &reader);
//void OnClientRegionRequestDropPortal(PacketReader &reader);
//void OnClientRegionOpenStoreListing(PacketReader &reader);
//void OnClientRegionOpenUserStore(PacketReader &reader);
//void OnClientRegionOpenQuestCharcterDialog(PacketReader &reader);
//void OnClientRegionUIScriptableBarStart(PacketReader &reader);
//void OnClientRegionUIScriptableBarStopped(PacketReader &reader);
//void OnClientRegionUIScriptableBarCancel(PacketReader &reader);
//void OnClientRegionUIHintTextUpdate(PacketReader &reader);
//void OnClientRegionQuestOfferResponse(PacketReader &reader);
//void OnClientRegionQuestCompleted(PacketReader &reader);
//void OnClientRegionQuestRemoved(PacketReader &reader);
//void OnClientRegionShowWorldDetail(PacketReader &reader);
//void OnClientRegionShowTutorialHints(PacketReader &reader);
//void OnClientRegionTutorialHintsEnabled(PacketReader &reader);


// temp, move into ClientRegion
std::map<uint32_t, std::string> sessionIdToNameMap = std::map<uint32_t, std::string>();

class ClientRegion
{
public:
    static void OnUserLogin(PacketReader &reader)
    {
        auto secret = reader.ReadUint32();
    }

    static void OnUserLoginReply(PacketReader &reader)
    {
        printf("ClientRegionMessages::UserLoginReply\n");

        auto success = reader.ReadUint8();
        auto sessionId = reader.ReadUint32();
        auto regionServerVersion = reader.ReadString();
        auto privileges = reader.ReadStringList();

        printf("LoginReply:\n Success: %d\n SessionId: %d\n RegionServerVersion: %s\n Privileges: \n", success, sessionId, regionServerVersion.c_str());
        for (const auto &item : privileges)
        {
            printf("   %s\n", item.c_str());
        }
    }

    static void OnAddUser(PacketReader &reader)
    {
        static const std::regex kPatternAvatarType("avatarAssetId = \"([^\"]+)\"[^a]+avatarInventoryId = \"([^\"]+)\"");

        printf("ClientRegionMessages::AddUser\n");

        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
        auto handle = reader.ReadString();
        auto avatarType = reader.ReadString();
        auto personaId = reader.ReadUUID();

        auto personaIdButts = Utils::ClusterButt(personaId);
        auto personaIdFormatted = Utils::ToUUID(personaId);

        sessionIdToNameMap[sessionId] = userName;

        std::string avatarAssetId = "";
        std::string avatarInventoryId = "";

        std::smatch match;
        std::regex_search(avatarType, match, kPatternAvatarType);

        if (match[1].matched)
        {
            avatarAssetId = match[1].str();
        }
        if (match[2].matched)
        {
            avatarInventoryId = match[2].str();
        }

        auto avatarAssetIdSwapped = Utils::ClusterButt(avatarAssetId);

        std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\userdump.csv";

        bool needToAddHeader = false;
        if (!std::filesystem::exists(userdumpPath) || std::filesystem::file_size(userdumpPath) == 0)
        {
            needToAddHeader = true;
        }

        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

        FILE *outFile = nullptr;
        fopen_s(&outFile, userdumpPath.string().c_str(), "a");
        if (needToAddHeader)
        {
            fprintf(outFile, "timestamp,username,handle,personaIdSwapped,avatarAssetIdSwapped,personaId,avatarAssetId,avatarInventoryId\n");
        }
        fprintf(outFile, "\"%lld\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
            timestamp,
            userName.c_str(),
            handle.c_str(),
            personaIdButts.c_str(),
            avatarAssetIdSwapped.c_str(),
            personaIdFormatted.c_str(),
            avatarAssetId.c_str(),
            avatarInventoryId.c_str()
        );
        fclose(outFile);

        printf("  SessionID: %d\n  Username: %s\n  Handle: %s\n  PersonaId: %s [%s]\n  avatarAssetId: %s [%s]\n  AvatarType: '%s'\n",
            sessionId,
            userName.c_str(),
            handle.c_str(),
            personaIdFormatted.c_str(),
            personaIdButts.c_str(),
            avatarAssetId.c_str(),
            avatarAssetIdSwapped.c_str(),
            avatarType.c_str()
        );
    }

    static void OnRemoveUser(PacketReader &reader)
    {
        auto sessionId = reader.ReadUint32();

        auto userName = std::string();
        auto foundName = sessionIdToNameMap.find(sessionId);
        if (foundName != sessionIdToNameMap.end())
        {
            userName = foundName->second;
        }
        else
        {
            userName = "UNKNOWN";
        }

        printf("ClientRegionMessages::RemoveUser -> (%s) %d\n", userName.c_str(), sessionId);
    }

    static void OnRenameUser(PacketReader &reader)
    {
        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
    }

    static void OnChatMessageToServer(PacketReader &reader)
    {
        auto toSessionId = reader.ReadUint32();
        auto message = reader.ReadString();
    }

    static void OnChatMessageToClient(PacketReader &reader)
    {
        auto fromSessionId = reader.ReadUint32();
        auto toSessionId = reader.ReadUint32();
        auto message = reader.ReadString();

        auto username = std::string();

        auto usernameFromSessionId = sessionIdToNameMap.find(fromSessionId);
        if (usernameFromSessionId == sessionIdToNameMap.end())
        {
            username = "UNKNOWN";
        }
        else
        {
            username = usernameFromSessionId->second;
        }

        printf("OnChatMessageToClient: (%s) %d -> %d: %s\n", username.c_str(), fromSessionId, toSessionId, message.c_str());
    }

    static void OnVibrationPulseToClient(PacketReader &reader)
    {
        auto controlPointType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
        auto duration = reader.ReadFloat();
    }

    static void OnSetAgentController(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();
        auto frame = reader.ReadUint64();

        printf("OnSetAgentController:\n  agentControllerId = %u\n  frame = %llu\n",
            agentControllerId,
            frame
        );
    }

    static void OnTeleportTo(PacketReader &reader)
    {
        auto personaHandle = reader.ReadString();
        auto locationHandle = reader.ReadString();
    }

    static void OnTeleportToUri(PacketReader &reader)
    {
        auto sansarUri = reader.ReadString();
    }

    static void OnTeleportToEditMode(PacketReader &reader)
    {
        auto returnSpawnPointName = reader.ReadString();
        auto workspaceEditView = reader.ReadUint8();
    }

    static void OnDebugTimeChangeToServer(PacketReader &reader)
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();
    }

    static void OnDebugTimeChangeToClient(PacketReader &reader)
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();
        auto requestAccepted = reader.ReadUint8();
        auto errorMessage = reader.ReadString();
    }

    static void OnVisualDebuggerCaptureToServer(PacketReader &reader)
    {
        auto startTimeFormatted = reader.ReadString();
        auto beginCapture = reader.ReadUint8();
        auto viewers = reader.ReadStringList();
    }

    static void OnVisualDebuggerCaptureToClient(PacketReader &reader)
    {
        auto startTimeFormatted = reader.ReadString();
        auto compressedHkmBytes = reader.ReadArray();
        auto uncompressedSize = reader.ReadUint64();
        auto beginCapture = reader.ReadUint8();
        auto succeeded = reader.ReadUint8();
        auto errorMessage = reader.ReadString();
    }

    static void OnScriptModalDialog(PacketReader &reader)
    {
        auto eventId = reader.ReadUint64();
        auto message = reader.ReadString();
        auto leftButtonLabel = reader.ReadString();
        auto rightButtonLabel = reader.ReadString();
    }

    static void OnScriptModalDialogResponse(PacketReader &reader)
    {
        auto eventId = reader.ReadUint64();
        auto response = reader.ReadString();
    }

    static void OnTwitchEventSubscription(PacketReader &reader)
    {
        auto eventMask = reader.ReadUint32();
    }

    static void OnTwitchEvent(PacketReader &reader)
    {
        auto eventType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
    }

    static void OnClientStaticReady(PacketReader &reader)
    {
        auto ready = reader.ReadUint8();
    }

    static void OnClientDynamicReady(PacketReader &reader)
    {
        auto position = reader.ReadVectorF(3);
        auto orientation = reader.ReadVectorF(4);
        auto targetPersonaId_0 = reader.ReadUint64();
        auto targetPersonaId_1 = reader.ReadUint64();
        auto targetSpawnPointName = reader.ReadString();
        auto spawnStyle = reader.ReadUint8();
        auto ready = reader.ReadUint8();
    }

    static void OnInitialChunkSubscribed(PacketReader &reader)
    {
        auto unused = reader.ReadUint8();
    }

    static void OnClientRegionCommandMessage(PacketReader &reader)
    {
        auto commandLine = reader.ReadString(); // What's this suspicious looking thing
    }

    static void OnClientKickNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientSmiteNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientMuteNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientvoiceBroadcastStartNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientVoiceBroadcastStopNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientRuntimeInventoryUpdatedNotification(PacketReader &reader)
    {
        auto message = reader.ReadString();
    }

    static void OnClientSetRegionBroadcasted(PacketReader &reader)
    {
        auto broadcasted = reader.ReadUint8();
    }

    static void OnSubscribeCommand(PacketReader &reader)
    {
        auto command = reader.ReadString();
        auto action = reader.ReadUint8();
    }

    static void OnUnsubscribeCommand(PacketReader &reader)
    {
        auto action = reader.ReadUint8(); // yes, this is reverse from subscribecommand?
        auto command = reader.ReadString();
    }

    static void OnClientCommand(PacketReader &reader)
    {
        auto command = reader.ReadString();
        auto action = reader.ReadUint8();
        auto origin = reader.ReadVectorF(3);
        auto targetPosition = reader.ReadVectorF(3);
        auto normal = reader.ReadVectorF(3);
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto device = reader.ReadUint8();
        auto mouseLook = reader.ReadUint8();
        auto controlMode = reader.ReadUint8();
        auto isAimTarget = reader.ReadUint8();
    }

    static void OnRequestDropPortal(PacketReader &reader)
    {
        auto sansarUri = reader.ReadString();
        auto sansarUriDescription = reader.ReadString();
    }

    static void OnOpenStoreListing(PacketReader &reader)
    {
        auto listingId = reader.ReadUUID();
    }

    static void OnOpenUserStore(PacketReader &reader)
    {
        auto creatorHandle = reader.ReadString();
    }

    static void OnOpenQuestCharcterDialog(PacketReader &reader)
    {
        auto characterId = reader.ReadUUID();
    }

    static void OnUIScriptableBarStart(PacketReader &reader)
    {
        auto barId = reader.ReadUint32();
        auto scriptEventId = reader.ReadUint64();
        auto label = reader.ReadString();
        auto duration = reader.ReadFloat();
        auto color = reader.ReadVectorF(3);
        auto startPct = reader.ReadFloat();
        auto endPct = reader.ReadFloat();
        auto options = reader.ReadUint8();
        auto start = reader.ReadUint8();
    }

    static void OnUIScriptableBarStopped(PacketReader &reader)
    {
        auto barId = reader.ReadUint32();
        auto scriptEventId = reader.ReadUint64();
        auto completed = reader.ReadUint8();
    }

    static void OnUIScriptableBarCancel(PacketReader &reader)
    {
        auto barId = reader.ReadUint32();
    }

    static void OnUIHintTextUpdate(PacketReader &reader)
    {
        auto text = reader.ReadString();
    }

    static void OnQuestOfferResponse(PacketReader &reader)
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto accepted = reader.ReadUint8();
    }

    static void OnQuestCompleted(PacketReader &reader)
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto completedState = reader.ReadUint32();
    }

    static void OnQuestRemoved(PacketReader &reader)
    {
        auto questId = reader.ReadUUID();
    }

    static void OnShowWorldDetail(PacketReader &reader)
    {
        auto sansarUri = reader.ReadString();
        auto show = reader.ReadUint8();
    }

    static void OnShowTutorialHints(PacketReader &reader)
    {
        auto tutorialHintEnum = reader.ReadUint32();
        auto variant = reader.ReadUint32();
    }

    static void OnTutorialHintsEnabled(PacketReader &reader)
    {
        auto enabled = reader.ReadUint8();
    }
};

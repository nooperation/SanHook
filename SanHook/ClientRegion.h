#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <regex>
#include <chrono>
#include <filesystem>

#include "MessageHandler.h"
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

class ClientRegion : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case ClientRegionMessages::UserLogin: // TAG: 1B9B360
            {
                ClientRegion::OnUserLogin(reader);
                break;
            }
            case ClientRegionMessages::UserLoginReply: // TAG: 1B9B520
            {
                ClientRegion::OnUserLoginReply(reader);
                break;
            }
            case ClientRegionMessages::AddUser: // TAG: 1B9B590
            {
                ClientRegion::OnAddUser(reader);
                break;
            }
            case ClientRegionMessages::RemoveUser: // TAG: 1B9B600
            {
                ClientRegion::OnRemoveUser(reader);
                break;
            }
            case ClientRegionMessages::RenameUser: // TAG: 1B9B7C0
            {
                ClientRegion::OnRenameUser(reader);
                break;
            }
            case ClientRegionMessages::ChatMessageToServer: // TAG: 1B9B830
            {
                ClientRegion::OnChatMessageToServer(reader);
                break;
            }
            case ClientRegionMessages::ChatMessageToClient: // TAG: 1B9B8A0
            {
                ClientRegion::OnChatMessageToClient(reader);
                break;
            }
            case ClientRegionMessages::VibrationPulseToClient: // TAG: 1B9B910
            {
                ClientRegion::OnVibrationPulseToClient(reader);
                break;
            }
            case ClientRegionMessages::SetAgentController: // TAG: 1B9B980
            {
                ClientRegion::OnSetAgentController(reader);
                break;
            }
            case ClientRegionMessages::TeleportTo: // TAG: 1B9B9F0
            {
                ClientRegion::OnTeleportTo(reader);
                break;
            }
            case ClientRegionMessages::TeleportToUri: // TAG: 1B9BA60
            {
                ClientRegion::OnTeleportToUri(reader);
                break;
            }
            case ClientRegionMessages::TeleportToEditMode: // TAG: 1B9BC10
            {
                ClientRegion::OnTeleportToEditMode(reader);
                break;
            }
            case ClientRegionMessages::DebugTimeChangeToServer: // TAG: 1B9BC80
            {
                ClientRegion::OnDebugTimeChangeToServer(reader);
                break;
            }
            case ClientRegionMessages::DebugTimeChangeToClient: // TAG: 1B9BCF0
            {
                ClientRegion::OnDebugTimeChangeToClient(reader);
                break;
            }
            case ClientRegionMessages::VisualDebuggerCaptureToServer: // TAG: 1B9BD60
            {
                ClientRegion::OnVisualDebuggerCaptureToServer(reader);
                break;
            }
            case ClientRegionMessages::VisualDebuggerCaptureToClient: // TAG: 1B9BDD0
            {
                ClientRegion::OnVisualDebuggerCaptureToClient(reader);
                break;
            }
            case ClientRegionMessages::ScriptModalDialog: // TAG: 1B9BE40
            {
                ClientRegion::OnScriptModalDialog(reader);
                break;
            }
            case ClientRegionMessages::ScriptModalDialogResponse: // TAG: 1B9BEB0
            {
                ClientRegion::OnScriptModalDialogResponse(reader);
                break;
            }
            case ClientRegionMessages::TwitchEventSubscription: // TAG: 1B9BF20
            {
                ClientRegion::OnTwitchEventSubscription(reader);
                break;
            }
            case ClientRegionMessages::TwitchEvent: // TAG: 1B9C0E0
            {
                ClientRegion::OnTwitchEvent(reader);
                break;
            }
            case ClientRegionMessages::ClientStaticReady: // TAG: 1B9C150
            {
                ClientRegion::OnClientStaticReady(reader);
                break;
            }
            case ClientRegionMessages::ClientDynamicReady: // TAG: 1B9C310
            {
                ClientRegion::OnClientDynamicReady(reader);
                break;
            }
            case ClientRegionMessages::InitialChunkSubscribed: // TAG: 1B9C380
            {
                ClientRegion::OnInitialChunkSubscribed(reader);
                break;
            }
            case ClientRegionMessages::ClientRegionCommandMessage: // TAG: 1B9C540
            {
                ClientRegion::OnClientRegionCommandMessage(reader);
                break;
            }
            case ClientRegionMessages::ClientKickNotification: // TAG: 1B9C6F0
            {
                ClientRegion::OnClientKickNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientSmiteNotification: // TAG: 1B9C8A0
            {
                ClientRegion::OnClientSmiteNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientMuteNotification: // TAG: 1B9CA50
            {
                ClientRegion::OnClientMuteNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientVoiceBroadcastStartNotification: // TAG: 1B9CC00
            {
                ClientRegion::OnClientvoiceBroadcastStartNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientVoiceBroadcastStopNotification: // TAG: 1B9CDB0
            {
                ClientRegion::OnClientVoiceBroadcastStopNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientRuntimeInventoryUpdatedNotification: // TAG: 1B9CF60
            {
                ClientRegion::OnClientRuntimeInventoryUpdatedNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientSetRegionBroadcasted: // TAG: 1B9D110
            {
                ClientRegion::OnClientSetRegionBroadcasted(reader);
                break;
            }
            case ClientRegionMessages::SubscribeCommand: // TAG: 1B9D2D0
            {
                ClientRegion::OnSubscribeCommand(reader);
                break;
            }
            case ClientRegionMessages::UnsubscribeCommand: // TAG: 1B9D340
            {
                ClientRegion::OnUnsubscribeCommand(reader);
                break;
            }
            case ClientRegionMessages::ClientCommand: // TAG: 1B9D3B0
            {
                ClientRegion::OnClientCommand(reader);
                break;
            }
            case ClientRegionMessages::RequestDropPortal: // TAG: 1B9D420
            {
                ClientRegion::OnRequestDropPortal(reader);
                break;
            }
            case ClientRegionMessages::OpenStoreListing: // TAG: 1B9D490
            {
                ClientRegion::OnOpenStoreListing(reader);
                break;
            }
            case ClientRegionMessages::OpenUserStore: // TAG: 1B9D620
            {
                ClientRegion::OnOpenUserStore(reader);
                break;
            }
            case ClientRegionMessages::OpenQuestCharacterDialog: // TAG: 1B9D7D0
            {
                ClientRegion::OnOpenQuestCharcterDialog(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarStart: // TAG: 1B9D960
            {
                ClientRegion::OnUIScriptableBarStart(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarStopped: // TAG: 1B9D9D0
            {
                ClientRegion::OnUIScriptableBarStopped(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarCancel: // TAG: 1B9DA40
            {
                ClientRegion::OnUIScriptableBarCancel(reader);
                break;
            }
            case ClientRegionMessages::UIHintTextUpdate: // TAG: 1B9DC00
            {
                ClientRegion::OnUIHintTextUpdate(reader);
                break;
            }
            case ClientRegionMessages::QuestOfferResponse: // TAG: 1B9DDB0
            {
                ClientRegion::OnQuestOfferResponse(reader);
                break;
            }
            case ClientRegionMessages::QuestCompleted: // TAG: 1B9DE20
            {
                ClientRegion::OnQuestCompleted(reader);
                break;
            }
            case ClientRegionMessages::QuestRemoved: // TAG: 1B9DE90
            {
                ClientRegion::OnQuestRemoved(reader);
                break;
            }
            case ClientRegionMessages::ShowWorldDetail: // TAG: 1B9E020
            {
                ClientRegion::OnShowWorldDetail(reader);
                break;
            }
            case ClientRegionMessages::ShowTutorialHint: // TAG: 1B9E090
            {
                ClientRegion::OnShowTutorialHints(reader);
                break;
            }
            case ClientRegionMessages::TutorialHintsSetEnabled: // TAG: 1B9E100
            {
                ClientRegion::OnTutorialHintsEnabled(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnUserLogin(PacketReader &reader)// TAG: 1B9B360
    {
        auto secret = reader.ReadUint32();
    }

    static void OnUserLoginReply(PacketReader &reader)// TAG: 1B9B520
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

    static void OnAddUser(PacketReader &reader) // TAG: 1B9B590
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

    static void OnRemoveUser(PacketReader &reader) // TAG: 1B9B600
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

    static void OnRenameUser(PacketReader &reader) // TAG: 1B9B7C0
    {
        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
    }

    static void OnChatMessageToServer(PacketReader &reader) // TAG: 1B9B830
    {
        auto toSessionId = reader.ReadUint32();
        auto message = reader.ReadString();
    }

    static void OnChatMessageToClient(PacketReader &reader) // TAG: 1B9B8A0
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

    static void OnVibrationPulseToClient(PacketReader &reader) // TAG: 1B9B910
    {
        auto controlPointType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
        auto duration = reader.ReadFloat();
    }

    static void OnSetAgentController(PacketReader &reader) // TAG: 1B9B980
    {
        auto agentControllerId = reader.ReadUint32();
        auto frame = reader.ReadUint64();

        printf("OnSetAgentController:\n  agentControllerId = %u\n  frame = %llu\n",
            agentControllerId,
            frame
        );
    }

    static void OnTeleportTo(PacketReader &reader) // TAG: 1B9B9F0
    {
        auto personaHandle = reader.ReadString();
        auto locationHandle = reader.ReadString();
    }

    static void OnTeleportToUri(PacketReader &reader) // TAG: 1B9BA60
    {
        auto sansarUri = reader.ReadString();
    }

    static void OnTeleportToEditMode(PacketReader &reader) // TAG: 1B9BC10
    {
        auto returnSpawnPointName = reader.ReadString();
        auto workspaceEditView = reader.ReadUint8();
    }

    static void OnDebugTimeChangeToServer(PacketReader &reader) // TAG: 1B9BC80
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();
    }

    static void OnDebugTimeChangeToClient(PacketReader &reader) // TAG: 1B9BCF0
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();
        auto requestAccepted = reader.ReadUint8();
        auto errorMessage = reader.ReadString();
    }

    static void OnVisualDebuggerCaptureToServer(PacketReader &reader) // TAG: 1B9BD60
    {
        auto startTimeFormatted = reader.ReadString();
        auto beginCapture = reader.ReadUint8();
        auto viewers = reader.ReadStringList();
    }

    static void OnVisualDebuggerCaptureToClient(PacketReader &reader) // TAG: 1B9BDD0
    {
        auto startTimeFormatted = reader.ReadString();
        auto compressedHkmBytes = reader.ReadArray();
        auto uncompressedSize = reader.ReadUint64();
        auto beginCapture = reader.ReadUint8();
        auto succeeded = reader.ReadUint8();
        auto errorMessage = reader.ReadString();
    }

    static void OnScriptModalDialog(PacketReader &reader) // TAG: 1B9BE40
    {
        auto eventId = reader.ReadUint64();
        auto message = reader.ReadString();
        auto leftButtonLabel = reader.ReadString();
        auto rightButtonLabel = reader.ReadString();
    }

    static void OnScriptModalDialogResponse(PacketReader &reader) // TAG: 1B9BEB0
    {
        auto eventId = reader.ReadUint64();
        auto response = reader.ReadString();
    }

    static void OnTwitchEventSubscription(PacketReader &reader) // TAG: 1B9BF20
    {
        auto eventMask = reader.ReadUint32();
    }

    static void OnTwitchEvent(PacketReader &reader) // TAG: 1B9C0E0
    {
        auto eventType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
    }

    static void OnClientStaticReady(PacketReader &reader) // TAG: 1B9C150
    {
        auto ready = reader.ReadUint8();
    }

    static void OnClientDynamicReady(PacketReader &reader) // TAG: 1B9C310
    {
        auto position = reader.ReadVectorF(3);
        auto orientation = reader.ReadVectorF(4);
        auto targetPersonaId_0 = reader.ReadUint64();
        auto targetPersonaId_1 = reader.ReadUint64();
        auto targetSpawnPointName = reader.ReadString();
        auto spawnStyle = reader.ReadUint8();
        auto ready = reader.ReadUint8();
    }

    static void OnInitialChunkSubscribed(PacketReader &reader) // TAG: 1B9C380
    {
        auto unused = reader.ReadUint8();
    }

    static void OnClientRegionCommandMessage(PacketReader &reader) // TAG: 1B9C540
    {
        auto commandLine = reader.ReadString(); // What's this suspicious looking thing
    }

    static void OnClientKickNotification(PacketReader &reader) // TAG: 1B9C6F0
    {
        auto message = reader.ReadString();
    }

    static void OnClientSmiteNotification(PacketReader &reader) // TAG: 1B9C8A0
    {
        auto message = reader.ReadString();
    }

    static void OnClientMuteNotification(PacketReader &reader) // TAG: 1B9CA50
    {
        auto message = reader.ReadString();
    }

    static void OnClientvoiceBroadcastStartNotification(PacketReader &reader) // TAG: 1B9CC00
    {
        auto message = reader.ReadString();
    }

    static void OnClientVoiceBroadcastStopNotification(PacketReader &reader) // TAG: 1B9CDB0
    {
        auto message = reader.ReadString();
    }

    static void OnClientRuntimeInventoryUpdatedNotification(PacketReader &reader) // TAG: 1B9CF60
    {
        auto message = reader.ReadString();
    }

    static void OnClientSetRegionBroadcasted(PacketReader &reader) // TAG: 1B9D110
    {
        auto broadcasted = reader.ReadUint8();
    }

    static void OnSubscribeCommand(PacketReader &reader) // TAG: 1B9D2D0
    {
        auto command = reader.ReadString();
        auto action = reader.ReadUint8();
    }

    static void OnUnsubscribeCommand(PacketReader &reader) // TAG: 1B9D340
    {
        auto action = reader.ReadUint8(); // yes, this is reverse from subscribecommand?
        auto command = reader.ReadString();
    }

    static void OnClientCommand(PacketReader &reader) // TAG: 1B9D3B0
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

    static void OnRequestDropPortal(PacketReader &reader) // TAG: 1B9D420
    {
        auto sansarUri = reader.ReadString();
        auto sansarUriDescription = reader.ReadString();
    }

    static void OnOpenStoreListing(PacketReader &reader) // TAG: 1B9D490
    {
        auto listingId = reader.ReadUUID();
    }

    static void OnOpenUserStore(PacketReader &reader) // TAG: 1B9D620
    {
        auto creatorHandle = reader.ReadString();
    }

    static void OnOpenQuestCharcterDialog(PacketReader &reader) // TAG: 1B9D7D0
    {
        auto characterId = reader.ReadUUID();
    }

    static void OnUIScriptableBarStart(PacketReader &reader) // TAG: 1B9D960
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

    static void OnUIScriptableBarStopped(PacketReader &reader) // TAG: 1B9D9D0
    {
        auto barId = reader.ReadUint32();
        auto scriptEventId = reader.ReadUint64();
        auto completed = reader.ReadUint8();
    }

    static void OnUIScriptableBarCancel(PacketReader &reader) // TAG: 1B9DA40
    {
        auto barId = reader.ReadUint32();
    }

    static void OnUIHintTextUpdate(PacketReader &reader) // TAG: 1B9DC00
    {
        auto text = reader.ReadString();
    }

    static void OnQuestOfferResponse(PacketReader &reader) // TAG: 1B9DDB0
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto accepted = reader.ReadUint8();
    }

    static void OnQuestCompleted(PacketReader &reader) // TAG: 1B9DE20
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto completedState = reader.ReadUint32();
    }

    static void OnQuestRemoved(PacketReader &reader) // TAG: 1B9DE90
    {
        auto questId = reader.ReadUUID();
    }

    static void OnShowWorldDetail(PacketReader &reader) // TAG: 1B9E020
    {
        auto sansarUri = reader.ReadString();
        auto show = reader.ReadUint8();
    }

    static void OnShowTutorialHints(PacketReader &reader) // TAG: 1B9E090
    {
        auto tutorialHintEnum = reader.ReadUint32();
        auto variant = reader.ReadUint32();
    }

    static void OnTutorialHintsEnabled(PacketReader &reader) // TAG: 1B9E100
    {
        auto enabled = reader.ReadUint8();
    }
};

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

extern bool isFlyMode;

class ClientRegion : public MessageHandler
{
private:
    std::map<uint32_t, std::string> sessionIdToNameMap = std::map<uint32_t, std::string>();

public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case ClientRegionMessages::UserLogin: // TAG: 1B9B360
            {
                this->OnUserLogin(reader);
                break;
            }
            case ClientRegionMessages::UserLoginReply: // TAG: 1B9B520
            {
                this->OnUserLoginReply(reader);
                break;
            }
            case ClientRegionMessages::AddUser: // TAG: 1B9B590
            {
                this->OnAddUser(reader);
                break;
            }
            case ClientRegionMessages::RemoveUser: // TAG: 1B9B600
            {
                this->OnRemoveUser(reader);
                break;
            }
            case ClientRegionMessages::RenameUser: // TAG: 1B9B7C0
            {
                this->OnRenameUser(reader);
                break;
            }
            case ClientRegionMessages::ChatMessageToServer: // TAG: 1B9B830
            {
                this->OnChatMessageToServer(reader);
                break;
            }
            case ClientRegionMessages::ChatMessageToClient: // TAG: 1B9B8A0
            {
                this->OnChatMessageToClient(reader);
                break;
            }
            case ClientRegionMessages::VibrationPulseToClient: // TAG: 1B9B910
            {
                this->OnVibrationPulseToClient(reader);
                break;
            }
            case ClientRegionMessages::SetAgentController: // TAG: 1B9B980
            {
                this->OnSetAgentController(reader);
                break;
            }
            case ClientRegionMessages::TeleportTo: // TAG: 1B9B9F0
            {
                this->OnTeleportTo(reader);
                break;
            }
            case ClientRegionMessages::TeleportToUri: // TAG: 1B9BA60
            {
                this->OnTeleportToUri(reader);
                break;
            }
            case ClientRegionMessages::TeleportToEditMode: // TAG: 1B9BC10
            {
                this->OnTeleportToEditMode(reader);
                break;
            }
            case ClientRegionMessages::DebugTimeChangeToServer: // TAG: 1B9BC80
            {
                this->OnDebugTimeChangeToServer(reader);
                break;
            }
            case ClientRegionMessages::DebugTimeChangeToClient: // TAG: 1B9BCF0
            {
                this->OnDebugTimeChangeToClient(reader);
                break;
            }
            case ClientRegionMessages::VisualDebuggerCaptureToServer: // TAG: 1B9BD60
            {
                this->OnVisualDebuggerCaptureToServer(reader);
                break;
            }
            case ClientRegionMessages::VisualDebuggerCaptureToClient: // TAG: 1B9BDD0
            {
                this->OnVisualDebuggerCaptureToClient(reader);
                break;
            }
            case ClientRegionMessages::ScriptModalDialog: // TAG: 1B9BE40
            {
                this->OnScriptModalDialog(reader);
                break;
            }
            case ClientRegionMessages::ScriptModalDialogResponse: // TAG: 1B9BEB0
            {
                this->OnScriptModalDialogResponse(reader);
                break;
            }
            case ClientRegionMessages::TwitchEventSubscription: // TAG: 1B9BF20
            {
                this->OnTwitchEventSubscription(reader);
                break;
            }
            case ClientRegionMessages::TwitchEvent: // TAG: 1B9C0E0
            {
                this->OnTwitchEvent(reader);
                break;
            }
            case ClientRegionMessages::ClientStaticReady: // TAG: 1B9C150
            {
                this->OnClientStaticReady(reader);
                break;
            }
            case ClientRegionMessages::ClientDynamicReady: // TAG: 1B9C310
            {
                this->OnClientDynamicReady(reader);
                break;
            }
            case ClientRegionMessages::InitialChunkSubscribed: // TAG: 1B9C380
            {
                this->OnInitialChunkSubscribed(reader);
                break;
            }
            case ClientRegionMessages::ClientRegionCommandMessage: // TAG: 1B9C540
            {
                this->OnClientRegionCommandMessage(reader);
                break;
            }
            case ClientRegionMessages::ClientKickNotification: // TAG: 1B9C6F0
            {
                this->OnClientKickNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientSmiteNotification: // TAG: 1B9C8A0
            {
                this->OnClientSmiteNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientMuteNotification: // TAG: 1B9CA50
            {
                this->OnClientMuteNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientVoiceBroadcastStartNotification: // TAG: 1B9CC00
            {
                this->OnClientvoiceBroadcastStartNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientVoiceBroadcastStopNotification: // TAG: 1B9CDB0
            {
                this->OnClientVoiceBroadcastStopNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientRuntimeInventoryUpdatedNotification: // TAG: 1B9CF60
            {
                this->OnClientRuntimeInventoryUpdatedNotification(reader);
                break;
            }
            case ClientRegionMessages::ClientSetRegionBroadcasted: // TAG: 1B9D110
            {
                this->OnClientSetRegionBroadcasted(reader);
                break;
            }
            case ClientRegionMessages::SubscribeCommand: // TAG: 1B9D2D0
            {
                this->OnSubscribeCommand(reader);
                break;
            }
            case ClientRegionMessages::UnsubscribeCommand: // TAG: 1B9D340
            {
                this->OnUnsubscribeCommand(reader);
                break;
            }
            case ClientRegionMessages::ClientCommand: // TAG: 1B9D3B0
            {
                this->OnClientCommand(reader);
                break;
            }
            case ClientRegionMessages::RequestDropPortal: // TAG: 1B9D420
            {
                this->OnRequestDropPortal(reader);
                break;
            }
            case ClientRegionMessages::OpenStoreListing: // TAG: 1B9D490
            {
                this->OnOpenStoreListing(reader);
                break;
            }
            case ClientRegionMessages::OpenUserStore: // TAG: 1B9D620
            {
                this->OnOpenUserStore(reader);
                break;
            }
            case ClientRegionMessages::OpenQuestCharacterDialog: // TAG: 1B9D7D0
            {
                this->OnOpenQuestCharcterDialog(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarStart: // TAG: 1B9D960
            {
                this->OnUIScriptableBarStart(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarStopped: // TAG: 1B9D9D0
            {
                this->OnUIScriptableBarStopped(reader);
                break;
            }
            case ClientRegionMessages::UIScriptableBarCancel: // TAG: 1B9DA40
            {
                this->OnUIScriptableBarCancel(reader);
                break;
            }
            case ClientRegionMessages::UIHintTextUpdate: // TAG: 1B9DC00
            {
                this->OnUIHintTextUpdate(reader);
                break;
            }
            case ClientRegionMessages::QuestOfferResponse: // TAG: 1B9DDB0
            {
                this->OnQuestOfferResponse(reader);
                break;
            }
            case ClientRegionMessages::QuestCompleted: // TAG: 1B9DE20
            {
                this->OnQuestCompleted(reader);
                break;
            }
            case ClientRegionMessages::QuestRemoved: // TAG: 1B9DE90
            {
                this->OnQuestRemoved(reader);
                break;
            }
            case ClientRegionMessages::ShowWorldDetail: // TAG: 1B9E020
            {
                this->OnShowWorldDetail(reader);
                break;
            }
            case ClientRegionMessages::ShowTutorialHint: // TAG: 1B9E090
            {
                this->OnShowTutorialHints(reader);
                break;
            }
            case ClientRegionMessages::TutorialHintsSetEnabled: // TAG: 1B9E100
            {
                this->OnTutorialHintsEnabled(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnUserLogin(PacketReader &reader)// TAG: 1B9B360
    {
        auto secret = reader.ReadUint32();

        myControllerId = UINT32_MAX;
        mySessionId = UINT32_MAX;
        myComponentId = UINT64_MAX;
        targetComponentId = UINT64_MAX;
        sessionToComponentIdMap.clear();
        handleToSessionIdMap.clear();

        printf("[%s] ClientRegionMessages::UserLogin\n  secret = %u\n",
            _isSender ? "OUT" : "IN",
            secret
        );
    }

    void OnUserLoginReply(PacketReader &reader)// TAG: 1B9B520
    {
        auto success = reader.ReadUint8();
        auto sessionId = reader.ReadUint32();
        auto regionServerVersion = reader.ReadString();
        auto privileges = reader.ReadStringList();

        mySessionId = sessionId;

        printf("[%s] ClientRegionMessages::UserLoginReply:\n Success: %d\n SessionId: %d\n RegionServerVersion: %s\n Privileges: \n",
            _isSender ? "OUT" : "IN",
            success,
            sessionId,
            regionServerVersion.c_str()
        );
        for (const auto &item : privileges)  // TODO: what if inject our own privliges here. maybe no longer need the other patches in the loader for freecam?
        {
            printf("    %s\n", item.c_str());
        }
    }

    void OnAddUser(PacketReader &reader) // TAG: 1B9B590
    {
        static const std::regex kPatternAvatarType("avatarAssetId = \"([^\"]+)\"[^a]+avatarInventoryId = \"([^\"]+)\"");

        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
        auto handle = reader.ReadString();
        auto avatarType = reader.ReadString();
        auto personaId = reader.ReadUUID();

        auto personaIdButts = Utils::ClusterButt(personaId);
        auto personaIdFormatted = Utils::ToUUID(personaId);

        handleToSessionIdMap[handle] = sessionId;

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

        sessionIdToNameMap[sessionId] = userName;

        printf("[%s] ClientRegionMessages::AddUser\n  SessionID: %d\n  Username: %s\n  Handle: %s\n  PersonaId: %s [%s]\n  avatarAssetId: %s [%s]\n  AvatarType: '%s'\n",
            _isSender ? "OUT" : "IN",
            sessionId,
            userName.c_str(),
            handle.c_str(),
            personaIdFormatted.c_str(),
            personaIdButts.c_str(),
            avatarAssetId.c_str(),
            avatarAssetIdSwapped.c_str(),
            avatarType.c_str()
        );

        if (!_isSender)
        {
            std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\userdump.csv";

            bool needToAddHeader = false;
            if (!std::filesystem::exists(userdumpPath) || std::filesystem::file_size(userdumpPath) == 0)
            {
                needToAddHeader = true;
            }

            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

            FILE *outFile = nullptr;
            for (auto i = 0; i < 10; ++i)
            {
                fopen_s(&outFile, userdumpPath.string().c_str(), "a");
                if (outFile != nullptr)
                {
                    break;
                }
            }

            if (outFile != nullptr)
            {
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
            }
        }
    }

    void OnRemoveUser(PacketReader &reader) // TAG: 1B9B600
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

        printf("[%s] ClientRegionMessages::RemoveUser -> (%s) %d\n",
            _isSender ? "OUT" : "IN",
            userName.c_str(),
            sessionId
        );
    }

    void OnRenameUser(PacketReader &reader) // TAG: 1B9B7C0
    {
        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();

        auto previousUsername = std::string();
        auto foundName = sessionIdToNameMap.find(sessionId);
        if (foundName != sessionIdToNameMap.end())
        {
            previousUsername = foundName->second;
        }
        else
        {
            previousUsername = "UNKNOWN";
        }

        printf("[%s] ClientRegionMessages::RenameUser -> (%s) %d was renamed to %s\n",
            _isSender ? "OUT" : "IN",
            previousUsername.c_str(),
            sessionId,
            userName.c_str()
        );




    }

    void OnChatMessageToServer(PacketReader &reader) // TAG: 1B9B830
    {
        auto toSessionId = reader.ReadUint32();
        auto message = reader.ReadString();

        printf("[%s] ClientRegionMessages::ChatMessageToServer -> (%u) %s\n",
            _isSender ? "OUT" : "IN",
            toSessionId,
            message.c_str()
        );

        /*
        auto buffer = reader.GetBuffer();
        auto *messageId = (uint32_t *)&buffer[0];
        *messageId = ClientRegionMessages::RenameUser;

        auto *toSessionIdPtr = (uint32_t *)&buffer[4];
        *toSessionIdPtr = toSessionId;
        */
    }

    void OnChatMessageToClient(PacketReader &reader) // TAG: 1B9B8A0
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

        printf("[%s] OnChatMessageToClient: (%s) %d -> %d: %s\n",
            _isSender ? "OUT" : "IN",
            username.c_str(),
            fromSessionId,
            toSessionId,
            message.c_str()
        );
    }

    void OnVibrationPulseToClient(PacketReader &reader) // TAG: 1B9B910
    {
        auto controlPointType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
        auto duration = reader.ReadFloat();
    }

    void OnSetAgentController(PacketReader &reader) // TAG: 1B9B980
    {
        auto agentControllerId = reader.ReadUint32();
        auto frame = reader.ReadUint64();

        myControllerId = agentControllerId;

        printf("[%s] OnSetAgentController:\n  agentControllerId = %u\n  frame = %llu\n",
            _isSender ? "OUT" : "IN",
            agentControllerId,
            frame
        );
    }

    void OnTeleportTo(PacketReader &reader) // TAG: 1B9B9F0
    {
        auto personaHandle = reader.ReadString();
        auto locationHandle = reader.ReadString();

        printf("[%s] OnTeleportTo:\n  personaHandle = %s\n  locationHandle = %s\n",
            _isSender ? "OUT" : "IN",
            personaHandle.c_str(),
            locationHandle.c_str()
        );
    }

    void OnTeleportToUri(PacketReader &reader) // TAG: 1B9BA60
    {
        auto sansarUri = reader.ReadString();

        printf("[%s] OnTeleportToUri:\n  sansarUri = %s\n",
            _isSender ? "OUT" : "IN",
            sansarUri.c_str()
        );
    }

    void OnTeleportToEditMode(PacketReader &reader) // TAG: 1B9BC10
    {
        auto returnSpawnPointName = reader.ReadString();
        auto workspaceEditView = reader.ReadUint8();

        // 2 = fashon picker thingy for avatar
        // 3 = select avatar
        // 4 = select your new avatar
        printf("[%s] OnTeleportToEditMode:\n  returnSpawnPointName = %s\n  workspaceEditView = %u\n",
            _isSender ? "OUT" : "IN",
            returnSpawnPointName.c_str(),
            workspaceEditView
        );
    }

    void OnDebugTimeChangeToServer(PacketReader &reader) // TAG: 1B9BC80
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();

        printf("[%s] OnDebugTimeChangeToServer:\n  requestId = %u\n  clientDeltaTimeForced = %f\n  clientDeltaTimeScale = %f\n",
            _isSender ? "OUT" : "IN",
            requestId,
            clientDeltaTimeForced,
            clientDeltaTimeScale
        );
    }

    void OnDebugTimeChangeToClient(PacketReader &reader) // TAG: 1B9BCF0
    {
        auto requestId = reader.ReadUint32();
        auto clientDeltaTimeForced = reader.ReadFloat();
        auto clientDeltaTimeScale = reader.ReadFloat();
        auto requestAccepted = reader.ReadUint8();
        auto errorMessage = reader.ReadString();

        printf("[%s] OnDebugTimeChangeToServer:\n  requestId = %u\n  clientDeltaTimeForced = %f\n  clientDeltaTimeScale = %f\n  requestAccepted = %u\n  errorMessage = %s\n",
            _isSender ? "OUT" : "IN",
            requestId,
            clientDeltaTimeForced,
            clientDeltaTimeScale,
            requestAccepted,
            errorMessage.c_str()
        );
    }

    void OnVisualDebuggerCaptureToServer(PacketReader &reader) // TAG: 1B9BD60
    {
        auto startTimeFormatted = reader.ReadString();
        auto beginCapture = reader.ReadUint8();
        auto viewers = reader.ReadStringList();

        printf("[%s] OnVisualDebuggerCaptureToServer:\n  startTimeFormatted = %s\n  beginCapture = %u\n  viewers =\n",
            _isSender ? "OUT" : "IN",
            startTimeFormatted.c_str(),
            beginCapture
        );

        for (auto &item: viewers)
        {
            printf("     %s\n", item.c_str());
        }
    }

    void OnVisualDebuggerCaptureToClient(PacketReader &reader) // TAG: 1B9BDD0
    {
        auto startTimeFormatted = reader.ReadString();
        auto compressedHkmBytes = reader.ReadArray();
        auto uncompressedSize = reader.ReadUint64();
        auto beginCapture = reader.ReadUint8();
        auto succeeded = reader.ReadUint8();
        auto errorMessage = reader.ReadString();
    }

    void OnScriptModalDialog(PacketReader &reader) // TAG: 1B9BE40
    {
        auto eventId = reader.ReadUint64();
        auto message = reader.ReadString();
        auto leftButtonLabel = reader.ReadString();
        auto rightButtonLabel = reader.ReadString();

        printf("[%s] OnScriptModalDialog:\n  eventId = %llu\n  message = %s\n  leftButtonLabel = %s\n  rightButtonLabel = %s\n",
            _isSender ? "OUT" : "IN",
            eventId,
            message.c_str(),
            leftButtonLabel.c_str(),
            rightButtonLabel.c_str()
        );
    }

    void OnScriptModalDialogResponse(PacketReader &reader) // TAG: 1B9BEB0
    {
        auto eventId = reader.ReadUint64();
        auto response = reader.ReadString();

        printf("[%s] OnScriptModalDialogResponse:\n  eventId = %llu\n  response = %s\n",
            _isSender ? "OUT" : "IN",
            eventId,
            response.c_str()
        );
    }

    void OnTwitchEventSubscription(PacketReader &reader) // TAG: 1B9BF20
    {
        auto eventMask = reader.ReadUint32();
    }

    void OnTwitchEvent(PacketReader &reader) // TAG: 1B9C0E0
    {
        auto eventType = reader.ReadUint32();
        auto intensity = reader.ReadFloat();
    }

    void OnClientStaticReady(PacketReader &reader) // TAG: 1B9C150
    {
        auto ready = reader.ReadUint8();
    }

    void OnClientDynamicReady(PacketReader &reader) // TAG: 1B9C310
    {
        auto position = reader.ReadVectorF(3);
        auto orientation = reader.ReadVectorF(4);
        auto targetPersonaId_0 = reader.ReadUint64();
        auto targetPersonaId_1 = reader.ReadUint64();
        auto targetSpawnPointName = reader.ReadString();
        auto spawnStyle = reader.ReadUint8();
        auto ready = reader.ReadUint8();

        if (_isVerbose)
        {
            printf("OnClientDynamicReady\n  targetPersonaId_0 = %llu\n  targetPersonaId_1 = %llu\n  targetSpawnPointName = %s\n  spawnStyle = %u\n  ready = %u\n",
                targetPersonaId_0,
                targetPersonaId_1,
                targetSpawnPointName.c_str()
            );
        }
    }

    void OnInitialChunkSubscribed(PacketReader &reader) // TAG: 1B9C380
    {
        auto unused = reader.ReadUint8();
    }

    // whenever a user does %%command we send this and the voicemoderationcommand packets
    void OnClientRegionCommandMessage(PacketReader &reader) // TAG: 1B9C540
    {
        auto commandLine = reader.ReadString();

        printf("[%s] OnClientRegionCommandMessage:\n  commandLine = %s\n",
            _isSender ? "OUT" : "IN",
            commandLine.c_str()
        );

        if (_isSender)
        {
            if (commandLine.length() != 0 && commandLine[0] == '%')
            {
                HandleCustomCommand(commandLine.substr(1));
            }
            
            /*
            // aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa123
            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

            unsigned char personaFrom[16] = {
                0x90, 0x4C, 0x58, 0x02, 0x2B, 0xAD, 0x3A, 0x1C, 0xF9, 0x43, 0x97, 0x5A,
                0xA3, 0x0D, 0x04, 0xA6
            };


            ///////////////////////////////////
            *((uint32_t *)&buffer[0]) = ClientKafkaMessages::RegionChat;
            memcpy(&buffer[4], personaFrom, 16);
            memset(&buffer[20], 0, 16);
            *((uint32_t *)&buffer[36]) = 0; // instanceAddress
            *((uint32_t *)&buffer[40]) = 0; // agentControllerId
            *((uint32_t *)&buffer[44]) = 2; // messageLength
            buffer[48] = 'H';
            buffer[49] = 'i';
            *((uint64_t *)&buffer[50]) = 0;
            buffer[58] = 0;
            *((uint64_t *)&buffer[59]) = 0;
            *((uint64_t *)&buffer[67]) = 0;
            */


            //// This works:
            //// %%<url>aaaa
            //*((uint32_t *)&buffer[0]) = ClientRegionMessages::RequestDropPortal;
            //*((uint32_t *)&buffer[4]) = *((uint32_t *)&buffer[4]) - 4;
            //*((uint32_t *)&buffer[reader.GetBufferSize() - 4]) = 0;

        }
    }

    void HandleCustomCommand(std::string message)
    {
        auto args = std::string();
        auto command = message;

        auto spacer = message.find_first_of(' ');
        if (spacer != std::string::npos)
        {
            command = message.substr(0, spacer);
            args = message.substr(spacer + 1);
        }
        
        if (command == "fly")
        {
            isFlyMode = !isFlyMode;
            printf("FlyMode = %s!\n", isFlyMode ? "TRUE" : "FALSE");
        }
        if (command == "follow")
        {
            auto handle = args;

            if (handle == "")
            {
                printf("No longer following %d\n", targetComponentId);
                targetComponentId = UINT64_MAX;
                return;
            }

            if (handle == "ALL")
            {
                printf("Following ALL\n");
                targetComponentId = 0;
                return;
            }

            auto handleResult = handleToSessionIdMap.find(handle);
            if (handleResult == handleToSessionIdMap.end())
            {
                printf("! Cannot follow. Unable to find session id for '%s'\n", handle.c_str());
                return;
            }
            auto sessionId = handleResult->second;
            
            auto componentIdResult = sessionToComponentIdMap.find(sessionId);
            if (componentIdResult == sessionToComponentIdMap.end())
            {
                printf("! Cannot follow. Unable to find componentId id for '%s' (session = %d)\n", handle.c_str(), sessionId);
                return;
            }
            auto componentId = componentIdResult->second;

            printf("Now following %s (sessionId = %d)(componentId = %d)\n",
                handle.c_str(),
                sessionId,
                componentId
            );

            targetComponentId = componentId;
        }
    }

    void OnClientKickNotification(PacketReader &reader) // TAG: 1B9C6F0
    {
        auto message = reader.ReadString();

        printf("[%s] OnClientKickNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    void OnClientSmiteNotification(PacketReader &reader) // TAG: 1B9C8A0
    {
        auto message = reader.ReadString();

        printf("[%s] OnClientSmiteNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    void OnClientMuteNotification(PacketReader &reader) // TAG: 1B9CA50
    {
        auto message = reader.ReadString();

        printf("[%s] OnClientMuteNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    void OnClientvoiceBroadcastStartNotification(PacketReader &reader) // TAG: 1B9CC00
    {
        auto message = reader.ReadString();

        printf("[%s] OnClientvoiceBroadcastStartNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    void OnClientVoiceBroadcastStopNotification(PacketReader &reader) // TAG: 1B9CDB0
    {
        auto message = reader.ReadString();


        printf("[%s] OnClientVoiceBroadcastStopNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    // %%backpack-on  %%backpack-off  %%backpack-clear
    void OnClientRuntimeInventoryUpdatedNotification(PacketReader &reader) // TAG: 1B9CF60
    {
        auto message = reader.ReadString();

        printf("[%s] OnClientRuntimeInventoryUpdatedNotification:\n  message = %s\n",
            _isSender ? "OUT" : "IN",
            message.c_str()
        );
    }

    void OnClientSetRegionBroadcasted(PacketReader &reader) // TAG: 1B9D110
    {
        auto broadcasted = reader.ReadUint8();
    }

    void OnSubscribeCommand(PacketReader &reader) // TAG: 1B9D2D0
    {
        auto command = reader.ReadString();
        auto action = reader.ReadUint8();

        printf("[%s] OnSubscribeCommand:\n  command = %s\n  action = %u\n",
            _isSender ? "OUT" : "IN",
            command.c_str(),
            action
        );
    }

    void OnUnsubscribeCommand(PacketReader &reader) // TAG: 1B9D340
    {
        auto action = reader.ReadUint8(); // yes, this is reverse from subscribecommand?
        auto command = reader.ReadString();

        printf("[%s] OnUnsubscribeCommand:\n  command = %s\n  action = %u\n",
            _isSender ? "OUT" : "IN",
            command.c_str(),
            action
        );
    }

    void OnClientCommand(PacketReader &reader) // TAG: 1B9D3B0
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

        printf("[%s] OnClientCommand:\n  command = %s\n  action = %u\n  origin = <%f, %f, %f>\n  targetPosition = <%f, %f, %f>\n",
            _isSender ? "OUT" : "IN",
            command.c_str(),
            action,
            origin[0], origin[1], origin[2],
            targetPosition[0], targetPosition[1], targetPosition[2]
        );
    }

    void OnRequestDropPortal(PacketReader &reader) // TAG: 1B9D420
    {
        auto sansarUri = reader.ReadString();
        auto sansarUriDescription = reader.ReadString();

        printf("[%s] OnRequestDropPortal:\n  sansarUri = %s\n  sansarUriDescription = %s\n",
            _isSender ? "OUT" : "IN",
            sansarUri.c_str(),
            sansarUriDescription.c_str()
        );
    }

    void OnOpenStoreListing(PacketReader &reader) // TAG: 1B9D490
    {
        auto listingId = reader.ReadUUID();

        printf("[%s] OnOpenStoreListing:\n  listingId = %s\n",
            _isSender ? "OUT" : "IN",
            listingId.c_str()
        );
    }

    void OnOpenUserStore(PacketReader &reader) // TAG: 1B9D620
    {
        auto creatorHandle = reader.ReadString();

        printf("[%s] OnOpenUserStore:\n  creatorHandle = %s\n",
            _isSender ? "OUT" : "IN",
            creatorHandle.c_str()
        );
    }

    void OnOpenQuestCharcterDialog(PacketReader &reader) // TAG: 1B9D7D0
    {
        auto characterId = reader.ReadUUID();

        printf("[%s] OnOpenQuestCharcterDialog:\n  characterId = %s\n",
            _isSender ? "OUT" : "IN",
            characterId.c_str()
        );
    }

    void OnUIScriptableBarStart(PacketReader &reader) // TAG: 1B9D960
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

    void OnUIScriptableBarStopped(PacketReader &reader) // TAG: 1B9D9D0
    {
        auto barId = reader.ReadUint32();
        auto scriptEventId = reader.ReadUint64();
        auto completed = reader.ReadUint8();
    }

    void OnUIScriptableBarCancel(PacketReader &reader) // TAG: 1B9DA40
    {
        auto barId = reader.ReadUint32();
    }

    void OnUIHintTextUpdate(PacketReader &reader) // TAG: 1B9DC00
    {
        auto text = reader.ReadString();
    }

    void OnQuestOfferResponse(PacketReader &reader) // TAG: 1B9DDB0
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto accepted = reader.ReadUint8();
    }

    void OnQuestCompleted(PacketReader &reader) // TAG: 1B9DE20
    {
        auto questId = reader.ReadUUID();
        auto questDefinitionId = reader.ReadUUID();
        auto completedState = reader.ReadUint32();
    }

    void OnQuestRemoved(PacketReader &reader) // TAG: 1B9DE90
    {
        auto questId = reader.ReadUUID();
    }

    void OnShowWorldDetail(PacketReader &reader) // TAG: 1B9E020
    {
        auto sansarUri = reader.ReadString();
        auto show = reader.ReadUint8();

        printf("[%s] OnShowWorldDetail:\n  sansarUri = %s\n  show = %u\n",
            _isSender ? "OUT" : "IN",
            sansarUri.c_str(),
            show
        );
    }

    void OnShowTutorialHints(PacketReader &reader) // TAG: 1B9E090
    {
        auto tutorialHintEnum = reader.ReadUint32();
        auto variant = reader.ReadUint32();
    }

    void OnTutorialHintsEnabled(PacketReader &reader) // TAG: 1B9E100
    {
        auto enabled = reader.ReadUint8();
    }
};

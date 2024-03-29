#include <winsock2.h>
#include <ws2tcpip.h>

#include <Windows.h>
#include <detours.h>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <map>
#include <iostream>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ntdll")


static int (WINAPI *original_recvfrom)(SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen) = recvfrom;
static int (WINAPI *original_sendto)(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen) = sendto;
int (WINAPI *original_recv)(SOCKET s, char *buf, int len, int flags) = recv;
int (WINAPI *original_send)(SOCKET s, const char *buf, int len, int flags) = send;

std::string GetAddressFromAddr(const sockaddr *addr);

#include "bitReader.hpp"
#include "Messages.h"
#include "PacketReader.hpp"

#include "ClientKafka.h"
#include "AnimationComponent.h"
#include "GameWorld.h"
#include "AgentController.h"
#include "Audio.h"
#include "Simulation.h"
#include "Render.h"
#include "EditServer.h"
#include "ClientVoice.h"
#include "WorldState.h"
#include "RegionRegion.h"
#include "ClientRegion.h"
#include "Utils.hpp"

#include "Magic.h"

extern "C" {
    NTSYSAPI NTSTATUS NtQueryTimerResolution(OUT PULONG minimumResolution, OUT PULONG maximumResolution, OUT PULONG currentResolution);
    NTSYSAPI NTSTATUS NtSetTimerResolution(IN ULONG desiredResolution, IN BOOLEAN setResolution, OUT PULONG currentResoultion);
}


unsigned long long ReturnPoint_ProcessPacketRecv = 0;
unsigned long long ReturnPoint_ProcessPacketSend = 0;
unsigned long long ReturnPoint_GetPositionForVoiceServer = 0;
unsigned long long ReturnPoint_ProcessHttpBodyRecv = 0;
unsigned long long ReturnPoint_ProcessHttpSend = 0;
unsigned long long ReturnPoint_ProcessPositionUpdate = 0;
unsigned long long ReturnPoint_ProcessBodyCinfo = 0;

//float *AvatarPositionOffset = 0;
float *CameraPositionOffset = nullptr;

std::vector<float> currentTargetPosition = std::vector<float>({0.0f, 0.0f, 0.0f});

bool isFlyMode = false;
uint32_t myControllerId = UINT32_MAX;
uint32_t mySessionId = UINT32_MAX;
uint64_t myComponentId = UINT64_MAX;
uint64_t targetComponentId = UINT64_MAX;
uint64_t targetFollowComponentId = UINT64_MAX;
std::string targetHandle = "";

int followMode = 0;
bool isUsingTargetAsSpawnPosition = false;
bool knowsTargetPosition = false;

std::unordered_map<uint32_t, uint64_t> sessionToComponentIdMap;
std::unordered_map<std::string, uint32_t> handleToSessionIdMap;

uint16_t GetPortFromAddr(const sockaddr *addr)
{
    if (!addr)
    {
        return 0;
    }

    static std::map<uint64_t, uint16_t> addr_lookup;
    auto cached = addr_lookup.find((uint64_t)addr);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    int port = 0;

    if (addr->sa_family == AF_INET)
    {
        auto s = (sockaddr_in *)addr;
        port = ntohs(s->sin_port);
    }
    else
    {
        auto s = (sockaddr_in6 *)addr;
        port = ntohs(s->sin6_port);
    }

    return port;
}

std::string GetAddressFromAddr(const sockaddr *addr)
{
    if (!addr)
    {
        return "NULL";
    }

    static std::map<uint64_t, std::string> addr_lookup;
    auto cached = addr_lookup.find((uint64_t)addr);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    int port = 0;
    char address[INET6_ADDRSTRLEN];

    if (addr->sa_family == AF_INET)
    {
        auto s = (sockaddr_in *)addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, address, sizeof(address));
    }
    else
    {
        auto s = (sockaddr_in6 *)addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, address, sizeof(address));
    }

    auto result = std::string(address) + " port " + std::to_string(port);
    // addr_lookup[(uint64_t)addr] = result;

    return result;
}

uint16_t GetPortFromSocket(SOCKET s)
{
    static std::map<SOCKET, uint16_t> addr_lookup;
    auto cached = addr_lookup.find(s);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    sockaddr_storage addr = {};
    socklen_t namelen = sizeof(addr);

    auto result = getpeername(s, (sockaddr *)&addr, &namelen);

    auto port = GetPortFromAddr((const sockaddr *)&addr);
    return port;
}

std::string GetAddressFromSocket(SOCKET s)
{
    static std::map<SOCKET, std::string> addr_lookup;
    auto cached = addr_lookup.find(s);
    if (cached != addr_lookup.end())
    {
        return cached->second;
    }

    sockaddr_storage addr = {};
    socklen_t namelen = sizeof(addr);

    auto result = getpeername(s, (sockaddr *)&addr, &namelen);

    auto name = GetAddressFromAddr((const sockaddr *)&addr);
    addr_lookup[s] = name;
    return name;
}

int WINAPI Hooked_Recvfrom(SOCKET s, char *buff, int len, int flags, struct sockaddr *from, int *fromlen)
{
    auto result = original_recvfrom(s, buff, len, flags, from, fromlen);
    if (result == SOCKET_ERROR)
    {
        return result;
    }

    auto sin = (sockaddr_in6 *)from;
    auto port = htons(sin->sin6_port);


        printf("<-- RECVFROM port %d: [%d] %s\n", port, result, Utils::ArrayToHexString(buff, result).c_str());
        Utils::DumpPacket(buff, result, false, s, port);
 

    return result;
}

int WINAPI Hooked_Sendto(SOCKET s, const char *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
    //auto address = GetAddressFromSocket(s);
    
    auto sin = (sockaddr_in6 *)to;
    auto port = htons(sin->sin6_port);

    //char ip[INET6_ADDRSTRLEN];
    //inet_ntop(AF_INET, &sin->sin6_addr, ip, sizeof(ip));

    //printf("--> SENDTO port %d: [%d] %s\n", port, len, Utils::ArrayToHexString(buf, len).c_str());

    if (buf == nullptr || len < 1)
    {
        return original_sendto(s, buf, len, flags, to, tolen);
    }

    //if (buf[0] != 0x07)
    //{
    //    printf("SENDTO\n");
    //    Utils::DumpPacket(buf, len, true);
    //}
    //
    //if (buf[0] == 0x06)
    //{
    //    auto messageId = *((uint32_t *)&buf[1]);
    //}
    /*
    if (buf[0] == 0x07 || buf[0] == 0x08)
    {
        if (len > 10)
        {
            auto *messageId = (uint32_t *)&buf[4];
            if (*messageId == ClientRegionMessages::ClientRegionCommandMessage)
            {
                // this actually works \o/
                *((uint32_t *)&buf[4]) = ClientRegionMessages::RequestDropPortal;
                *((uint32_t *)&buf[8]) = *((uint32_t *)&buf[8]) - 4;
                *((uint32_t *)&buf[len-2-4]) = 0;
            }
        }
    }
    */
    Utils::DumpPacket(buf, len, true, s, port);
 
    auto result = original_sendto(s, buf, len, flags, to, tolen);
    return result;
}


int WINAPI Hooked_Recv(SOCKET s, char *buf, int len, int flags)
{
    auto result = original_recv(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        auto port = GetPortFromSocket(s);
        if (port != 443 && port != 80)
        {

            auto name = GetAddressFromSocket(s);
            Utils::DumpPacket2(name.c_str(), buf, result, false);

            if (buf[0] == 0xe8 || buf[0] == 0x29 || buf[0] == 0x26)
            {
                memset(buf, 0, result);
            }
        }
    }

    return result;
}


int WINAPI Hooked_Send(SOCKET s, const char *buf, int len, int flags)
{
    auto result = original_send(s, buf, len, flags);
    if (result != SOCKET_ERROR)
    {
        auto port = GetPortFromSocket(s);
        if (port != 443 && port != 80)
        {
            auto name = GetAddressFromSocket(s);
            Utils::DumpPacket2(name.c_str(), buf, len, true);
        }
    }

    return result;
}


void RewriteCode(void *targetAddress, uint8_t *newCode, std::size_t newCodeLength)
{
    DWORD oldProtection = 0;
    if (!VirtualProtect(targetAddress, newCodeLength, PAGE_EXECUTE_READWRITE, &oldProtection))
    {
        printf("Failed to VirtualProtect address...\n");
    }

    printf("Writing %lld bytes to %llX...\n", newCodeLength, (uint64_t)targetAddress);
    auto result = memcpy(targetAddress, newCode, newCodeLength);

    if (!VirtualProtect(targetAddress, newCodeLength, oldProtection, &oldProtection))
    {
        printf("Failed to restore VirtualProtect protection...\n");
    }
}

std::vector<std::unique_ptr<MessageHandler>> messageHandlers;
std::unique_ptr<MessageHandler> messageHandlerEditServer;

std::unordered_map<int, std::string> idToMessageMap = {
    {0x412484C4, "Audio.LoadSound"},
    {0x8FC77316, "Audio.PlaySound"},
    {0x6A2C4CEF, "Audio.PlayStream"},
    {0x866BF5CF, "Audio.StopBroadcastingSound"},
    {0x5DCD6123, "Audio.SetAudioStream"},
    {0xEC3CA8EC, "Audio.SetMediaSource"},
    {0x559B7F04, "Audio.PerformMediaAction"},
    {0x1A5C9610, "Audio.StopSound"},
    {0x20EDD0C4, "Audio.SetLoudness"},
    {0x7BB86A5B, "Audio.SetPitch"},
    {0x6951DAEC, "Render.LightStateChanged"},
    {0x0D094FEA, "Simulation.InitialTimestamp"},
    {0x1E9B31CE, "Simulation.Timestamp"},
    {0x86E6A7F6, "Simulation.SetWorldGravityMagnitude"},
    {0x864418DA, "Simulation.ActiveRigidBodyUpdate"},
    {0x0D938F45, "Simulation.RigidBodyDeactivated"},
    {0x45FAAEBC, "Simulation.RigidBodyPropertyChanged"},
    {0x3A92215C, "Simulation.RigidBodyDestroyed"},
    {0x00AC2B81, "AgentController.AgentPlayAnimation"},
    {0x0B617A9A, "AgentController.ExitSit"},
    {0x1651CD68, "AgentController.ObjectInteractionPromptUpdate"},
    {0xBB086E9B, "AgentController.ObjectInteractionCreate"},
    {0xE5321C47, "AgentController.RequestSitOnObject"},
    {0x191F08C0, "AgentController.SitOnObject"},
    {0x09DD53F6, "AgentController.SetAgentFiltersBody"},
    {0x2B87F09D, "AgentController.RequestSetAgentFiltersBody"},
    {0x31D1EC43, "AgentController.SetCharacterUserProperty"},
    {0x158B2580, "AgentController.CreateSpeechGraphicsPlayer"},
    {0x2C21850D, "AgentController.RequestSpawnItem"},
    {0xEB3C4296, "AgentController.RequestDeleteLatestSpawn"},
    {0x3EB3EDF7, "AgentController.RequestDeleteAllSpawns"},
    {0x2DF35CF3, "AgentController.ControlPoint"},
    {0x75C0AC6B, "AgentController.WarpCharacter"},
    {0x25C093E0, "AgentController.RequestWarpCharacter"},
    {0xFCA3EF20, "AgentController.CharacterControlPointInput"},
    {0x8FB6F456, "AgentController.CharacterControlPointInputReliable"},
    {0x3D490CAB, "AgentController.CharacterControllerInput"},
    {0xA7D6EFD1, "AgentController.CharacterControllerInputReliable"},
    {0x982B98D8, "AgentController.RequestAgentPlayAnimation"},
    {0x5489A347, "AgentController.RequestBehaviorStateUpdate"},
    {0x85BA6E75, "AgentController.AttachToCharacterNode"},
    {0x80F90328, "AgentController.DetachFromCharacterNode"},
    {0x67B63AA3, "AgentController.RequestDetachFromCharacterNode"},
    {0x645C4976, "AgentController.SetCharacterNodePhysics"},
    {0x83F1D7DB, "AgentController.WarpCharacterNode"},
    {0xBB382C6B, "AgentController.CharacterIKBone"},
    {0xE945D8B8, "AgentController.CharacterIKPose"},
    {0x4C3B3B4B, "AgentController.CharacterIKBoneDelta"},
    {0x893A18BE, "AgentController.CharacterIKPoseDelta"},
    {0xA25F81AB, "AgentController.ObjectInteraction"},
    {0x17B7D18A, "AgentController.ObjectInteractionUpdate"},
    {0x6F5546CE, "AgentController.UserReaction"},
    {0xAE522F17, "GameWorld.StaticMeshFlagsChanged"},
    {0xCA6CCC08, "GameWorld.StaticMeshScaleChanged"},
    {0xD22C9D73, "GameWorld.Timestamp"},
    {0xEFC20B7F, "GameWorld.MoveEntity"},
    {0x403D5704, "GameWorld.ChangeMaterialVectorParam"},
    {0x4F20B073, "GameWorld.ChangeMaterialFloatParam"},
    {0x45C605B8, "GameWorld.ChangeMaterial"},
    {0x3F020C77, "GameWorld.RiggedMeshFlagsChange"},
    {0xEA2934E8, "GameWorld.RiggedMeshScaleChanged"},
    {0x60C955C0, "GameWorld.ScriptCameraMessage"},
    {0x575D5715, "GameWorld.ScriptCameraCapture"},
    {0x371D99C1, "GameWorld.UpdateRuntimeInventorySettings"},
    {0x513700E2, "RegionRegion.DynamicSubscribe"},
    {0xE87C89BB, "RegionRegion.DynamicPlayback"},
    {0x5A4AFA33, "RegionRegion.MasterFrameSync"},
    {0xBB5865E8, "RegionRegion.AgentControllerMapping"},
    {0x685B436C, "WorldState.CreateWorld"},
    {0x20C45982, "WorldState.DestroyWorld"},
    {0x065C105B, "WorldState.RigidBodyComponentInitialState"},
    {0xDE87FDD8, "WorldState.AnimationComponentInitialState"},
    {0xA5C4FB23, "WorldState.LoadClusterDefinition"},
    {0x941E6445, "WorldState.ComponentRelativeTransform"},
    {0x349AD257, "WorldState.InitiateCluster"},
    {0x73810D53, "WorldState.CreateClusterViaDefinition"},
    {0x2926D248, "WorldState.DestroyCluster"},
    {0x5749A1CD, "WorldState.DestroyObject"},
    {0x1505C6D8, "WorldState.DestroySourceIdSpace"},
    {0x32DC63D7, "WorldState.CreateCharacterNode"},
    {0xF555FE2D, "WorldState.CreateAgentController"},
    {0x16406FB7, "WorldState.DestroyAgentController"},
    {0x0AF50C12, "ClientKafka.FriendResponseLoaded"},
    {0x5915FBFE, "ClientKafka.PresenceUpdateFanoutLoaded"},
    {0xB4AB87F5, "ClientKafka.FriendTableLoaded"},
    {0x0A7562A7, "ClientKafka.RelationshipTableLoaded"},
    {0x4B73CF2C, "ClientKafka.PrivateChatLoaded"},
    {0x9BC4EF8A, "ClientKafka.PrivateChatStatusLoaded"},
    {0xD3CAA979, "ClientKafka.ScriptRegionConsoleLoaded"},
    {0x4AC30FE7, "ClientKafka.ClientMetric"},
    {0xDCF900A4, "ClientKafka.RegionHeartbeatMetric"},
    {0xBA6DB2FC, "ClientKafka.RegionEventMetric"},
    {0x3BFA4474, "ClientKafka.SubscribeScriptRegionConsole"},
    {0xD49B04C3, "ClientKafka.UnsubscribeScriptRegionConsole"},
    {0x00B0E15E, "ClientKafka.ScriptConsoleLog"},
    {0x46C5FDF3, "ClientKafka.LongLivedNotification"},
    {0x59CF6950, "ClientKafka.LongLivedNotificationDelete"},
    {0x3494608D, "ClientKafka.LongLivedNotificationsLoaded"},
    {0xAD589C6F, "ClientKafka.ShortLivedNotification"},
    {0x0C0C9D81, "ClientKafka.Login"},
    {0xA685E82B, "ClientKafka.LoginReply"},
    {0x08445006, "ClientKafka.EnterRegion"},
    {0xE4ADC2EB, "ClientKafka.LeaveRegion"},
    {0x304D3746, "ClientKafka.RegionChat"},
    {0x2DC9B029, "ClientKafka.PrivateChat"},
    {0x955C35EB, "ClientKafka.PrivateChatStatus"},
    {0x1DB989E8, "ClientKafka.PresenceUpdate"},
    {0xA356B3ED, "ClientKafka.FriendRequest"},
    {0x14FFCD37, "ClientKafka.FriendRequestStatus"},
    {0xE24EBDD3, "ClientKafka.FriendResponse"},
    {0x22565685, "ClientKafka.FriendResponseStatus"},
    {0x203CC0A8, "ClientKafka.FriendTable"},
    {0x650939F7, "ClientKafka.RelationshipOperation"},
    {0x078DCC26, "ClientKafka.RelationshipTable"},
    {0xA2190F5D, "ClientKafka.InventoryItemCapabilities"},
    {0xE3466906, "ClientKafka.InventoryItemRevision"},
    {0xD7C7DC26, "ClientKafka.InventoryItemUpdate"},
    {0xB11C8C84, "ClientKafka.InventoryItemDelete"},
    {0x75BAFB95, "ClientKafka.InventoryLoaded"},
    {0xF5361468, "ClientKafka.FriendRequestLoaded"},
    {0x59AC5555, "ClientVoice.Login"},
    {0xA6972017, "ClientVoice.LoginReply"},
    {0x5A978A32, "ClientVoice.AudioData"},
    {0xD9306963, "ClientVoice.SpeechGraphicsData"},
    {0x0D50D087, "ClientVoice.LocalAudioData"},
    {0xF2FB6AD0, "ClientVoice.LocalAudioStreamState"},
    {0x1798BA9C, "ClientVoice.LocalAudioPosition"},
    {0x56800096, "ClientVoice.LocalAudioMute"},
    {0x573EE089, "ClientVoice.LocalSetRegionBroadcasted"},
    {0x90DA7ED3, "ClientVoice.LocalSetMuteAll"},
    {0x47C4FFDF, "ClientVoice.GroupAudioData"},
    {0xC91B2D1C, "ClientVoice.LocalTextData"},
    {0x88C28A79, "ClientVoice.MasterInstance"},
    {0x3F7171FB, "ClientVoice.VoiceModerationCommand"},
    {0x742CE528, "ClientVoice.VoiceModerationCommandResponse"},
    {0x3A168D81, "ClientVoice.VoiceNotification"},
    {0x3902800A, "ClientRegion.UserLogin"},
    {0x30CDBED6, "ClientRegion.UserLoginReply"},
    {0xF6B9093E, "ClientRegion.AddUser"},
    {0x0A7FC621, "ClientRegion.RemoveUser"},
    {0xC67C58F7, "ClientRegion.RenameUser"},
    {0x083642BD, "ClientRegion.ChatMessageToClient"},
    {0xDDDEC199, "ClientRegion.ChatMessageToServer"},
    {0xD6F4CF23, "ClientRegion.SetAgentController"},
    {0x41FE0612, "ClientRegion.DebugTimeChangeToServer"},
    {0x0741CA9B, "ClientRegion.VisualDebuggerCaptureToServer"},
    {0xF8E77C8E, "ClientRegion.ClientStaticReady"},
    {0x575AC239, "ClientRegion.ClientDynamicReady"},
    {0xECE56EFD, "ClientRegion.ClientRegionCommandMessage"},
    {0x7D22C30C, "ClientRegion.RequestDropPortal"},
    {0x0D3809EB, "ClientRegion.VibrationPulseToClient"},
    {0x5C7CC1FC, "ClientRegion.TeleportTo"},
    {0x2BDBDB56, "ClientRegion.TeleportToUri"},
    {0x706F63FB, "ClientRegion.TeleportToEditMode"},
    {0x5178DC5E, "ClientRegion.DebugTimeChangeToClient"},
    {0xF66AD9BF, "ClientRegion.VisualDebuggerCaptureToClient"},
    {0x88023C72, "ClientRegion.ScriptModalDialog"},
    {0xB34F3A45, "ClientRegion.ScriptModalDialogResponse"},
    {0x981AB0D6, "ClientRegion.TwitchEventSubscription"},
    {0x28F54053, "ClientRegion.TwitchEvent"},
    {0xB4E1AB7B, "ClientRegion.InitialChunkSubscribed"},
    {0x4B68A51C, "ClientRegion.ClientKickNotification"},
    {0x58003034, "ClientRegion.ClientSmiteNotification"},
    {0x6188A537, "ClientRegion.ClientMuteNotification"},
    {0x7E28AEAF, "ClientRegion.ClientVoiceBroadcastStartNotification"},
    {0xC33DE58B, "ClientRegion.ClientVoiceBroadcastStopNotification"},
    {0x9643B9C3, "ClientRegion.ClientRuntimeInventoryUpdatedNotification"},
    {0x87341F77, "ClientRegion.ClientSetRegionBroadcasted"},
    {0xABDA80C7, "ClientRegion.SubscribeCommand"},
    {0xA36E9F9C, "ClientRegion.UnsubscribeCommand"},
    {0xB87F9C66, "ClientRegion.ClientCommand"},
    {0x05C1A8D7D, "ClientRegion.OpenStoreListing"},
    {0x53078A1E, "ClientRegion.OpenUserStore"},
    {0x4221836F, "ClientRegion.OpenQuestCharacterDialog"},
    {0x036164050, "ClientRegion.UIScriptableBarStart"},
    {0xBAFD799D, "ClientRegion.UIScriptableBarStopped"},
    {0x604E18DE, "ClientRegion.UIScriptableBarCancel"},
    {0x64225637, "ClientRegion.UIHintTextUpdate"},
    {0x4DB48E35, "ClientRegion.QuestOfferResponse"},
    {0xE1EE5F5D, "ClientRegion.QuestCompleted"},
    {0x34793AB0, "ClientRegion.QuestRemoved"},
    {0x5F483F0C, "ClientRegion.ShowWorldDetail"},
    {0x581827CC, "ClientRegion.ShowTutorialHint"},
    {0xE4C496DF, "ClientRegion.TutorialHintsSetEnabled"},
    { 0x01753788, "ClientRegion.ReactionDefinition" }, // NEW: 2021-03-25
    { 0xFA87F231, "ClientRegion.SystemReactionDefinition" }, // NEW: 2021-03-25
    { 0x9B5B20E9, "ClientRegion.UpdateReactions" }, // NEW: 2021-03-25
    { 0x28323E96, "ClientRegion.AddReaction" }, // NEW: 2021-03-25
    { 0x3F337471, "ClientRegion.RemoveReaction" }, // NEW: 2021-03-25
    { 0xDE4E07ED, "ClientRegion.UIScriptableScoreBoard" }, // NEW: 2023-06-13
    {0x046D3C1E, "EditServer.UserLogin"},
    {0xE227C3E2, "EditServer.UserLoginReply"},
    {0x50155562, "EditServer.AddUser"},
    {0x5729AC25, "EditServer.RemoveUser"},
    {0xBC512F47, "EditServer.OpenWorkspace"},
    {0x43C06583, "EditServer.CloseWorkspace"},
    {0x76363E28, "EditServer.EditWorkspaceCommand"},
    {0x7C7BDCA8, "EditServer.SaveWorkspace"},
    {0xFAE838FC, "EditServer.SaveWorkspaceReply"},
    {0x5963934F, "EditServer.BuildWorkspace"},
    {0xF12FD324, "EditServer.UpdateWorkspaceClientBuiltBakeData"},
    {0x15B220E0, "EditServer.BuildWorkspaceCompileReply"},
    {0xC9FCDB71, "EditServer.BuildWorkspaceProgressUpdate"},
    {0xF090AF8E, "EditServer.BuildWorkspaceUploadReply"},
    {0x7D87DBEA, "EditServer.WorkspaceReadyReply"},
    {0x7E37F335, "EditServer.SaveWorkspaceSelectionToInventory"},
    {0x439C3637, "EditServer.SaveWorkspaceSelectionToInventoryReply"},
    {0xB5487205, "EditServer.InventoryCreateItem"},
    {0x6F75848E, "EditServer.InventoryDeleteItem"},
    {0x690612C6, "EditServer.InventoryChangeItemName"},
    {0x6C202756, "EditServer.InventoryChangeItemState"},
    {0x2C2AE45E, "EditServer.InventoryModifyItemThumbnailAssetId"},
    {0xF93582DD, "EditServer.InventoryModifyItemCapabilities"},
    {0xA67D88F8, "EditServer.InventorySaveItem"},
    {0x144D39F8, "EditServer.InventoryUpdateItemReply"},
    {0xF2E11A50, "EditServer.InventoryItemUpload"},
    {0xA25566F4, "EditServer.InventoryItemUploadReply"},
    {0xBE2C532E, "EditServer.InventoryCreateListing"},
    {0x4EA3D072, "EditServer.InventoryCreateListingReply"},
    {0xB5BFECD3, "EditServer.BeginEditServerSpawn"},
    {0xB3623297, "EditServer.EditServerSpawnReady"},
    {0x0B3B7D2E, "AnimationComponent.FloatVariable"},
    {0x4C1B3DF2, "AnimationComponent.FloatNodeVariable"},
    {0x91419DEB, "AnimationComponent.FloatRangeNodeVariable"},
    {0x23314E53, "AnimationComponent.VectorVariable"},
    {0x0CC9F1B8, "AnimationComponent.QuaternionVariable"},
    {0xC11AFDE7, "AnimationComponent.Int8Variable"},
    {0xA67454F0, "AnimationComponent.BoolVariable"},
    {0xAB2F1EB1, "AnimationComponent.CharacterTransform"},
    {0x970F93D4, "AnimationComponent.CharacterTransformPersistent"},
    {0x53A4BF26, "AnimationComponent.CharacterAnimationDestroyed"},
    {0x8C738C9E, "AnimationComponent.AnimationOverride"},
    {0xCE9B5148, "AnimationComponent.BehaviorInternalState"},
    {0x16C090B1, "AnimationComponent.CharacterBehaviorInternalState"},
    {0x217192BE, "AnimationComponent.BehaviorStateUpdate"},
    {0x7846436E, "AnimationComponent.BehaviorInitializationData"},
    {0x51A1705A, "AnimationComponent.CharacterSetPosition"},
    {0x009385A0, "AnimationComponent.PlayAnimation"},
};

void ProcessPacketSend(uint8_t *packet, uint64_t length)
{
    //printf("ProcessPacketSend: Length = %lld | packet = %llX\n", length, *(uint64_t)packet);

    try
    {
        PacketReader reader(packet, length);
        auto messageId = reader.ReadUint32();
    
        if (messageId == 0)
        {
            auto unknown = reader.ReadUint32();
            auto channel = reader.ReadString();
            auto numPackets = reader.ReadUint32();

            printf("-----------------------\n");
            printf("Channel: %s\nPacket versions (%d):\n",
                channel.c_str(),
                numPackets
            );

            std::vector<uint32_t> packetVersions;
            for (size_t i = 0; i < numPackets; i++)
            {
                auto id = reader.ReadUint32();
                auto version = reader.ReadUint16();

                printf("  %08X = %d\n", id, version);
            }
            printf("-----------------------\n");

            return;
        }

        auto handled_packet = false;

        //if (messageId != 0xE945D8B8 && messageId != 0xFCA3EF20 && messageId != 0x8FB6F456 && messageId != 0x217192BE && messageId != 0x3D490CAB && messageId != 0xA7D6EFD1 && messageId != 0xAB2F1EB1  &&messageId != 0x970F93D4) 
        //{
        //    auto foundMessage = idToMessageMap.find(messageId);
        //    if (foundMessage != idToMessageMap.end())
        //    {
        //        printf("[OUT] %s [%08X]\n", foundMessage->second.c_str(), foundMessage->first);
        //    }
        //
        //}

        for (auto &item : messageHandlers)
        {
                // printf("SendMessage %08X\n",
                //    messageId
                //);

            if (item->OnMessage(messageId, reader, true))
            {
                handled_packet = true;
                break;
            }
        }
    
        if (!handled_packet)
        {
            printf("ProcessPacketSend: Unhandled message %X\n", messageId);
            Utils::DumpPacket((const char *)packet, length, true);
        }
    }
    catch (std::exception ex)
    {
        printf("!!! ProcessPacketSend Caught exception -> %s\n", ex.what());
    }
}


void GetPositionForVoiceServer(float *position)
{
   // printf("GetPositionForVoiceServer: %f, %f, %f\n", position[0], position[1], position[2]);

    try
    {
        memcpy(position, CameraPositionOffset, 3 * sizeof(float));
    }
    catch (std::exception ex)
    {
        printf("!!! GetPositionForVoiceServer Caught exception -> %s\n", ex.what());
    }
}

void ProcessPacketSendB(uint8_t *packet, uint64_t length)
{
    //printf("ProcessPacketSend: Length = %lld | packet = %llX\n", length, *(uint64_t)packet);

    try
    {
        PacketReader reader(packet, length);
        auto messageId = reader.ReadUint32();
    
        if (messageId == 0)
        {
            auto unknown = reader.ReadUint32();
            auto channel = reader.ReadString();
            auto numPackets = reader.ReadUint32();

            printf("-----------------------\n");
            printf("Channel: %s\nPacket versions (%d):\n",
                channel.c_str(),
                numPackets
            );

            std::vector<uint32_t> packetVersions;
            for (size_t i = 0; i < numPackets; i++)
            {
                auto id = reader.ReadUint32();
                auto version = reader.ReadUint16();

                printf("  %08X = %d\n", id, version);
            }
            printf("-----------------------\n");

            return;
        }

        messageHandlerEditServer->OnMessage(messageId, reader, true);
    }
    catch (std::exception ex)
    {
        printf("!!! ProcessPacketSendB Caught exception -> %s\n", ex.what());
    }
}


void ProcessPacketRecv(uint64_t messageId, uint8_t *packet, uint64_t length)
{
  //   printf("ProcessPacketRecv: MessageId = %llX | Length = %lld | packet = %llX\n", messageId, length, (uint64_t)packet);

    try
    {
        PacketReader reader(packet, length);
        messageId = reader.ReadUint32();

        auto handled_packet = false;

       // if (messageId == ClientKafkaMessages::InventoryItemUpdate || messageId == ClientKafkaMessages::InventoryLoaded) {
       //     return;
       // }

        //auto foundMessage = idToMessageMap.find(messageId);
        //if (foundMessage != idToMessageMap.end())
        //{
        //    printf("[IN] %s [%08X]\n", foundMessage->second.c_str(), foundMessage->first);
        //}

        for (auto &item : messageHandlers)
        {
            if (item->OnMessage(messageId, reader, false))
            {
                handled_packet = true;
                break;
            }
        }

        if(!handled_packet)
        {
            printf("ProcessPacketRecv: Unhandled message %llX\n", messageId);
            Utils::DumpPacket((const char*)packet, length, false);
        }

        // "Illegally formatted message received. You probably failed to bind a callback for this message. Message was: %s (id: 0x%x)\n"'
    }
    catch (std::exception ex)
    {
        printf("!!! Caught exception -> %s\n", ex.what());
    }
}

void ProcessHttpBodyRecv(char *packet, uint64_t length)
{
    length = length > 9999 ? 9999 : length;

    try
    {
        //if (strstr(packet, "Content-Type: image") == nullptr) {
        //    return;
        //}
        for (size_t i = 0; i < length; i++)
        {
            uint8_t ch = (uint8_t)packet[i];
            if (ch != '\r' && ch != '\n' && ch != 0 && !isprint(ch)) {
                length = i;
                break;
            }
        }


        auto bodyResponse = std::string(packet, length);
        printf("[IN] ProcessHttpBodyRecv:\n%s\n",
            bodyResponse.c_str()
        );
    }
    catch (std::exception ex)
    {
        printf("!!! Caught exception -> %s\n", ex.what());
    }
}

void ProcessHttpSend(char *packet, uint64_t length)
{
    try
    {
        auto bodyResponse = std::string(packet, length);
        printf("[OUT] ProcessHttpBodySend:\n%s\n",
            bodyResponse.c_str()
        );
    }
    catch (std::exception ex)
    {
        printf("!!! Caught exception -> %s\n", ex.what());
    }
}


void ProcessBodyCinfo(uint8_t *clusterPtrAtBodyCInfo, uint8_t *realBodyCinfo, uint64_t length)
{
    if (length > 0x40)
    {
        auto bodyType = &realBodyCinfo[0x40];

        PacketReader reader(clusterPtrAtBodyCInfo + length, 1024);
        
        auto materialVersion = reader.ReadUint32();
        auto materialCount = reader.ReadUint32();
       // if (materialCount > 0) {
       //     printf("materialCount = %d\n", materialCount);
       //     return;
       // }

        // Not sure if this is actually shapeuuidptr. need to double check. Whatever it is, it's something we can use
        auto shapeUuidPtr = reader.ReadUUID();
        //auto audioMaetrialUuid = reader.ReadUUID();
        //
        //printf("shapeUuidPtr = %s\n", shapeUuidPtr.c_str());
        //printf("audioMaetrialUuid = %s\n", audioMaetrialUuid.c_str());
        //auto pCanGrab = (bool *)reader.GetCurrentPointer();
        //reader.ReadUint32();
        //
        //auto grabPointDefsLen = reader.ReadUint32();
        //if (grabPointDefsLen != 0)
        //{
        //    printf("grabPointDefsLen = %d\n", grabPointDefsLen);
        //    return;
        //}
        //
        //auto AudioResourcePoolSoundsLen = reader.ReadUint32();
        //if (AudioResourcePoolSoundsLen != 0)
        //{
        //    printf("AudioResourcePoolSoundsLen = %d\n", AudioResourcePoolSoundsLen);
        //    return;
        //}
        //
        //auto SitPointDefinitionsLen = reader.ReadUint32();
        //if (SitPointDefinitionsLen != 0)
        //{
        //    printf("SitPointDefinitionsLen = %d\n", SitPointDefinitionsLen);
        //    return;
        //}

        //auto pCanRide = (bool *)reader.GetCurrentPointer();
        //reader.ReadUint32();
        //
        //printf("pCanGrab = %d\n", *pCanGrab);
        //printf("pCanRide = %d\n", *pCanRide);
        //
        //*pCanRide = 1;
        //*pCanGrab = 1;

        // c679963060d4ed71c457a03f90747cfb
        // 9bb11f9fbd317048b1353576ca290277
        // 4d014c542da8c8cd18ce9631b581a77a
        // 04f97845facf7ea404bd0e371827fc2d
      // if (shapeUuidPtr == "176327e45ff2c3276466c3e492a1eb27")
       {
           printf("Updating bodytype to dynamic...\n");
           *bodyType = 1; // lol
       }
       // 
       ////if (shapeUuidPtr == "c679963060d4ed71c457a03f90747cfb")
       ////{
       ////    printf("Updating bodytype to dynamic...\n");
       ////    *bodyType = 1; // lol
       ////}
       ////if (shapeUuidPtr == "9bb11f9fbd317048b1353576ca290277")
       ////{
       ////    printf("Updating bodytype to dynamic...\n");
       ////    *bodyType = 1; // lol
       ////}
       ////if (shapeUuidPtr == "4d014c542da8c8cd18ce9631b581a77a")
       ////{
       ////    printf("Updating bodytype to dynamic...\n");
       ////    *bodyType = 1; // lol
       ////}
       ////if (shapeUuidPtr == "04f97845facf7ea404bd0e371827fc2d")
       ////{
       ////    printf("Updating bodytype to dynamic...\n");
       ////    *bodyType = 1; // lol
       ////}

        printf("Shape: %s BodyType = %d\n", shapeUuidPtr.c_str(), *bodyType);
        // 0 = static
        // 1 = keyframe
        // 2 = dynamic 

        //if (*bodyType == 0)
        {
           // *bodyType = 2; // lol
        }
    }
}

void ProcessPositionUpdate(float *avatarPositionOffset)
{
    AvatarPositionOffset = avatarPositionOffset;

    if (CameraPositionOffset != nullptr && AvatarPositionOffset != nullptr && isFlyMode)
    {
        if (GetAsyncKeyState('Q'))
        {
            AvatarPositionOffset[0] = CameraPositionOffset[0];
            AvatarPositionOffset[1] = CameraPositionOffset[1];
            AvatarPositionOffset[2] = CameraPositionOffset[2];
        }
    }
}

#include <timeapi.h>
#pragma comment(lib, "Winmm.lib")

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        FILE *pFile = nullptr;
        AllocConsole();
        freopen_s(&pFile, "CONOUT$", "w", stdout);
        timeBeginPeriod(1);

        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");
        printf("++++++++++++++++++++++++++++++++++\n");

        messageHandlers.emplace_back(std::make_unique<AgentController>());
        messageHandlers.emplace_back(std::make_unique<AnimationComponent>());
        messageHandlers.emplace_back(std::make_unique<Audio>());
        messageHandlers.emplace_back(std::make_unique<ClientKafka>());
        messageHandlers.emplace_back(std::make_unique<ClientRegion>());
        messageHandlers.emplace_back(std::make_unique<ClientVoice>());
        messageHandlers.emplace_back(std::make_unique<EditServer>());
        messageHandlers.emplace_back(std::make_unique<GameWorld>());
        messageHandlers.emplace_back(std::make_unique<RegionRegion>());
        messageHandlers.emplace_back(std::make_unique<Render>());
        messageHandlers.emplace_back(std::make_unique<Simulation>());
        messageHandlers.emplace_back(std::make_unique<WorldState>());

        messageHandlerEditServer = std::make_unique<EditServer>();

        auto base = Utils::GetBaseAddress();

        if (true)
        {
            // Search for "Illegally formatted message received. You probably failed to bind a callback for this message. Message was: %s (id: 0x%x)\n"'
            // Scan down 3 jumps. Patch after last jump containing just a mov mov mov call [rax + 8]
            //hijack_ProcessPacketRecv  intercept_ProcessPacketRecv
            // 49 8B 7E 08 45 8B 46 10 48 8B D7 ff 50 08

            uint8_t hijack_ProcessPacketRecv[] = {
                0x48, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RDX, [address]
                0xFF, 0xE2,                                                  // JMP RDX  
                0x90,                                                        // NOP
                0x90,                                                        // NOP
            };

            const auto offset = 0x1523132;
            *((uint64_t *)&hijack_ProcessPacketRecv[2]) = (uint64_t)intercept_ProcessPacketRecv;
            RewriteCode(base + offset, hijack_ProcessPacketRecv, sizeof(hijack_ProcessPacketRecv));

            // TODO: May be outdated...
            /// WARNING
            /// WARNING
            //      Return point will not be directly after our injected code, but instead follow the existing jmp that we overwrote
            /// WARNING
            /// WARNING

            ReturnPoint_ProcessPacketRecv = (uint64_t)(base + offset + sizeof(hijack_ProcessPacketRecv));
        }

        // OLD JUNK
        if (true) //true)
        {
            // Search for "OutgoingPacket"
            // next call, call qword ptr[rax+18] or whatever
            // jne-call-je-je 
            // rva at mov qword ptr[rsp+68], r15
            // 
            // intercept_ProcessPacketSend hijack_ProcessPacketSend
            // 4C 89 7C 24 68 48 8D 88 98 17 00 00 48 8B D3 4C 8B FB
        
            /// WARNING
            /// WARNING
            //       not immediately at the entrypoint with the two movs and setting up the stackframe, but rather down 3 jumps (mov [rsp+68], r15)
            /// WARNING
            /// WARNING
        
            uint8_t hijack_ProcessPacketSend[] = {
                0x48, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RDX, [address]
                0xFF, 0xE2                                                   // JMP RDX           
            };
        
            const auto offset = 0x138DAB6;
            *((uint64_t *)&hijack_ProcessPacketSend[2]) = (uint64_t)intercept_ProcessPacketSend;
            RewriteCode(base + offset, hijack_ProcessPacketSend, sizeof(hijack_ProcessPacketSend));
        
            ReturnPoint_ProcessPacketSend = (uint64_t)(base + offset + sizeof(hijack_ProcessPacketSend));
        }


        // 2023 - BROKEN 'send packet' hook that reads editserver stuff as well (needs to be updated)
        //if (false)
        //{
        //    // CC CC CC CC 48 8B 89 F8 00 00 00 48 85 C9 74 07 48 8B 01 48 FF 60 50 32 C0 C3 CC CC
        //    // one of those 5 or so line functions between 'ClientConnection::giveTime' and 'Connect: '
        //
        //    uint8_t hijack_ProcessPacketSendB[] = {
        //        0x52,                                                        // PUSH RDX
        //        0x48, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RDX, [address]
        //        0xFF, 0xE2,                                                  // JMP RDX           
        //     //   0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,   // 9x NOP
        //    };
        //
        //    *((uint64_t *)&hijack_ProcessPacketSendB[3]) = (uint64_t)intercept_ProcessPacketSendB;
        //    RewriteCode(base + 0x15241B0, hijack_ProcessPacketSendB, sizeof(hijack_ProcessPacketSendB));
        //}


        //if (false)  // OLD http recv
        //{
        //    // 48 8B C3 48 8B 9C 24 08 01 00 00 48 81 C4 E8 00 00 00 41 5D 5D C3 ??

        //    // search for ": consume: parser error"
        //    // scan down until the first RET, trace back up to first CALL. patch after the call
        //    uint8_t hijack_ProcessHttpBodyRecv[] = {
        //        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RAX, [address]
        //        0xFF, 0xE0,                                                  // JMP RAX           
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90                                                         // NOP
        //    };

        //    *((uint64_t *)&hijack_ProcessHttpBodyRecv[2]) = (uint64_t)intercept_ProcessHttpBodyRecv;
        //    RewriteCode(base + 0x13AABDE, hijack_ProcessHttpBodyRecv, sizeof(hijack_ProcessHttpBodyRecv));

        //    ReturnPoint_ProcessHttpBodyRecv = (uint64_t)(base + 0x13AABDE + sizeof(hijack_ProcessHttpBodyRecv));
        //}

        //if (0) // always false
        //{
        //    // 48 3B 7C 24 60 4C 8B F7 4C 0F 4C 74 24 60 45 33 FF 4C 89 74 24 60 48 8B 5C 24 58
        //    // NOT UPDATEd
        //    // search for "%.*s"
        //    // scan up until you get to the double call (with a jmp inbetween) (should be past the third call above the string).
        //    // patch after the double call
        //    uint8_t hijack_ProcessHttpSend[] = {
        //        0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RBX, [address]
        //        0xFF, 0xE3,                                                  // JMP RBX           
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90                                                         // NOP
        //    };
        //
        //    *((uint64_t *)&hijack_ProcessHttpSend[2]) = (uint64_t)intercept_ProcessHttpSend;
        //    RewriteCode(base + 0x13A979E, hijack_ProcessHttpSend, sizeof(hijack_ProcessHttpSend));
        //
        //    ReturnPoint_ProcessHttpSend = (uint64_t)(base + 0x13A979E + sizeof(hijack_ProcessHttpSend));
        //}



        //// NOTUPDATED OCT 2022
        //if (0) // Hack to force the logging flag for HTTP logs?
        //{
        //    // 48 8B 88 B8 0F 00 00 48 C1 E9 25 F6 C1 01 0F 84 7D 01 00 00
        //    // search for "%.*s". There's some condition nearby which skips and referrs to "HttpClient" something
        //    uint8_t force_loghttp[] = {
	    //        0x48, 0x33, 0xC9,                          // XOR RCX, RCX
        //        0x48, 0xC1, 0xE1, 0x25,                    // SHL RCX, 0x25
        //        0x48, 0x09, 0x88, 0xB8, 0x0F, 0x00, 0x00,  // MOV [RAX + 0xFB8], RCX
        //        0x90,                                      // NOP
        //        0x90,                                      // NOP
        //        0x90,                                      // NOP
        //        0x90,                                      // NOP
        //        0x90,                                      // NOP
        //        0x90                                       // NOP
        //    };
        //
        //    RewriteCode(base + 0x13A98C9, force_loghttp, sizeof(force_loghttp));
        //}
        
        //if (false)  // DELETEME
        //{
        //    // 48 8B C3 48 8B 9C 24 08 01 00 00 48 81 C4 E8 00 00 00 41 5D 5D C3 ??

        //    // search for " first 0x%x"
        //    // up 4 or so lines.  "jle cmp jle <MOV R8> jmp"
        //    // RAX = messageLength
        //    // [RSI+2C8] = message

        //    uint8_t hijack_ProcessHttpBodyRecv[] = {
        //        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RAX, [address]
        //        0xFF, 0xE0,                                                  // JMP RAX           
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90,                                                        // NOP
        //        0x90                                                         // NOP
        //    };

        //    *((uint64_t *)&hijack_ProcessHttpBodyRecv[2]) = (uint64_t)intercept_ProcessHttpBodyRecv;
        //    RewriteCode(base + 0x13AABDE, hijack_ProcessHttpBodyRecv, sizeof(hijack_ProcessHttpBodyRecv));

        //    ReturnPoint_ProcessHttpBodyRecv = (uint64_t)(base + 0x13AABDE + sizeof(hijack_ProcessHttpBodyRecv));
        //}
        
        // if (0)  // NEW process http recv (not updated 2023-02)
        // {
        //     // 138E8E0
        // 
        //     // search for "HttpClient::fireHandler" top of function, after the first je
        //     // R14 = full response length
        //     // R15 = full response
        //     // 
        //     // R11 = content body ?
        //     // 
        //     // [RSI+2C8] = message
        // 
        //     // 00007FF75F12E8E0 | 48:83BF 78010000 00      | cmp qword ptr ds:[rdi+178],0                   |
        //     // 00007FF75F12E8E8 | 48:899C24 30010000       | mov qword ptr ss:[rsp+130],rbx                 |
        // 
        //     uint8_t hijack_ProcessHttpBodyRecv[] = {
        //         0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RAX, [address]
        //         0xFF, 0xE0,                                                  // JMP RAX           
        //         0x90,                                                        // NOP
        //         0x90,                                                        // NOP
        //         0x90,                                                        // NOP
        //         0x90,                                                        // NOP
        //     };
        // 
        //     *((uint64_t *)&hijack_ProcessHttpBodyRecv[2]) = (uint64_t)intercept_ProcessHttpBodyRecv;
        //     RewriteCode(base + 0x138F3B7, hijack_ProcessHttpBodyRecv, sizeof(hijack_ProcessHttpBodyRecv));
        // 
        //     ReturnPoint_ProcessHttpBodyRecv = (uint64_t)(base + 0x138F3B7 + sizeof(hijack_ProcessHttpBodyRecv));
        // }





        if (true)
        {
            // 0F 10 00 0F 11 07 0F 10 48 10 0F 11 4F 10  (right after a je - call - jmp - mov rax)

            // waaaaaay above 'AppletMenu'
            // waaaaaay below 'fullbody_user_slots' (about 0x2543 below)
            // scan down until even handler
            // scan down until smaller function with a ton of xmm stuff going on. past 'x', past '`'
            // should have 5 chunks of xmm stuff in a single function.

            unsigned char hijack_PositionUpdate[14] = {
                0x48, 0xB9, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, // MOV RCX, [address]
                0xFF, 0xE1,                                                 // JMP RCX
                0x90,                                                       // NOP
                0x90                                                        // NOP
            };

            const auto offset = 0x172545C;
            *((uint64_t *)&hijack_PositionUpdate[2]) = (uint64_t)intercept_ProcessPositionUpdate;
            RewriteCode(base + offset, hijack_PositionUpdate, sizeof(hijack_PositionUpdate));

            ReturnPoint_ProcessPositionUpdate = (uint64_t)(base + offset + sizeof(hijack_PositionUpdate));
        }

         if (0) // always falseish  // NOT YET UPDATED (2020-09-30)
         {
        //     // NOT YET TESTED for 2020-09-10
        // 
        //     // TODO: Rewrite BodyCinfo... thisw has the actual body type in it at offset 0x40
        //     //   0x02 = dyamic
        //     //   0x00 = static
        // 
        //     //this is clientside only lol, boo
        //     // search for "bodyResourceHandle" (right after 3rd call) a little above "shape" (right after 4th call above it)
        //     // further down is the canGrabEverything stuff

            // 48 01 AB 68 1B 00 00 4C 39 64 24 40
            auto processBodyCinfoRva = 0x17EB361;
            unsigned char processBodyCinfoData[] = {
                0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RCX, <address>
                0xFF, 0xE1                                                   // JMP RCX
            };
            *((uint64_t *)&processBodyCinfoData[2]) = (uint64_t)intercept_ProcessBodyCinfo;
            RewriteCode(base + processBodyCinfoRva, processBodyCinfoData, sizeof(processBodyCinfoData));
            ReturnPoint_ProcessBodyCinfo = (uint64_t)(base + processBodyCinfoRva + sizeof(processBodyCinfoData));
         
         
            // search for "bodyResourceHandle" between "shape" and "name" (dealing with al). patch right after 'ja' (before al stuff happens)
            // 44 38 22 0F 95 C0 88 87 F0 03 00 00
           auto canGrabEverythingRva = 0x17E5ADE; // 0x17EB47E
           unsigned char canGrabEverythingData[6] = {
               0xB0, 0x01,                   // MOV AL, 1
               0x90,                         // NOP
               0x90,                         // NOP
               0x90,                         // NOP
               0x90                          // NOP
           };
           RewriteCode(base + canGrabEverythingRva, canGrabEverythingData, sizeof(canGrabEverythingData));


           // 44 38 22 0F 95 C0 88 87 F1 03 00 00 48 FF 83 68 1B 00 00
           // 17C8DD0
           auto canRideEverythingRva = 0x17E5B60; // 0x17EB500
           unsigned char canRideEverythingData[6] = {
               0xB0, 0x00,                   // MOV AL, 1
               0x90,                         // NOP
               0x90,                         // NOP
               0x90,                         // NOP
               0x90                          // NOP
           };
           RewriteCode(base + canRideEverythingRva, canRideEverythingData, sizeof(canRideEverythingData));


        // search for "spawnPointComponentDef", after "name". patch right after last 'ja', dealing with 'al'
        // 
        // 80 3A 00 0F 95 C0 88 87 C0 00 00 00 48 FF 83 68 1B 00 00
              auto nothingFixedInWorldRva = 0x17DAD22; // 0x17DDCF2
              unsigned char nothingFixedInWorldData[6] = {
                  0xB0, 0x00,                   // MOV AL, 0
                  0x90,                         // NOP
                  0x90,                         // NOP
                  0x90,                         // NOP
                  0x90                          // NOP
              };
              RewriteCode(base + nothingFixedInWorldRva, nothingFixedInWorldData, sizeof(nothingFixedInWorldData));
        }

        if (true) 
        {
            // The JBE after:
            // 66 0F 70 C3 55 66 0F 70 CB AA 0F 58 D8 0F 58 D9 0F C6 DB 00 0F 2E DA


            // Seating (desktop)
            auto unlimitedItemPickRangeRva_DesktopSeat = 0x178E114;
            unsigned char unlimitedItemPickRangeData_DesktopSeat[1] = {
                0xeb                            // JB -> JMP
            };
            RewriteCode(base + unlimitedItemPickRangeRva_DesktopSeat, unlimitedItemPickRangeData_DesktopSeat, sizeof(unlimitedItemPickRangeData_DesktopSeat));
            
            
            // Items (desktop)
            auto unlimitedItemPickRangeRva_DesktopItems = 0x178E4E2;
            unsigned char unlimitedItemPickRangeData_DesktopItems[1] = {
                0xeb                            // JB -> JMP
            };
            RewriteCode(base + unlimitedItemPickRangeRva_DesktopItems, unlimitedItemPickRangeData_DesktopItems, sizeof(unlimitedItemPickRangeData_DesktopItems));
            
            // UnknownA
            auto unlimitedItemPickRangeRva_UnknownA = 0x178E741;
            unsigned char unlimitedItemPickRangeData_UnknownA[1] = {
                0xeb                            // JB -> JMP
            };
            RewriteCode(base + unlimitedItemPickRangeRva_UnknownA, unlimitedItemPickRangeData_UnknownA, sizeof(unlimitedItemPickRangeData_UnknownA));
                        
            
            // UnknownB
            auto unlimitedItemPickRangeRva_UnknownB = 0x178E9DF;
            unsigned char unlimitedItemPickRangeData_UnknownB[1] = {
                0xeb                            // JB -> JMP
            };
            RewriteCode(base + unlimitedItemPickRangeRva_UnknownB, unlimitedItemPickRangeData_UnknownB, sizeof(unlimitedItemPickRangeData_UnknownB));
        }


        // if (true)
        // {
        //     // 48 83 F9 01 7C 2F 48 69 C9 D0 00 00 00  (above. sub,je,sub,je,sub,  just nop it all and jjump to the exit state
        //     auto skipUnknownEventCrash = 0x1724361;
        //     unsigned char skipUnknownEventCrashData[] = {
	    //         0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	    //         0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9, 0xAF,
	    //         0x01, 0x00, 0x00, 0x90  // Nop a bunch of stuff and force jump to exit state
        //     };
        //     RewriteCode(base + skipUnknownEventCrash, skipUnknownEventCrashData, sizeof(skipUnknownEventCrashData));
        // 
        // 
        //     //{
        //     //    auto skipUnknownEventCrash_Logging = 0x1C339AC;
        //     //    unsigned char skipUnknownEventCrashData_Logging[] = {
	    //     //        0xEB // JMP
        //     //    };
        //     //    RewriteCode(base + skipUnknownEventCrash_Logging, skipUnknownEventCrashData_Logging, sizeof(skipUnknownEventCrashData_Logging));
        //     //}
        //     //{
        //     //  auto skipUnknownEventCrash_Logging = 0x1C3C481;
        //     //   unsigned char skipUnknownEventCrashData_Logging[] = {
	    //     //       0xE9, 0xA2, 0x00, 0x00, 0x00, 0x90  // JE -> JMP
        //     //  };
        //     //  RewriteCode(base + skipUnknownEventCrash_Logging, skipUnknownEventCrashData_Logging, sizeof(skipUnknownEventCrashData_Logging));
        //    // }
        // }

        // OUT OF DATE, update me
       // if (true)
       // {
       //     // 2B D1 75 07 48 FF C0 85 C9 75 ED 85 D2 75 42   nop the jne before 'client version is latest'
       //     auto skipUnknownEventCrash = 0x1D082E5;
       //     unsigned char skipUnknownEventCrashData[] = {
	   //         0x90, 0x90,
       //     };
       //     RewriteCode(base + skipUnknownEventCrash, skipUnknownEventCrashData, sizeof(skipUnknownEventCrashData));
       // }

        if (true)
        {
            // 0F 85 A6 00 00 00 40 38 73 52 0F 84 9C 00 00 00 48 8B 4B 10 48 8B 81 80 8E 00 00 80 78 38 01
            auto doubleJumpFix1 = 0x17676AB;
            unsigned char doubleJumpFix1Data[] = {
	            0x90, 0x90, 0x90, 0x90,0x90, 0x90,
            };
            RewriteCode(base + doubleJumpFix1, doubleJumpFix1Data, sizeof(doubleJumpFix1Data));

            // 0F 84 9C 00 00 00 48 8B 4B 10 48 8B 81 80 8E 00 00 80 78 38 01 0F 84 87 00 00 00 48 8B 89 98 8E 00 00
            auto doubleJumpFix2 = 0x17676B5;
            unsigned char doubleJumpFix2Data[] = {
	            0x90, 0x90, 0x90, 0x90,0x90, 0x90,
            };
            RewriteCode(base + doubleJumpFix2, doubleJumpFix2Data, sizeof(doubleJumpFix2Data));

            // je right above 'started jumping'
            auto doubleJumpFix3 = 0x17676EF;
            unsigned char doubleJumpFix3Data[] = {
	            0x90, 0x90,
            };
            RewriteCode(base + doubleJumpFix3, doubleJumpFix3Data, sizeof(doubleJumpFix3Data));
        }

        if (true)
        {
            // 0F 28 44 24 40 0F 11 87 20 43 00 00 48 85 DB
            auto offset_GetPositionForVoiceServer = 0x121C375;
            unsigned char hijack_GetPositionForVoiceServer[] = {
                0x48, 0xB9, 0x11, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, // MOV RCX, [address]
                0xFF, 0xE1                                                  // JMP RCX
            };

            *((uint64_t *)&hijack_GetPositionForVoiceServer[2]) = (uint64_t)intercept_GetPositionForVoiceServer;
            RewriteCode(base + offset_GetPositionForVoiceServer, hijack_GetPositionForVoiceServer, sizeof(hijack_GetPositionForVoiceServer));
        
            ReturnPoint_GetPositionForVoiceServer = (uint64_t)(base + offset_GetPositionForVoiceServer + sizeof(hijack_GetPositionForVoiceServer));
        }

        // We got this constant from memory.
        // Search for "no-input-source".
        // Between "room scale" and "no-input-source"
        // Right below a call to RotMatrix, enter that call (no longer called there directly. just look between the two for the call. it's there)
        // rcx+30 = our pointer
        // Function above it with a bunch of xmm stuff going on (see screenshots)
        //const static auto kCameraPositionOffset = 0x4C8A060;

        // CameraPositionOffset: (Do not just copy rva - we have to break here, follow [rcx+30 in dump, copy rva of address)
        // 0F 28 49 30 0F 29 4B 30 48 8B 5C 24 70
         CameraPositionOffset = (float *)(base + 0x4CAA220);








        //auto aabbCheckBypassAvatar1 = (char *)(base + 0x217341B);
        //RewriteCode(aabbCheckBypassAvatar1, (uint8_t*)"\x90\x90", 2);
        //
        //auto aabbCheckBypassAvatar2 = (char *)(base + 0x217343C);
        //RewriteCode(aabbCheckBypassAvatar2, (uint8_t*)"\x90\x90", 2);
        //
        //auto aabbCheckBypassAttachment = (char *)(base + 0x2171A03);
        //RewriteCode(aabbCheckBypassAttachment, (uint8_t*)"\xEB", 1);

        //auto maxVertsClothingHair = (char *)(base + 0x2171EA8 + 3);
        //RewriteCode(maxVertsClothingHair, (uint8_t*)"\xFF\xFF\xFF\x7F", 4);
        //
        //auto maxVertsAttachment = (char *)(base + 0x2171627 + 3);
        //RewriteCode(maxVertsAttachment, (uint8_t*)"\xFF\xFF\xFF\x7F", 4);
        //
        //auto maxVertsAvatar = (char *)(base + 0x217324D + 7);
        //RewriteCode(maxVertsAvatar, (uint8_t*)"\xFF\xFF\xFF\x7F", 4);






        // 37 66 35 39 31 31 35 38 35 63 35 30 31 36 34 34 35 31 35 31
       // auto licenseAddr = (char *)(base + 0x4769A80);
       // printf("Umbra License = %s\n", licenseAddr);
       // OutputDebugStringA(licenseAddr);
       // RewriteCode(licenseAddr, (uint8_t*)"7f571357585512445d52", 20); // 7 year license. good enough

        // 7f5810585a5313475f57

        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
      //   DetourAttach(&(PVOID &)original_recvfrom, Hooked_Recvfrom); // This is too much to handle. A million different edge cases in packets and ordering. Just going to let the program take care of all the work for us and hook the end calls...
   //     DetourAttach(&(PVOID &)original_sendto, Hooked_Sendto);
      //   DetourAttach(&(PVOID &)original_recv, Hooked_Recv); // need to hard hook into ssl functions or post-decrypted recv handler...
     //   DetourAttach(&(PVOID &)original_send, Hooked_Send);

        DetourTransactionCommit();

        Utils::InitKeyToNameDatabase();

        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}


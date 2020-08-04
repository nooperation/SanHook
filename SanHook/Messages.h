#pragma once

#undef PlaySound

#include <cstdint>

class AudioMessages
{
public:
    //static const uint32_t = 0x;
    static const uint32_t LoadSound = 0x412484C4;
    static const uint32_t PlaySound = 0x8FC77316;
    static const uint32_t PlayStream = 0x6A2C4CEF;
    static const uint32_t StopBroadcastingSound = 0x866BF5CF;
    static const uint32_t SetAudioStream = 0x5DCD6123;
    static const uint32_t SetMediaSource = 0xEC3CA8EC;
    static const uint32_t PerformMediaAction = 0x559B7F04;
    static const uint32_t StopSound = 0x1A5C9610;
    static const uint32_t SetLoudness = 0x20EDD0C4;
    static const uint32_t SetPitch = 0x7BB86A5B;
};

class RenderMessages
{
public:
    //static const uint32_t = 0x;
    static const uint32_t LightStateChanged = 0x6951DAEC;
};

class SimulationMessages
{
public:
    //static const uint32_t = 0x;
    static const uint32_t InitialTimestamp = 0x0D094FEA;
    static const uint32_t Timestamp = 0x1E9B31CE;
    static const uint32_t SetWorldGravityMagnitude = 0x86E6A7F6;
    static const uint32_t ActiveRigidBodyUpdate = 0x864418DA;
    static const uint32_t RigidBodyDeactivated = 0x0D938F45;
    static const uint32_t RigidBodyPropertyChanged = 0x45FAAEBC;
    static const uint32_t RigidBodyDestroyed = 0x3A92215C;
};

class AgentControllerMessages
{
public:
    static const uint32_t PlayAnimation = 0x009385A0;
    static const uint32_t AgentPlayanimation = 0x00AC2B81;
    static const uint32_t ExitSit = 0x0B617A9A;
    static const uint32_t ObjectInteractionPromptUpdate = 0x1651CD68;
    static const uint32_t ObjectInteractionCreate = 0xBB086E9B;
    static const uint32_t RequestSitOnObject = 0xE5321C47;
    static const uint32_t SitOnObject = 0x191F08C0;
    static const uint32_t RiggedMeshScaleChanged = 0xEA2934E8;
    static const uint32_t ScriptCameraMessage = 0x60C955C0;
    static const uint32_t SetAgentFiltersBody = 0x09DD53F6;
    static const uint32_t RequestSetAgentFiltersBody = 0x2B87F09D;
    static const uint32_t SetCharacterUserProperty = 0x31D1EC43;
    static const uint32_t CreateSpeechGraphicsPlayer = 0x158B2580;
    static const uint32_t RequestSpawnItem = 0x2C21850D;
    static const uint32_t RequestDeleteLatestSpawn = 0xEB3C4296;
    static const uint32_t RequestDeleteAllSpawns = 0x3EB3EDF7;
    static const uint32_t ControlPoint = 0x2DF35CF3;
    static const uint32_t WarpCharacter = 0x75C0AC6B;
    static const uint32_t RequestWarpCharacter = 0x25C093E0;
    static const uint32_t CharacterControlPointInput = 0xFCA3EF20;
    static const uint32_t CharacterControlPointInputReliable = 0x8FB6F456;
    static const uint32_t CharacterControllerInput = 0x3D490CAB;
    static const uint32_t CharacterControllerInputReliable = 0xA7D6EFD1;
    static const uint32_t RequestAgentPlayAnimation = 0x982B98D8;
    static const uint32_t RequestBehaviorStateUpdate = 0x5489A347;
    static const uint32_t AttachToCharacterNode = 0x85BA6E75;
    static const uint32_t DetachFromCharacterNode = 0x80F90328;
    static const uint32_t RequestDetachFromCharacterNode = 0x67B63AA3;
    static const uint32_t SetCharacterNodePhysics = 0x645C4976;
    static const uint32_t WarpCharacterNode = 0x83F1D7DB;
    static const uint32_t CharacterIKBone = 0xBB382C6B;
    static const uint32_t CharacterIKPose = 0xE945D8B8;
    static const uint32_t CharacterIKBoneDelta = 0x4C3B3B4B;
    static const uint32_t CharacterIKPoseDelta = 0x893A18BE;
    static const uint32_t ObjectInteraction = 0xA25F81AB;
    static const uint32_t ObjectInteractionUpdate = 0x17B7D18A;
};

class GameWorldMessages {
public:
    // static const uint32_t = 0x;
    static const uint32_t StaticMeshFlagsChanged = 0xAE522F17;
    static const uint32_t StaticMeshScaleChanged = 0xCA6CCC08;
    static const uint32_t RiggedMeshFlagsChange = 0x3F020C77;
    static const uint32_t Timestamp = 0xD22C9D73;
    static const uint32_t MoveEntity = 0xEFC20B7F;
    static const uint32_t ChangeMaterialVectorParam = 0x403D5704;
    static const uint32_t ChangeMaterialFloatParam = 0x4F20B073;
    static const uint32_t ChangeMaterial = 0x45C605B8;
};

class WorldStateMessages {
public:
    static const uint32_t DynamicPlayback = 0x1505C6D8;
    static const uint32_t MasterFrameSync = 0x5A4AFA33;
    static const uint32_t AgentControllerMapping = 0xBB5865E8;

    static const uint32_t CreateWorld = 0x685B436C;
    static const uint32_t DestroyWorld = 0x20C45982;
    static const uint32_t RigidBodyComponentInitialState = 0x065C105B;
    static const uint32_t AnimationComponentInitialState = 0xDE87FDD8;
    static const uint32_t LoadClusterDefinition = 0xA5C4FB23;
    static const uint32_t ComponentRelativeTransform = 0x941E6445;
    static const uint32_t InitiateCluster = 0x349AD257;
    static const uint32_t CreateClusterViaDefinition = 0x73810D53;
    static const uint32_t DestroyCluster = 0x2926D248;
    static const uint32_t DestroyObject = 0x5749A1CD;
    static const uint32_t DestroySourceIdSpace = 0x1505C6D8;
    static const uint32_t CreateCharacterNode = 0x32DC63D7;
    static const uint32_t CreateAgentController = 0xF555FE2D;
    static const uint32_t DestroyAgentController = 0x16406FB7;
    //static const uint32_t  = 0x;
};

class ClientKafkaMessages
{
public:
    static const uint32_t FriendResponseLoaded = 0x0AF50C12;
    static const uint32_t PresenceUpdateFanoutLoaded = 0x5915FBFE;
    static const uint32_t FriendTableLoaded = 0xB4AB87F5;
    static const uint32_t RelationshipTableLoaded = 0x0A7562A7;
    static const uint32_t PrivateChatLoaded = 0x4B73CF2C;
    static const uint32_t PrivateChatStatusLoaded = 0x9BC4EF8A;
    static const uint32_t ScriptRegionConsoleLoaded = 0xD3CAA979;
    static const uint32_t ClientMetric = 0x4AC30FE7;
    static const uint32_t RegionHeartbeatMetric = 0xDCF900A4;
    static const uint32_t RegionEventMetric = 0xBA6DB2FC;
    static const uint32_t SubscribeScriptRegionConsole = 0x3BFA4474;
    static const uint32_t UnsubscriptScriptRegionConsole = 0xD49B04C3;
    static const uint32_t ScriptConsoleLog = 0x00B0E15E;
    static const uint32_t LongLivedNotification = 0x46C5FDF3;
    static const uint32_t LongLivedNotificationDelete = 0x59CF6950;
    static const uint32_t LongLivedNotificationLoaded = 0x3494608D;
    static const uint32_t shortLivedNotification = 0xAD589C6F;
    static const uint32_t Login = 0x0C0C9D81;
    static const uint32_t LoginReply = 0xA685E82B;
    static const uint32_t EnterRegion = 0x08445006;
    static const uint32_t LeaveRegion = 0xE4ADC2EB;
    static const uint32_t RegionChat = 0x304D3746;
    static const uint32_t PrivateChat = 0x2DC9B029;
    static const uint32_t PrivateChatStatus = 0x955C35EB;
    static const uint32_t PresenceUpdate = 0x1DB989E8;
    static const uint32_t FriendRequest = 0xA356B3ED;
    static const uint32_t FriendRequestStatus = 0x14FFCD37;
    static const uint32_t FriendResponse = 0xE24EBDD3;
    static const uint32_t FriendresponseStatus = 0x22565685;
    static const uint32_t FriendTable = 0x203CC0A8;
    static const uint32_t RelationshipOperational = 0x650939F7;
    static const uint32_t RelationshipTable = 0x078DCC26;
    static const uint32_t InventoryItemCapabilities = 0xA2190F5D;
    static const uint32_t InventoryItemRevision = 0xE3466906;
    static const uint32_t InventoryItemUpdate = 0xD7C7DC26;
    static const uint32_t InventoryItemDelete = 0xB11C8C84;
    static const uint32_t InventoryLoaded = 0x75BAFB95;
    static const uint32_t FriendRequestLoaded = 0xF5361468;
};

class ClientVoiceMessages {
public:
    static const uint32_t LoginReply = 0xA6972017;
    static const uint32_t LocalAudioStreamState = 0xF2FB6AD0; /// uhhh double check these two
};

class ClientRegionMessages {
public:
    static const uint32_t UserLogin = 0x3902800A;
    static const uint32_t UserLoginReply = 0x30CDBED6;
    static const uint32_t AddUser = 0xF6B9093E;
    static const uint32_t RemoveUser = 0x0A7FC621;
    static const uint32_t RenameUser = 0xC67C58F7;
    static const uint32_t ChatMessageToClient = 0x083642BD;
    static const uint32_t ChatMessageToServer = 0xDDDEC199;
    static const uint32_t SetAgentController = 0xD6F4CF23;
    static const uint32_t DebugTimeChangeToServer = 0x41FE0612;
    static const uint32_t VisualDebuggerCaptureToServer = 0x0741CA9B;
    static const uint32_t ClientStaticReady = 0xF8E77C8E;
    static const uint32_t ClientDynamicReady = 0x575AC239;
    static const uint32_t ClientRegionCommandMessage = 0xECE56EFD;
    static const uint32_t RequestDropPortal = 0x7D22C30C;
    static const uint32_t VibrationPulseToClient = 0x0D3809EB;
    static const uint32_t TeleportTo = 0x5C7CC1FC;
    static const uint32_t TeleportToUri = 0x2BDBDB56;
    static const uint32_t TeleportToEditMode = 0x706F63FB;
    static const uint32_t DebugTimeChangeToClient = 0x5178DC5E;
    static const uint32_t VisualDebuggerCaptureToClient = 0xF66AD9BF;
    static const uint32_t ScriptModalDialog = 0x88023C72;
    static const uint32_t ScriptModalDialogResponse = 0xB34F3A45;
    static const uint32_t TwitchEventSubscription = 0x981AB0D6;
    static const uint32_t TwitchEvent = 0x28F54053;
    static const uint32_t InitialChunkSubscribed = 0xB4E1AB7B;
    static const uint32_t ClientKickNotification = 0x4B68A51C;
    static const uint32_t ClientSmiteNotification = 0x58003034;
    static const uint32_t ClientNotification = 0x6188A537;
    static const uint32_t ClientVoiceBroadcastStartNotification = 0x7E28AEAF;
    static const uint32_t ClientVoiceBroadcastStopNotification = 0xC33DE58B;
    static const uint32_t CreateRegionBroadcasted = 0x87341F77;
    static const uint32_t SubscribeCommand = 0xABDA80C7;
    static const uint32_t UnsubscribeCommand = 0xA36E9F9C;
    static const uint32_t ClientCommand = 0xB87F9C66;
    static const uint32_t OpenStoreListing = 0x05C1A8D7D;
    static const uint32_t OpenUserStore = 0x53078A1E;
    static const uint32_t OpenQuestCharacterDialog = 0x4221836F;
    static const uint32_t UIScriptableBarStart = 0x036164050;
    static const uint32_t UIScriptableBarStopped = 0xBAFD799D;
    static const uint32_t UIScriptableBarCancel = 0x604E18DE;
    static const uint32_t UIHintTextUpdate = 0x64225637;
    static const uint32_t QuestOfferResponse = 0x4DB48E35;
    static const uint32_t QuestCompleted = 0xE1EE5F5D;
    static const uint32_t QuestRemoved = 0x34793AB0;
    static const uint32_t ShowWorldDetail = 0x5F483F0C;
    static const uint32_t ShowTutorialHint = 0x581827CC;
    static const uint32_t TutorialHintsSetEnabled = 0xE4C496DF;
};

class EditServerMessages {
public:
    static const uint32_t UserLogin = 0x046D3C1E;
    static const uint32_t UserLoginReply = 0xE227C3E2;
    static const uint32_t AddUser = 0x50155562;
    static const uint32_t RemoveUser = 0x5729AC25;
    static const uint32_t OpenWorkspace = 0xBC512F47;
    static const uint32_t CloseWorkspace = 0x43C06583;
    static const uint32_t EditWorkspaceCommand = 0x76363E28;
    static const uint32_t SaveWorkspace = 0x7C7BDCA8;
    static const uint32_t SaveWorkspaceReply = 0xFAE838FC;
    static const uint32_t BuildWorkspace = 0x5963934F;
    static const uint32_t UpdateWorkspaceClientbuiltBakeData = 0xF12FD324;
    static const uint32_t BuildWorkspaceCompileReply = 0x15B220E0;
    static const uint32_t BuildWorkspaceProgressUpdate = 0xC9FCDB71;
    static const uint32_t BuildWorkspaceUploadReply = 0xF090AF8E;
    static const uint32_t WorkspaceReadyReply = 0x7D87DBEA;
    static const uint32_t SaveWorkspaceSelectionToInventory = 0x7E37F335;
    static const uint32_t SaveWorkspaceSelectionToInventoryReply = 0x439C3637;
    static const uint32_t InventoryCreateItem = 0xB5487205;
    static const uint32_t InventoryDeleteItem = 0x6F75848E;
    static const uint32_t InventoryChangeItemName = 0x690612C6;
    static const uint32_t InventoryChangeItemState = 0x6C202756;
    static const uint32_t InventoryModifyItemThumbnailAssetId = 0x2C2AE45E;
    static const uint32_t InventoryModifyItemCapabilities = 0xF93582DD;
    static const uint32_t InventorySaveItem = 0xA67D88F8;
    static const uint32_t InventoryUpdateItemReply = 0x144D39F8;
    static const uint32_t InventoryItemUpload = 0xF2E11A50;
    static const uint32_t InventoryItemUploadReply = 0xA25566F4;
    static const uint32_t InventoryCreateListing = 0xBE2C532E;
    static const uint32_t InventoryCreateListingReply = 0x4EA3D072;
    static const uint32_t BeginEditServerSpawn = 0xB5BFECD3;
    static const uint32_t EditServerSpawnReady = 0xB3623297;
};

class AnimationComponentMessages {
public:
    static const uint32_t FloatVariable = 0x0B3B7D2E;
    static const uint32_t FloatNodeVariable = 0x4C1B3DF2;
    static const uint32_t FloatRangeNodeVariable = 0x91419DEB;
    static const uint32_t VectorVariable = 0x23314E53;
    static const uint32_t QuaternionVariable = 0x0CC9F1B8;
    static const uint32_t Int8Variable = 0xC11AFDE7;
    static const uint32_t BoolVariable = 0xA67454F0;
    static const uint32_t CharacterTransform = 0xAB2F1EB1;
    static const uint32_t CharacterTransformPersistent = 0x970F93D4;
    static const uint32_t CharacterAnimationDestroyed = 0x53A4BF26;
    static const uint32_t AnimationOverride = 0x8C738C9E;
    static const uint32_t BehaviorInternalState = 0xCE9B5148;
    static const uint32_t CharacterBehaviorInternalState = 0x16C090B1;
    static const uint32_t BehaviorStateUpdate = 0x217192BE;
    static const uint32_t BehaviorInitializationData = 0x7846436E;
    static const uint32_t CharacterSetPosition = 0x51A1705A;
    static const uint32_t PlayAnimation = 0x009385A0;
};
#pragma once


class AudioMessages
{
public:
    //static const int = 0x;
    static const int LoadSound = 0x412484C4;
    static const int PlaySound = 0x8FC77316;
    static const int PlayStream = 0x6A2C4CEF;
    static const int StopBroadcastingSound = 0x866BF5CF;
    static const int SetAudioStream = 0x5DCD6123;
    static const int SetMediaSource = 0xEC3CA8EC;
    static const int PerformMediaAction = 0x559B7F04;
    static const int StopSound = 0x1A5C9610;
    static const int SetLoudness = 0x20EDD0C4;
    static const int SetPitch = 0x7BB86A5B;
};

class RenderMessages
{
public:
    //static const int = 0x;
    static const int LightStateChanged = 0x6951DAEC;
};

class SimulationMessages
{
public:
    //static const int = 0x;
    static const int InitialTimestamp = 0x0D094FEA;
    static const int Timestamp = 0x1E9B31CE;
    static const int SetWorldGravityMagnitude = 0x86E6A7F6;
    static const int ActiveRigidBodyUpdate = 0x864418DA;
    static const int RigidBodyDeactivated = 0x0D938F45;
    static const int RigidBodyPropertyChanged = 0x45FAAEBC;
    static const int RigidBodyDestroyed = 0x3A92215C;
};

class AgentControllerMessages
{
public:
    static const int PlayAnimation = 0x009385A0;
    static const int AgentPlayanimation = 0x00AC2B81;
    static const int ExitSit = 0x0B617A9A;
    static const int ObjectInteractionPromptUpdate = 0x1651CD68;
    static const int ObjectInteractionCreate = 0xBB086E9B;
    static const int RequestSitOnObject = 0xE5321C47;
    static const int SitOnObject = 0x191F08C0;
    static const int RiggedMeshScaleChanged = 0xEA2934E8;
    static const int ScriptCameraMessage = 0x60C955C0;
    static const int SetAgentFiltersBody = 0x09DD53F6;
    static const int RequestSetAgentFiltersBody = 0x2B87F09D;
    static const int SetCharacterUserProperty = 0x31D1EC43;
    static const int CreateSpeechGraphicsPlayer = 0x158B2580;
    static const int RequestSpawnItem = 0x2C21850D;
    static const int RequestDeleteLatestSpawn = 0xEB3C4296;
    static const int RequestDeleteAllSpawns = 0x3EB3EDF7;
    static const int ControlPoint = 0x2DF35CF3;
    static const int WarpCharacter = 0x75C0AC6B;
    static const int RequestWarpCharacter = 0x25C093E0;
    static const int CharacterControlPointInput = 0xFCA3EF20;
    static const int CharacterControlPointInputReliable = 0x8FB6F456;
    static const int CharacterControllerInput = 0x3D490CAB;
    static const int CharacterControllerInputReliable = 0xA7D6EFD1;
    static const int RequestAgentPlayAnimation = 0x982B98D8;
    static const int RequestBehaviorStateUpdate = 0x5489A347;
    static const int AttachToCharacterNode = 0x85BA6E75;
    static const int DetachFromCharacterNode = 0x80F90328;
    static const int RequestDetachFromCharacterNode = 0x67B63AA3;
    static const int SetCharacterNodePhysics = 0x645C4976;
    static const int WarpCharacterNode = 0x83F1D7DB;
    static const int CharacterIKBone = 0xBB382C6B;
    static const int CharacterIKPose = 0xE945D8B8;
    static const int CharacterIKBoneDelta = 0x4C3B3B4B;
    static const int CharacterIKPoseDelta = 0x893A18BE;
    static const int ObjectInteraction = 0xA25F81AB;
    static const int ObjectInteractionUpdate = 0x17B7D18A;
};

class GameWorldMessages {
public:
    // static const int = 0x;
    static const int StaticMeshFlagsChanged = 0xAE522F17;
    static const int StaticMeshScaleChanged = 0xCA6CCC08;
    static const int RiggedMeshFlagsChange = 0x3F020C77;
    static const int Timestamp = 0xD22C9D73;
    static const int MoveEntity = 0xEFC20B7F;
    static const int ChangeMaterialVectorParam = 0x403D5704;
    static const int ChangeMaterialFloatParam = 0x4F20B073;
    static const int ChangeMaterial = 0x45C605B8;
};

class WorldStateMessages {
public:
    static const int DynamicPlayback = 0x1505C6D8;
    static const int MasterFrameSync = 0x5A4AFA33;
    static const int AgentControllerMapping = 0xBB5865E8;

    static const int CreateWorld = 0x685B436C;
    static const int DestroyWorld = 0x20C45982;
    static const int RigidBodyComponentInitialState = 0x065C105B;
    static const int AnimationComponentInitialState = 0xDE87FDD8;
    static const int LoadClusterDefinition = 0xA5C4FB23;
    static const int ComponentRelativeTransform = 0x941E6445;
    static const int InitiateCluster = 0x349AD257;
    static const int CreateClusterViaDefinition = 0x73810D53;
    static const int DestroyCluster = 0x2926D248;
    static const int DestroyObject = 0x5749A1CD;
    static const int DestroySourceIdSpace = 0x1505C6D8;
    static const int CreateCharacterNode = 0x32DC63D7;
    static const int CreateAgentController = 0xF555FE2D;
    static const int DestroyAgentController = 0x16406FB7;
    //static const int  = 0x;
};

class ClientKafkaMessages
{
public:
    static const int FriendResponseLoaded = 0x0AF50C12;
    static const int PresenceUpdateFanoutLoaded = 0x5915FBFE;
    static const int FriendTableLoaded = 0xB4AB87F5;
    static const int RelationshipTableLoaded = 0x0A7562A7;
    static const int PrivateChatLoaded = 0x4B73CF2C;
    static const int PrivateChatStatusLoaded = 0x9BC4EF8A;
    static const int ScriptRegionConsoleLoaded = 0xD3CAA979;
    static const int ClientMetric = 0x4AC30FE7;
    static const int RegionHeartbeatMetric = 0xDCF900A4;
    static const int RegionEventMetric = 0xBA6DB2FC;
    static const int SubscribeScriptRegionConsole = 0x3BFA4474;
    static const int UnsubscriptScriptRegionConsole = 0xD49B04C3;
    static const int ScriptConsoleLog = 0x00B0E15E;
    static const int LongLivedNotification = 0x46C5FDF3;
    static const int LongLivedNotificationDelete = 0x59CF6950;
    static const int LongLivedNotificationLoaded = 0x3494608D;
    static const int shortLivedNotification = 0xAD589C6F;
    static const int Login = 0x0C0C9D81;
    static const int LoginReply = 0xA685E82B;
    static const int EnterRegion = 0x08445006;
    static const int LeaveRegion = 0xE4ADC2EB;
    static const int RegionChat = 0x304D3746;
    static const int PrivateChat = 0x2DC9B029;
    static const int PrivateChatStatus = 0x955C35EB;
    static const int PresenceUpdate = 0x1DB989E8;
    static const int FriendRequest = 0xA356B3ED;
    static const int FriendRequestStatus = 0x14FFCD37;
    static const int FriendResponse = 0xE24EBDD3;
    static const int FriendresponseStatus = 0x22565685;
    static const int FriendTable = 0x203CC0A8;
    static const int RelationshipOperational = 0x650939F7;
    static const int RelationshipTable = 0x078DCC26;
    static const int InventoryItemCapabilities = 0xA2190F5D;
    static const int InventoryItemRevision = 0xE3466906;
    static const int InventoryItemUpdate = 0xD7C7DC26;
    static const int InventoryItemDelete = 0xB11C8C84;
    static const int InventoryLoaded = 0x75BAFB95;
    static const int FriendRequestLoaded = 0xF5361468;
};


class ClientRegionMessages {
public:
    static const int UserLogin = 0x3902800A;
    static const int UserLoginReply = 0x30CDBED6;
    static const int AddUser = 0xF6B9093E;
    static const int RemoveUser = 0x0A7FC621;
    static const int RenameUser = 0xC67C58F7;
    static const int ChatMessageToClient = 0x083642BD;
    static const int ChatMessageToServer = 0xDDDEC199;
    static const int SetAgentController = 0xD6F4CF23;
    static const int DebugTimeChangeToServer = 0x41FE0612;
    static const int VisualDebuggerCaptureToServer = 0x0741CA9B;
    static const int ClientStaticReady = 0xF8E77C8E;
    static const int ClientDynamicReady = 0x575AC239;
    static const int ClientRegionCommandMessage = 0xECE56EFD;
    static const int RequestDropPortal = 0x7D22C30C;
    static const int VibrationPulseToClient = 0x0D3809EB;
    static const int TeleportTo = 0x5C7CC1FC;
    static const int TeleportToUri = 0x2BDBDB56;
    static const int TeleportToEditMode = 0x706F63FB;
    static const int DebugTimeChangeToClient = 0x5178DC5E;
    static const int VisualDebuggerCaptureToClient = 0xF66AD9BF;
    static const int ScriptModalDialog = 0x88023C72;
    static const int ScriptModalDialogResponse = 0xB34F3A45;
    static const int TwitchEventSubscription = 0x981AB0D6;
    static const int TwitchEvent = 0x28F54053;
    static const int InitialChunkSubscribed = 0xB4E1AB7B;
    static const int ClientKickNotification = 0x4B68A51C;
    static const int ClientSmiteNotification = 0x58003034;
    static const int ClientNotification = 0x6188A537;
    static const int ClientVoiceBroadcastStartNotification = 0x7E28AEAF;
    static const int ClientVoiceBroadcastStopNotification = 0xC33DE58B;
    static const int CreateRegionBroadcasted = 0x87341F77;
    static const int SubscribeCommand = 0xABDA80C7;
    static const int UnsubscribeCommand = 0xA36E9F9C;
    static const int ClientCommand = 0xB87F9C66;
    static const int OpenStoreListing = 0x05C1A8D7D;
    static const int OpenUserStore = 0x53078A1E;
    static const int OpenQuestCharacterDialog = 0x4221836F;
    static const int UIScriptableBarStart = 0x036164050;
    static const int UIScriptableBarStopped = 0xBAFD799D;
    static const int UIScriptableBarCancel = 0x604E18DE;
    static const int UIHintTextUpdate = 0x64225637;
    static const int QuestOfferResponse = 0x4DB48E35;
    static const int QuestCompleted = 0xE1EE5F5D;
    static const int QuestRemoved = 0x34793AB0;
    static const int ShowWorldDetail = 0x5F483F0C;
    static const int ShowTutorialHint = 0x581827CC;
    static const int TutorialHintsSetEnabled = 0xE4C496DF;
};

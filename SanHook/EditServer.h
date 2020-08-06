#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnEditServerUserLogin(PacketReader &reader);
//void OnEditServerUserLoginReply(PacketReader &reader);
//void OnEditServerAddUser(PacketReader &reader);
//void OnEditServerRemoveUser(PacketReader &reader);
//void OnEditServerOpenWorkspace(PacketReader &reader);
//void OnEditServerCloseWorkspace(PacketReader &reader);
//void OnEditServerEditWorkspaceCommand(PacketReader &reader);
//void OnEditServerSaveWorkspace(PacketReader &reader);
//void OnEditServerSaveWorkspaceReply(PacketReader &reader);
//void OnEditServerBuildWorkspace(PacketReader &reader);
//void OnEditServerUpdateWorkspaceClientBuiltBakeData(PacketReader &reader);
//void OnEditServerBuildWorkspaceCompileReply(PacketReader &reader);
//void OnEditServerBuildWorkspaceProgressUpdate(PacketReader &reader);
//void OnEditServerBuildWorkspaceuploadReply(PacketReader &reader);
//void OnEditServerWorkspaceReadyReply(PacketReader &reader);
//void OnEditServerSaveworkspaceSelectionToInventory(PacketReader &reader);
//void OnEditServerSaveWorkspaceSelectionToInventoryReply(PacketReader &reader);
//void OnEditServerInventoryCreateItem(PacketReader &reader);
//void OnEditServerInventoryDeleteItem(PacketReader &reader);
//void OnEditServerInventoryChangeItemName(PacketReader &reader);
//void OnEditServerInventoryChangeItemState(PacketReader &reader);
//void OnEditServerInventoryModifyItemThumbnailAssetId(PacketReader &reader);
//void OnEditServerInventoryModifyItemCapabilities(PacketReader &reader);
//void OnEditServerInventorySaveItem(PacketReader &reader);
//void OnEditServerInventoryUpdateItemReply(PacketReader &reader);
//void OnEditServerInventoryItemUpload(PacketReader &reader);
//void OnEditServerInventoryItemUploadReply(PacketReader &reader);
//void OnEditServerInventoryCreateListing(PacketReader &reader);
//void OnEditServerInventoryCreateListingReply(PacketReader &reader);
//void OnEditServerBeginEditServerSpawn(PacketReader &reader);
//void OnEditServerEditServerSpawnReady(PacketReader &reader);

class EditServer {
public:
    static void OnUserLogin(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto secret = reader.ReadUint32();
    }

    static void OnUserLoginReply(PacketReader &reader)
    {
        auto success = reader.ReadUint8();
        auto sessionId = reader.ReadUint32();
        auto editServerVersion = reader.ReadString();

        printf("EditServerMessages::LoginReply\n  success = %d\n  sessionId = %d\n  editServerVersion = %s\n",
            success,
            sessionId,
            editServerVersion.c_str()
        );
    }

    static void OnAddUser(PacketReader &reader)
    {
        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
        auto personaId = reader.ReadUUID();

        printf("EditServerMessages::AddUser\n  sessionId = %u\n  userName = %s\n  personaId = %s\n",
            sessionId,
            userName.c_str(),
            personaId.c_str()
        );
    }

    static void OnRemoveUser(PacketReader &reader)
    {
        auto sessionId = reader.ReadUint32();
    }

    static void OnOpenWorkspace(PacketReader &reader)
    {
        auto worldSourceInventoryItemId = reader.ReadUUID();
        auto worldSourceResourceId = reader.ReadString();
    }

    static void OnCloseWorkspace(PacketReader &reader)
    {
        auto workspaceId = reader.ReadUint32();
    }

    static void OnEditWorkspaceCommand(PacketReader &reader)
    {
        auto commandData = reader.ReadArray();
    }

    static void OnSaveWorkspace(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto sessionData = reader.ReadArray();
        auto sceneName = reader.ReadString();
    }

    static void OnSaveWorkspaceReply(PacketReader &reader)
    {
        auto success = reader.ReadUint8();
        auto itemInventoryId = reader.ReadString();
        auto itemResourceId = reader.ReadString();
        auto itemName = reader.ReadString();
    }

    static void OnBuildWorkspace(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto sceneName = reader.ReadString();
        auto start = reader.ReadUint8();
    }

    static void OnUpdateWorkspaceClientBuiltBakeData(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto bakeData = reader.ReadArray();
    }

    static void OnBuildWorkspaceCompileReply(PacketReader &reader)
    {
        auto compileStatus = reader.ReadUint8();
        auto isCanceled = reader.ReadUint8();
        auto errors = reader.ReadStringList();
        auto nonErrorMessage = reader.ReadStringList();
    }

    static void OnBuildWorkspaceProgressUpdate(PacketReader &reader)
    {
        auto stage = reader.ReadUint32();
        auto percent = reader.ReadFloat();
    }

    static void OnBuildWorkspaceuploadReply(PacketReader &reader)
    {
        auto success = reader.ReadUint8();
        auto worldDefinitionId = reader.ReadString();
    }

    static void OnWorkspaceReadyReply(PacketReader &reader)
    {
        auto workspaceId = reader.ReadUint32();

        printf("EditServerMessages::WorkspaceReadyReply\n  workspaceId = %u\n",
            workspaceId
        );
    }

    static void OnSaveworkspaceSelectionToInventory(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto itemName = reader.ReadString();
        auto selectionId = reader.ReadUint64();
        auto triggeringState = reader.ReadUint32();
        auto parentInstanceId = reader.ReadUint32();
    }

    static void OnSaveWorkspaceSelectionToInventoryReply(PacketReader &reader)
    {
        auto itemName = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto triggeringState = reader.ReadUint32();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadUint32();
    }

    static void OnInventoryCreateItem(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto createRequestId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto ssetId = reader.ReadString();
        auto licenseAssetName = reader.ReadString();
        auto inventoryTag = reader.ReadString();
        auto assetSerializationTag = reader.ReadString();
        auto thumbnailAssetId = reader.ReadString();
        auto additionalCapabilities = reader.ReadStringList();
        auto resourceVersion = reader.ReadUint64();
        auto assetState = reader.ReadUint8();
    }

    static void OnInventoryDeleteItem(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto deleteRequestId = reader.ReadUUID();
        auto itemToDeleteId = reader.ReadUUID();
    }

    static void OnInventoryChangeItemName(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto changeNameRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newName = reader.ReadString();
    }

    static void OnInventoryChangeItemState(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto changeStateRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newState = reader.ReadUint8();
    }

    static void OnInventoryModifyItemThumbnailAssetId(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto modifyThumbnailRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newThumbnailAssetId = reader.ReadString();
    }

    static void OnInventoryModifyItemCapabilities(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto modifyCapabilitiesRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto capabilitiesToAdd = reader.ReadStringList();
        auto capabilitiesToRemove = reader.ReadStringList();
    }

    static void OnInventorySaveItem(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto saveItemRequestId = reader.ReadUUID();
        auto itemToSaveId = reader.ReadUUID();
        auto assetId = reader.ReadString();
        auto licenseAssetName = reader.ReadString();
        auto assetState = reader.ReadUint8();
        auto assetSerializationTag = reader.ReadString();
        auto resourceVersion = reader.ReadUint64();
    }

    static void OnInventoryUpdateItemReply(PacketReader &reader)
    {
        auto requestId = reader.ReadUUID();
        auto itemId = reader.ReadUUID();
        auto assetId = reader.ReadString();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadString();
        auto errorMsg = reader.ReadString();
    }

    static void OnInventoryItemUpload(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    static void OnInventoryItemUploadReply(PacketReader &reader)
    {
        auto productId = reader.ReadUUID();
        auto status = reader.ReadString();
        auto listingUrl = reader.ReadString();
        auto editUrl = reader.ReadString();
        auto errorCode = reader.ReadString();
        auto errorMessage = reader.ReadString();
    }

    static void OnInventoryCreateListing(PacketReader &reader)
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
        auto bundleName = reader.ReadString();
    }

    static void OnInventoryCreateListingReply(PacketReader &reader)
    {
        auto canBeListed = reader.ReadUint8();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    static void OnBeginEditServerSpawn(PacketReader &reader)
    {
        auto inventoryId = reader.ReadUUID();
        auto personaId = reader.ReadUUID(); // what's this message, from server or to server?
        auto serial = reader.ReadUint32();
        auto inventoryName = reader.ReadString();
    }

    static void OnEditServerSpawnReady(PacketReader &reader)
    {
        auto isValid = reader.ReadUint8();
        auto serial = reader.ReadUint32();
        auto instanceCount = reader.ReadUint32();
        auto inventoryName = reader.ReadString();
        auto instanceNames = reader.ReadStringList();
        auto folderNames = reader.ReadStringList();
    }
};

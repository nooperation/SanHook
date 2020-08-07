#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
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

class EditServer : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case EditServerMessages::UserLogin: // 19B81B0
            {
                EditServer::OnUserLogin(reader);
                break;
            }
            case EditServerMessages::UserLoginReply: // 19B8220
            {
                EditServer::OnUserLoginReply(reader);
                break;
            }
            case EditServerMessages::AddUser: // 19B8290
            {
                EditServer::OnAddUser(reader);
                break;
            }
            case EditServerMessages::RemoveUser: // 19B8300
            {
                EditServer::OnRemoveUser(reader);
                break;
            }
            case EditServerMessages::OpenWorkspace: // 19B84C0
            {
                EditServer::OnOpenWorkspace(reader);
                break;
            }
            case EditServerMessages::CloseWorkspace: // 19B8530
            {
                EditServer::OnCloseWorkspace(reader);
                break;
            }
            case EditServerMessages::EditWorkspaceCommand: // 19B86F0
            {
                EditServer::OnEditWorkspaceCommand(reader);
                break;
            }
            case EditServerMessages::SaveWorkspace: // 19B88E0
            {
                EditServer::OnSaveWorkspace(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceReply: // 19B8950
            {
                EditServer::OnSaveWorkspaceReply(reader);
                break;
            }
            case EditServerMessages::BuildWorkspace: // 19B89C0
            {
                EditServer::OnBuildWorkspace(reader);
                break;
            }
            case EditServerMessages::UpdateWorkspaceClientBuiltBakeData: // 19B8A30
            {
                EditServer::OnUpdateWorkspaceClientBuiltBakeData(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceCompileReply: // 19B8AA0
            {
                EditServer::OnBuildWorkspaceCompileReply(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceProgressUpdate: // 19B8B10
            {
                EditServer::OnBuildWorkspaceProgressUpdate(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceUploadReply: // 19B8B80
            {
                EditServer::OnBuildWorkspaceuploadReply(reader);
                break;
            }
            case EditServerMessages::WorkspaceReadyReply: // 19B8BF0
            {
                EditServer::OnWorkspaceReadyReply(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceSelectionToInventory: // 19B8DB0
            {
                EditServer::OnSaveworkspaceSelectionToInventory(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceSelectionToInventoryReply: // 19B8E20
            {
                EditServer::OnSaveWorkspaceSelectionToInventoryReply(reader);
                break;
            }
            case EditServerMessages::InventoryCreateItem: // 19B8E90
            {
                EditServer::OnInventoryCreateItem(reader);
                break;
            }
            case EditServerMessages::InventoryDeleteItem: // 19B8F00
            {
                EditServer::OnInventoryDeleteItem(reader);
                break;
            }
            case EditServerMessages::InventoryChangeItemName: // 19B8F70
            {
                EditServer::OnInventoryChangeItemName(reader);
                break;
            }
            case EditServerMessages::InventoryChangeItemState: // 19B8FE0
            {
                EditServer::OnInventoryChangeItemState(reader);
                break;
            }
            case EditServerMessages::InventoryModifyItemThumbnailAssetId: // 19B9050
            {
                EditServer::OnInventoryModifyItemThumbnailAssetId(reader);
                break;
            }
            case EditServerMessages::InventoryModifyItemCapabilities: // 19B90C0
            {
                EditServer::OnInventoryModifyItemCapabilities(reader);
                break;
            }
            case EditServerMessages::InventorySaveItem: // 19B9130
            {
                EditServer::OnInventorySaveItem(reader);
                break;
            }
            case EditServerMessages::InventoryUpdateItemReply: // 19B91A0
            {
                EditServer::OnInventoryUpdateItemReply(reader);
                break;
            }
            case EditServerMessages::InventoryItemUpload: // 19B9210
            {
                EditServer::OnInventoryItemUpload(reader);
                break;
            }
            case EditServerMessages::InventoryItemUploadReply: // 19B9280
            {
                EditServer::OnInventoryItemUploadReply(reader);
                break;
            }
            case EditServerMessages::InventoryCreateListing: // 19B92F0
            {
                EditServer::OnInventoryCreateListing(reader);
                break;
            }
            case EditServerMessages::InventoryCreateListingReply: // 19B9360
            {
                EditServer::OnInventoryCreateListingReply(reader);
                break;
            }
            case EditServerMessages::BeginEditServerSpawn: // 19B93D0
            {
                EditServer::OnBeginEditServerSpawn(reader);
                break;
            }
            case EditServerMessages::EditServerSpawnReady: // 19B9440
            {
                EditServer::OnEditServerSpawnReady(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnUserLogin(PacketReader &reader) // 19B81B0
    {
        auto authorization = reader.ReadString();
        auto secret = reader.ReadUint32();
    }

    static void OnUserLoginReply(PacketReader &reader) // 19B8220
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

    static void OnAddUser(PacketReader &reader) // 19B8290
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

    static void OnRemoveUser(PacketReader &reader) // 19B8300
    {
        auto sessionId = reader.ReadUint32();
    }

    static void OnOpenWorkspace(PacketReader &reader) // 19B84C0
    {
        auto worldSourceInventoryItemId = reader.ReadUUID();
        auto worldSourceResourceId = reader.ReadString();
    }

    static void OnCloseWorkspace(PacketReader &reader) // 19B8530
    {
        auto workspaceId = reader.ReadUint32();
    }

    static void OnEditWorkspaceCommand(PacketReader &reader) // 19B86F0
    {
        auto commandData = reader.ReadArray();
    }

    static void OnSaveWorkspace(PacketReader &reader) // 19B88E0
    {
        auto authorization = reader.ReadString();
        auto sessionData = reader.ReadArray();
        auto sceneName = reader.ReadString();
    }

    static void OnSaveWorkspaceReply(PacketReader &reader) // 19B8950
    {
        auto success = reader.ReadUint8();
        auto itemInventoryId = reader.ReadString();
        auto itemResourceId = reader.ReadString();
        auto itemName = reader.ReadString();
    }

    static void OnBuildWorkspace(PacketReader &reader) // 19B89C0
    {
        auto authorization = reader.ReadString();
        auto sceneName = reader.ReadString();
        auto start = reader.ReadUint8();
    }

    static void OnUpdateWorkspaceClientBuiltBakeData(PacketReader &reader) // 19B8A30
    {
        auto authorization = reader.ReadString();
        auto bakeData = reader.ReadArray();
    }

    static void OnBuildWorkspaceCompileReply(PacketReader &reader) // 19B8AA0
    {
        auto compileStatus = reader.ReadUint8();
        auto isCanceled = reader.ReadUint8();
        auto errors = reader.ReadStringList();
        auto nonErrorMessage = reader.ReadStringList();
    }

    static void OnBuildWorkspaceProgressUpdate(PacketReader &reader) // 19B8B10
    {
        auto stage = reader.ReadUint32();
        auto percent = reader.ReadFloat();
    }

    static void OnBuildWorkspaceuploadReply(PacketReader &reader) // 19B8B80
    {
        auto success = reader.ReadUint8();
        auto worldDefinitionId = reader.ReadString();
    }

    static void OnWorkspaceReadyReply(PacketReader &reader) // 19B8BF0
    {
        auto workspaceId = reader.ReadUint32();

        printf("EditServerMessages::WorkspaceReadyReply\n  workspaceId = %u\n",
            workspaceId
        );
    }

    static void OnSaveworkspaceSelectionToInventory(PacketReader &reader) // 19B8DB0
    {
        auto authorization = reader.ReadString();
        auto itemName = reader.ReadString();
        auto selectionId = reader.ReadUint64();
        auto triggeringState = reader.ReadUint32();
        auto parentInstanceId = reader.ReadUint32();
    }

    static void OnSaveWorkspaceSelectionToInventoryReply(PacketReader &reader) // 19B8E20
    {
        auto itemName = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto triggeringState = reader.ReadUint32();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadUint32();
    }

    static void OnInventoryCreateItem(PacketReader &reader) // 19B8E90
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

    static void OnInventoryDeleteItem(PacketReader &reader) // 19B8F00
    {
        auto authorization = reader.ReadString();
        auto deleteRequestId = reader.ReadUUID();
        auto itemToDeleteId = reader.ReadUUID();
    }

    static void OnInventoryChangeItemName(PacketReader &reader) // 19B8F70
    {
        auto authorization = reader.ReadString();
        auto changeNameRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newName = reader.ReadString();
    }

    static void OnInventoryChangeItemState(PacketReader &reader) // 19B8FE0
    {
        auto authorization = reader.ReadString();
        auto changeStateRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newState = reader.ReadUint8();
    }

    static void OnInventoryModifyItemThumbnailAssetId(PacketReader &reader) // 19B9050
    {
        auto authorization = reader.ReadString();
        auto modifyThumbnailRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newThumbnailAssetId = reader.ReadString();
    }

    static void OnInventoryModifyItemCapabilities(PacketReader &reader) // 19B90C0
    {
        auto authorization = reader.ReadString();
        auto modifyCapabilitiesRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto capabilitiesToAdd = reader.ReadStringList();
        auto capabilitiesToRemove = reader.ReadStringList();
    }

    static void OnInventorySaveItem(PacketReader &reader) // 19B9130
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

    static void OnInventoryUpdateItemReply(PacketReader &reader) // 19B91A0
    {
        auto requestId = reader.ReadUUID();
        auto itemId = reader.ReadUUID();
        auto assetId = reader.ReadString();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadString();
        auto errorMsg = reader.ReadString();
    }

    static void OnInventoryItemUpload(PacketReader &reader) // 19B9210
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    static void OnInventoryItemUploadReply(PacketReader &reader) // 19B9280
    {
        auto productId = reader.ReadUUID();
        auto status = reader.ReadString();
        auto listingUrl = reader.ReadString();
        auto editUrl = reader.ReadString();
        auto errorCode = reader.ReadString();
        auto errorMessage = reader.ReadString();
    }

    static void OnInventoryCreateListing(PacketReader &reader) // 19B92F0
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
        auto bundleName = reader.ReadString();
    }

    static void OnInventoryCreateListingReply(PacketReader &reader) // 19B9360
    {
        auto canBeListed = reader.ReadUint8();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    static void OnBeginEditServerSpawn(PacketReader &reader) // 19B93D0
    {
        auto inventoryId = reader.ReadUUID();
        auto personaId = reader.ReadUUID(); // what's this message, from server or to server?
        auto serial = reader.ReadUint32();
        auto inventoryName = reader.ReadString();
    }

    static void OnEditServerSpawnReady(PacketReader &reader) // 19B9440
    {
        auto isValid = reader.ReadUint8();
        auto serial = reader.ReadUint32();
        auto instanceCount = reader.ReadUint32();
        auto inventoryName = reader.ReadString();
        auto instanceNames = reader.ReadStringList();
        auto folderNames = reader.ReadStringList();
    }
};

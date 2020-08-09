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
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case EditServerMessages::UserLogin: // TAG: 19B81B0
            {
                this->OnUserLogin(reader);
                break;
            }
            case EditServerMessages::UserLoginReply: // TAG: 19B8220
            {
                this->OnUserLoginReply(reader);
                break;
            }
            case EditServerMessages::AddUser: // TAG: 19B8290
            {
                this->OnAddUser(reader);
                break;
            }
            case EditServerMessages::RemoveUser: // TAG: 19B8300
            {
                this->OnRemoveUser(reader);
                break;
            }
            case EditServerMessages::OpenWorkspace: // TAG: 19B84C0
            {
                this->OnOpenWorkspace(reader);
                break;
            }
            case EditServerMessages::CloseWorkspace: // TAG: 19B8530
            {
                this->OnCloseWorkspace(reader);
                break;
            }
            case EditServerMessages::EditWorkspaceCommand: // TAG: 19B86F0
            {
                this->OnEditWorkspaceCommand(reader);
                break;
            }
            case EditServerMessages::SaveWorkspace: // TAG: 19B88E0
            {
                this->OnSaveWorkspace(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceReply: // TAG: 19B8950
            {
                this->OnSaveWorkspaceReply(reader);
                break;
            }
            case EditServerMessages::BuildWorkspace: // TAG: 19B89C0
            {
                this->OnBuildWorkspace(reader);
                break;
            }
            case EditServerMessages::UpdateWorkspaceClientBuiltBakeData: // TAG: 19B8A30
            {
                this->OnUpdateWorkspaceClientBuiltBakeData(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceCompileReply: // TAG: 19B8AA0
            {
                this->OnBuildWorkspaceCompileReply(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceProgressUpdate: // TAG: 19B8B10
            {
                this->OnBuildWorkspaceProgressUpdate(reader);
                break;
            }
            case EditServerMessages::BuildWorkspaceUploadReply: // TAG: 19B8B80
            {
                this->OnBuildWorkspaceuploadReply(reader);
                break;
            }
            case EditServerMessages::WorkspaceReadyReply: // TAG: 19B8BF0
            {
                this->OnWorkspaceReadyReply(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceSelectionToInventory: // TAG: 19B8DB0
            {
                this->OnSaveworkspaceSelectionToInventory(reader);
                break;
            }
            case EditServerMessages::SaveWorkspaceSelectionToInventoryReply: // TAG: 19B8E20
            {
                this->OnSaveWorkspaceSelectionToInventoryReply(reader);
                break;
            }
            case EditServerMessages::InventoryCreateItem: // TAG: 19B8E90
            {
                this->OnInventoryCreateItem(reader);
                break;
            }
            case EditServerMessages::InventoryDeleteItem: // TAG: 19B8F00
            {
                this->OnInventoryDeleteItem(reader);
                break;
            }
            case EditServerMessages::InventoryChangeItemName: // TAG: 19B8F70
            {
                this->OnInventoryChangeItemName(reader);
                break;
            }
            case EditServerMessages::InventoryChangeItemState: // TAG: 19B8FE0
            {
                this->OnInventoryChangeItemState(reader);
                break;
            }
            case EditServerMessages::InventoryModifyItemThumbnailAssetId: // TAG: 19B9050
            {
                this->OnInventoryModifyItemThumbnailAssetId(reader);
                break;
            }
            case EditServerMessages::InventoryModifyItemCapabilities: // TAG: 19B90C0
            {
                this->OnInventoryModifyItemCapabilities(reader);
                break;
            }
            case EditServerMessages::InventorySaveItem: // TAG: 19B9130
            {
                this->OnInventorySaveItem(reader);
                break;
            }
            case EditServerMessages::InventoryUpdateItemReply: // TAG: 19B91A0
            {
                this->OnInventoryUpdateItemReply(reader);
                break;
            }
            case EditServerMessages::InventoryItemUpload: // TAG: 19B9210
            {
                this->OnInventoryItemUpload(reader);
                break;
            }
            case EditServerMessages::InventoryItemUploadReply: // TAG: 19B9280
            {
                this->OnInventoryItemUploadReply(reader);
                break;
            }
            case EditServerMessages::InventoryCreateListing: // TAG: 19B92F0
            {
                this->OnInventoryCreateListing(reader);
                break;
            }
            case EditServerMessages::InventoryCreateListingReply: // TAG: 19B9360
            {
                this->OnInventoryCreateListingReply(reader);
                break;
            }
            case EditServerMessages::BeginEditServerSpawn: // TAG: 19B93D0
            {
                this->OnBeginEditServerSpawn(reader);
                break;
            }
            case EditServerMessages::EditServerSpawnReady: // TAG: 19B9440
            {
                this->OnEditServerSpawnReady(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnUserLogin(PacketReader &reader) // TAG: 19B81B0
    {
        auto authorization = reader.ReadString();
        auto secret = reader.ReadUint32();

        printf("[%s] this->OnUserLogin\n  authorization = %s\n  secret = %u\n",
            _isSender ? "OUT" : "IN",
            authorization.c_str(),
            secret
        );
    }

    void OnUserLoginReply(PacketReader &reader) // TAG: 19B8220
    {
        auto success = reader.ReadUint8();
        auto sessionId = reader.ReadUint32();
        auto editServerVersion = reader.ReadString();

        printf("[%s] EditServerMessages::LoginReply\n  success = %d\n  sessionId = %d\n  editServerVersion = %s\n",
            _isSender ? "OUT" : "IN",
            success,
            sessionId,
            editServerVersion.c_str()
        );
    }

    void OnAddUser(PacketReader &reader) // TAG: 19B8290
    {
        auto sessionId = reader.ReadUint32();
        auto userName = reader.ReadString();
        auto personaId = reader.ReadUUID();

        printf("[%s] EditServerMessages::AddUser\n  sessionId = %u\n  userName = %s\n  personaId = %s\n",
            _isSender ? "OUT" : "IN",
            sessionId,
            userName.c_str(),
            personaId.c_str()
        );
    }

    void OnRemoveUser(PacketReader &reader) // TAG: 19B8300
    {
        auto sessionId = reader.ReadUint32();
    }

    void OnOpenWorkspace(PacketReader &reader) // TAG: 19B84C0
    {
        auto worldSourceInventoryItemId = reader.ReadUUID();
        auto worldSourceResourceId = reader.ReadString();
    }

    void OnCloseWorkspace(PacketReader &reader) // TAG: 19B8530
    {
        auto workspaceId = reader.ReadUint32();
    }

    void OnEditWorkspaceCommand(PacketReader &reader) // TAG: 19B86F0
    {
        auto commandData = reader.ReadArray();
    }

    void OnSaveWorkspace(PacketReader &reader) // TAG: 19B88E0
    {
        auto authorization = reader.ReadString();
        auto sessionData = reader.ReadArray();
        auto sceneName = reader.ReadString();
    }

    void OnSaveWorkspaceReply(PacketReader &reader) // TAG: 19B8950
    {
        auto success = reader.ReadUint8();
        auto itemInventoryId = reader.ReadString();
        auto itemResourceId = reader.ReadString();
        auto itemName = reader.ReadString();
    }

    void OnBuildWorkspace(PacketReader &reader) // TAG: 19B89C0
    {
        auto authorization = reader.ReadString();
        auto sceneName = reader.ReadString();
        auto start = reader.ReadUint8();
    }

    void OnUpdateWorkspaceClientBuiltBakeData(PacketReader &reader) // TAG: 19B8A30
    {
        auto authorization = reader.ReadString();
        auto bakeData = reader.ReadArray();
    }

    void OnBuildWorkspaceCompileReply(PacketReader &reader) // TAG: 19B8AA0
    {
        auto compileStatus = reader.ReadUint8();
        auto isCanceled = reader.ReadUint8();
        auto errors = reader.ReadStringList();
        auto nonErrorMessage = reader.ReadStringList();
    }

    void OnBuildWorkspaceProgressUpdate(PacketReader &reader) // TAG: 19B8B10
    {
        auto stage = reader.ReadUint32();
        auto percent = reader.ReadFloat();
    }

    void OnBuildWorkspaceuploadReply(PacketReader &reader) // TAG: 19B8B80
    {
        auto success = reader.ReadUint8();
        auto worldDefinitionId = reader.ReadString();
    }

    void OnWorkspaceReadyReply(PacketReader &reader) // TAG: 19B8BF0
    {
        auto workspaceId = reader.ReadUint32();

        printf("[%s] EditServerMessages::WorkspaceReadyReply\n  workspaceId = %u\n",
            _isSender ? "OUT" : "IN",
            workspaceId
        );
    }

    void OnSaveworkspaceSelectionToInventory(PacketReader &reader) // TAG: 19B8DB0
    {
        auto authorization = reader.ReadString();
        auto itemName = reader.ReadString();
        auto selectionId = reader.ReadUint64();
        auto triggeringState = reader.ReadUint32();
        auto parentInstanceId = reader.ReadUint32();
    }

    void OnSaveWorkspaceSelectionToInventoryReply(PacketReader &reader) // TAG: 19B8E20
    {
        auto itemName = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto triggeringState = reader.ReadUint32();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadUint32();
    }

    void OnInventoryCreateItem(PacketReader &reader) // TAG: 19B8E90
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

    void OnInventoryDeleteItem(PacketReader &reader) // TAG: 19B8F00
    {
        auto authorization = reader.ReadString();
        auto deleteRequestId = reader.ReadUUID();
        auto itemToDeleteId = reader.ReadUUID();
    }

    void OnInventoryChangeItemName(PacketReader &reader) // TAG: 19B8F70
    {
        auto authorization = reader.ReadString();
        auto changeNameRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newName = reader.ReadString();
    }

    void OnInventoryChangeItemState(PacketReader &reader) // TAG: 19B8FE0
    {
        auto authorization = reader.ReadString();
        auto changeStateRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newState = reader.ReadUint8();
    }

    void OnInventoryModifyItemThumbnailAssetId(PacketReader &reader) // TAG: 19B9050
    {
        auto authorization = reader.ReadString();
        auto modifyThumbnailRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto newThumbnailAssetId = reader.ReadString();
    }

    void OnInventoryModifyItemCapabilities(PacketReader &reader) // TAG: 19B90C0
    {
        auto authorization = reader.ReadString();
        auto modifyCapabilitiesRequestId = reader.ReadUUID();
        auto itemToChangeId = reader.ReadUUID();
        auto capabilitiesToAdd = reader.ReadStringList();
        auto capabilitiesToRemove = reader.ReadStringList();
    }

    void OnInventorySaveItem(PacketReader &reader) // TAG: 19B9130
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

    void OnInventoryUpdateItemReply(PacketReader &reader) // TAG: 19B91A0
    {
        auto requestId = reader.ReadUUID();
        auto itemId = reader.ReadUUID();
        auto assetId = reader.ReadString();
        auto requestResult = reader.ReadUint32();
        auto statusCode = reader.ReadString();
        auto errorMsg = reader.ReadString();
    }

    void OnInventoryItemUpload(PacketReader &reader) // TAG: 19B9210
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    void OnInventoryItemUploadReply(PacketReader &reader) // TAG: 19B9280
    {
        auto productId = reader.ReadUUID();
        auto status = reader.ReadString();
        auto listingUrl = reader.ReadString();
        auto editUrl = reader.ReadString();
        auto errorCode = reader.ReadString();
        auto errorMessage = reader.ReadString();
    }

    void OnInventoryCreateListing(PacketReader &reader) // TAG: 19B92F0
    {
        auto authorization = reader.ReadString();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
        auto bundleName = reader.ReadString();
    }

    void OnInventoryCreateListingReply(PacketReader &reader) // TAG: 19B9360
    {
        auto canBeListed = reader.ReadUint8();
        auto itemId = reader.ReadUUID();
        auto itemName = reader.ReadString();
        auto categoryName = reader.ReadString();
    }

    void OnBeginEditServerSpawn(PacketReader &reader) // TAG: 19B93D0
    {
        auto inventoryId = reader.ReadUUID();
        auto personaId = reader.ReadUUID(); // what's this message, from server or to server?
        auto serial = reader.ReadUint32();
        auto inventoryName = reader.ReadString();

        printf("[%s] this->OnBeginEditServerSpawn\n  inventoryId = %s\n  personaId = %s\n  serial = %u\n inventoryName = %s\n",
            _isSender ? "OUT" : "IN",
            inventoryId.c_str(),
            personaId.c_str(),
            serial,
            inventoryName.c_str()
        );
    }

    void OnEditServerSpawnReady(PacketReader &reader) // TAG: 19B9440
    {
        auto isValid = reader.ReadUint8();
        auto serial = reader.ReadUint32();
        auto instanceCount = reader.ReadUint32();
        auto inventoryName = reader.ReadString();
        auto instanceNames = reader.ReadStringList();
        auto folderNames = reader.ReadStringList();
    }
};

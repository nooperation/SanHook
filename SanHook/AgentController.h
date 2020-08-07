#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnAgentControllerMessageCharacterControllerInput(PacketReader &reader);
//void OnAgentControllerMessageWarpCharacter(PacketReader &reader);
//void OnAgentControllerMessageCharacterControlPointInput(PacketReader &reader);
//void OnAgentControllerMessageCharacterControllerInputReliable(PacketReader &reader);
//void OnAgentControllerMessageCharacterControlPointInputReliable(PacketReader &reader);
//void OnAgentControllerMessageCharacterIKPose(PacketReader &reader);
//void OnAgentControllerMessageCharacterIKPoseDelta(PacketReader &reader);
//void OnAgentControllerMessageCreateSpeechGraphicsPlayer(PacketReader &reader);
//void OnAgentControllerMessageObjectInteractionUpdate(PacketReader &reader);
//void OnAgentControllerMessageControlPoint(PacketReader &reader);
//void OnAgentControllerMessageRequestWarpCharacter();
//void OnAgentControllerMessageAgentPlayAnimation(PacketReader &reader);
//void OnAgentControllerMessageRequestAgentPlayAnimation();
//void OnAgentControllerMessageRequestBehaviorStateUpdate();
//void OnAgentControllerMessageAttachToCharacterNode(PacketReader &reader);
//void OnAgentControllerMessageDetachFromCharacterNode(PacketReader &reader);
//void OnAgentControllerMessageRequestDetachFromCharacterNode();
//void OnAgentControllerMessageSetCharacterNodePhysics(PacketReader &reader);
//void OnAgentControllerMessageWarpCharacterNode(PacketReader &reader);
//void OnAgentControllerMessageCharacterIKBone(PacketReader &reader);
//void OnAgentControllerMessageCharacterIKBoneDelta(PacketReader &reader);
//void OnAgentControllerMessageObjectInteraction(PacketReader &reader);
//void OnAgentControllerMessageObjectInteractionPromptUpdate(PacketReader &reader);
//void OnAgentControllerMessageObjectInteractionCreate(PacketReader &reader);
//void OnAgentControllerMessageRequestSitOnObject();
//void OnAgentControllerMessageSitOnObject(PacketReader &reader);
//void OnAgentControllerMessageExitSit(PacketReader &reader);
//void OnAgentControllerMessageSetAgentFiltersBody(PacketReader &reader);
//void OnAgentControllerMessageRequestSetAgentfiltersBody();
//void OnAgentControllerMessageSetCharacterUserProperty(PacketReader &reader);
//void OnAgentControllerMessageRequestSpawnItem(PacketReader &reader);
//void OnAgentControllerMessageRequestDeleteLatestSpawn(PacketReader &reader);
//void OnAgentControllerMessageRequestDeleteAllSpawns(PacketReader &reader);
//void OnAgentControllerMessagePlayAnimation(PacketReader &reader);

class AgentController : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        if (messageId == AgentControllerMessages::ControlPoint)  // 170F790
        {
            AgentController::OnControlPoint(reader);
        }
        else if (messageId == AgentControllerMessages::WarpCharacter) // 170F800
        {
            AgentController::OnWarpCharacter(reader);
        }
        else if (messageId == AgentControllerMessages::RequestWarpCharacter)  // 170F870
        {
            AgentController::OnRequestWarpCharacter(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControlPointInput)  // 170F8E0
        {
            AgentController::OnCharacterControlPointInput(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControlPointInputReliable)  // 170F950
        {
            AgentController::OnCharacterControlPointInputReliable(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControllerInput) // 170F9C0
        {
            AgentController::OnCharacterControllerInput(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterControllerInputReliable) // 170FA30
        {
            AgentController::OnCharacterControllerInputReliable(reader);
        }
        else if (messageId == AgentControllerMessages::AgentPlayAnimation)  // 170FAA0
        {
            AgentController::OnAgentPlayAnimation(reader);
        }
        else if (messageId == AgentControllerMessages::RequestAgentPlayAnimation)  // 170FB10
        {
            AgentController::OnRequestAgentPlayAnimation(reader);
        }
        else if (messageId == AgentControllerMessages::RequestBehaviorStateUpdate)  // 170FB80
        {
            AgentController::OnRequestBehaviorStateUpdate(reader);
        }
        else if (messageId == AgentControllerMessages::AttachToCharacterNode)  // 170FBF0
        {
            AgentController::OnAttachToCharacterNode(reader);
        }
        else if (messageId == AgentControllerMessages::DetachFromCharacterNode)  // 170FC60
        {
            AgentController::OnDetachFromCharacterNode(reader);
        }
        else if (messageId == AgentControllerMessages::RequestDetachFromCharacterNode)  // 170FCD0
        {
            AgentController::OnRequestDetachFromCharacterNode(reader);
        }
        else if (messageId == AgentControllerMessages::SetCharacterNodePhysics)  // 170FD40
        {
            AgentController::OnSetCharacterNodePhysics(reader);
        }
        else if (messageId == AgentControllerMessages::WarpCharacterNode)  // 170FDB0
        {
            AgentController::OnWarpCharacterNode(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterIKBone)  // 170FE20
        {
            AgentController::OnCharacterIKBone(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterIKPose)  // 170FE90
        {
            AgentController::OnCharacterIKPose(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterIKBoneDelta)  // 1710040
        {
            AgentController::OnCharacterIKBoneDelta(reader);
        }
        else if (messageId == AgentControllerMessages::CharacterIKPoseDelta)  // 17100B0
        {
            AgentController::OnCharacterIKPoseDelta(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteraction)  // 1710260
        {
            AgentController::OnObjectInteraction(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionUpdate)  // 17102D0
        {
            AgentController::OnObjectInteractionUpdate(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionPromptUpdate)  // 1710340
        {
            AgentController::OnObjectInteractionPromptUpdate(reader);
        }
        else if (messageId == AgentControllerMessages::ObjectInteractionCreate)  // 17103B0
        {
            AgentController::OnObjectInteractionCreate(reader);
        }
        else if (messageId == AgentControllerMessages::RequestSitOnObject)  // 1710420
        {
            AgentController::OnRequestSitOnObject(reader);
        }
        else if (messageId == AgentControllerMessages::SitOnObject)  // 1710490
        {
            AgentController::OnSitOnObject(reader);
        }
        else if (messageId == AgentControllerMessages::ExitSit)  // 1710500
        {
            AgentController::OnExitSit(reader);
        }
        else if (messageId == AgentControllerMessages::SetAgentFiltersBody)  // 1710570
        {
            AgentController::OnSetAgentFiltersBody(reader);
        }
        else if (messageId == AgentControllerMessages::RequestSetAgentFiltersBody)  // 17105E0
        {
            AgentController::OnRequestSetAgentfiltersBody(reader);
        }
        else if (messageId == AgentControllerMessages::SetCharacterUserProperty)  // 1710650
        {
            AgentController::OnSetCharacterUserProperty(reader);
        }
        else if (messageId == AgentControllerMessages::CreateSpeechGraphicsPlayer) // 0x158B2580  // 17106C0
        {
            AgentController::OnCreateSpeechGraphicsPlayer(reader);
        }
        else if (messageId == AgentControllerMessages::RequestSpawnItem)  // 1710730
        {
            AgentController::OnRequestSpawnItem(reader);
        }
        else if (messageId == AgentControllerMessages::RequestDeleteLatestSpawn)  // 17107A0
        {
            AgentController::OnRequestDeleteLatestSpawn(reader);
        }
        else if (messageId == AgentControllerMessages::RequestDeleteAllSpawns)  // 1710810
        {
            AgentController::OnRequestDeleteAllSpawns(reader);
        }
        else
        {
            return false;
        }

        return true;
    }

    static void OnCharacterControllerInput(PacketReader &reader) // 170F9C0
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto jumpState = reader.ReadUint8();
        auto jumpBtnPressed = reader.ReadUint8();

        auto moveRight = reader.ReadBits(12); //  - 2047) * 0.0004885197850512946
        auto moveForward = reader.ReadBits(12); // - 2047) * 0.0004885197850512946
        auto cameraYaw = reader.ReadBits(13); // - 4095) * 0.0002442002442002442
        auto cameraPitch = reader.ReadBits(13); // - 4095) * 0.0002442002442002442
        auto behaviorYawDelta = reader.ReadBits(11); // - 1023) * 0.0009775171065493646
        auto behaviorPitchDelta = reader.ReadBits(11); // - 1023) * 0.0009775171065493646
        auto characterForward = reader.ReadBits(15); // - 0x3FFF) * 0.00006103888176768602
        auto cameraForward = reader.ReadBits(28);

        /*
        auto v34 = reader.ReadBits(2);
        auto v33 = reader.ReadBits(1);
        auto v32 = reader.ReadBits(1);
        
        if(v32) {
            v32 = (reader.readBits(12) - 2047) * 0.0004885197850512946 * 0.1;
            v32 = (reader.readBits(12) - 2047) * 0.0004885197850512946 * 0.1;
        }
        else {
            v32 = (reader.readBits(12) - 2047) * 0.0004885197850512946 * 0.70710802;
            v32 = (reader.readBits(12) - 2047) * 0.0004885197850512946 * 0.70710802;
        }

        if(v33) {
            shuffle -60
        }

        if(v34 == 0) {
            // ??
            shuffle -86
            shuffle 0

            xor (shuffle 85)
            xor (shuffle -1)
        }
        else if(v34 == 1) {
            // ??
            shuffle -86
            shuffle -1

            xor (shuffle 85)
            xor (shuffle 0)
        }
        else if(v34 == 2) {
            shuffle -1
            shuffle -86
            shuffle 85
            xor
        }
        else {
            shuffle -1
            
        }
        */

        //printf("OnCharacterControllerInput: Frame = %llu | Agent = %u | jumpState = %u | jump = %u\n",
        //    frame,
        //    agentControllerId,
        //    jumpState,
        //    jumpBtnPressed
        //);
    }

    static void OnObjectInteractionUpdate(PacketReader &reader)  // 17102D0
    {
        auto frame = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();
        auto objectId = reader.ReadUint32();
        auto enabled = reader.ReadUint8();

        printf("AnimationComponentMessages::BehaviorInitializationData:\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  enabled = %u\n",
            frame,
            clusterId,
            objectId,
            enabled
        );
    }

    static void OnCreateSpeechGraphicsPlayer(PacketReader &reader) // 17106C0
    {
        auto agentControllerId = reader.ReadUint32();

        auto numElements = reader.ReadUint32();
        auto sinkConfigData = reader.ReadBytes(numElements);

        printf("OnCreateSpeechGraphicsPlayer:\n  sinkConfigData = Array[%u]\n",
            numElements
        );
    }

    static void OnCharacterIKPoseDelta(PacketReader &reader)  // 17100B0
    {
        auto agentControllerId = reader.ReadUint32();
        auto frame = reader.ReadUint64();

        auto boneRotationsLength = reader.ReadUint32();
        //for (size_t i = 0; i < boneRotationsLength; i++)
        //{
        //    auto boneIndex = reader.ReadBits(6);
        //    auto localOrientation = reader.ReadBits(25);
        //}

        //auto rootBoneTranslationDelta = reader.ReadBits(27);

        printf("AgentControllerMessages::CharacterIKPoseDelta:\n  agentControllerId = %u\n  frame = %llu\n  boneRotationsLength = Array[%u]\n  rootBoneTranslationDelta = Array[27]\n",
            agentControllerId,
            frame,
            boneRotationsLength
        );
    }

    static void OnCharacterIKPose(PacketReader &reader)   // 170FE90
    {
        // TODO: Nope, not handling this one

        auto agentControllerId = reader.ReadUint32();
        auto frame = reader.ReadUint64();
        auto boneRotationsLength = reader.ReadUint32();

        /*
        for (size_t i = 0; i < boneRotationsLength; i++)
        {
            auto boneIndex = reader.ReadBits(6);
            auto localOrientation = reader.ReadBits(40);
        }

        auto rootBoneTranslation = reader.ReadBits(42);
        */

        printf("AgentControllerMessages::CharacterIKPose:\n  agentControllerId = %u\n  frame = %llu\n",
            agentControllerId,
            frame
        );
    }

    static void OnCharacterControlPointInputReliable(PacketReader &reader)  // 170F950
    {
        OnCharacterControlPointInput(reader);
    }

    static void OnCharacterControllerInputReliable(PacketReader &reader) // 170FA30
    {
        OnCharacterControllerInput(reader);
    }

    static void OnCharacterControlPointInput(PacketReader &reader)  // 170F8E0
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        auto controlPointsLength = reader.ReadUint32();

        //auto leftIndexTrigger = reader.ReadBits(8);
        //auto rightIndexTrigger = reader.ReadBits(8);
        //auto leftGripTrigger = reader.ReadBits(8);
        //auto rightGripTrigger = reader.ReadBits(8);
        //auto leftTouches = reader.ReadBits(7);
        //auto rightTouches = reader.ReadBits(7);
        //auto indexTriggerControlsHand = reader.ReadBits(1);
        //auto leftHandIsHolding = reader.ReadBits(1);
        //auto rightHandIsHolding = reader.ReadBits(1);

        printf("AgentControllerMessages::CharacterControlPointInput:\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }

    static void OnWarpCharacter(PacketReader &reader) // 170F800
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto position_x = reader.ReadFloat();
        auto position_y = reader.ReadFloat();
        auto position_z = reader.ReadFloat();
        auto rotation_x = reader.ReadFloat();
        auto rotation_y = reader.ReadFloat();
        auto rotation_z = reader.ReadFloat();
        auto rotation_w = reader.ReadFloat();

        printf("AgentControllerMessages::WarpCharacter:\n  frame = %llu\n  agentControllerId = %u\n  position = <%f, %f, %f>\n  rotation = <%f, %f, %f, %f>\n",
            frame,
            agentControllerId,
            position_x,
            position_y,
            position_z,
            rotation_x,
            rotation_y,
            rotation_z,
            rotation_w
        );
    }

    static void OnControlPoint(PacketReader &reader)  // 170F790
    {
        auto position = reader.ReadBits(0x30);
        auto orientation = reader.ReadBits(0x28);
        auto enabled = reader.ReadBits(1);
        auto controlPointType = reader.ReadBits(4);
    }

    static void OnRequestWarpCharacter(PacketReader &reader)  // 170F870
    {
        // double check...
        OnWarpCharacter(reader);
    }

    static void OnAgentPlayAnimation(PacketReader &reader)   // 170FAA0
    {
        auto agentControllerId = reader.ReadUint32();
    }

    static void OnRequestAgentPlayAnimation(PacketReader &reader)  // 170FB10
    {
        // double check...
        OnAgentPlayAnimation(reader);
    }

    static void OnRequestBehaviorStateUpdate(PacketReader &reader) // 170FB80
    {
        // double check...
        AnimationComponent::OnBehaviorStateUpdate(reader);
    }

    static void OnAttachToCharacterNode(PacketReader &reader)   // 170FBF0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto attachmentOffsetPosition = reader.ReadVectorF(3);
        auto attachmentOffsetOrientation = reader.ReadVectorF(4);
        auto nodeType = reader.ReadUint8();
        auto ownershipWatermark = reader.ReadUint8();
        auto broadcastToSelf = reader.ReadUint8();
    }

    static void OnDetachFromCharacterNode(PacketReader &reader)  // 170FC60
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto bodyPosition = reader.ReadVectorF(3);
        auto botyOrientation = reader.ReadVectorF(4);
        auto bodyVelocity = reader.ReadVectorF(3);
        auto bodyAngularVelocity = reader.ReadVectorF(3);
        auto nodeType = reader.ReadUint8();
    }

    static void OnRequestDetachFromCharacterNode(PacketReader &reader)  // 170FCD0
    {
        OnDetachFromCharacterNode(reader);
    }

    static void OnSetCharacterNodePhysics(PacketReader &reader)  // 170FD40
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto nodeType = reader.ReadUint8();
        auto collisionsEnabled = reader.ReadUint8();
        auto broadcastToSelf = reader.ReadUint8();
    }

    static void OnWarpCharacterNode(PacketReader &reader) // 170FDB0
    {
        auto agentControllerId = reader.ReadUint32();
        auto nodeType = reader.ReadUint32(); /// why is this node type 4bytes?
    }

    static void OnCharacterIKBone(PacketReader &reader) // 170FE20
    {
        auto boneIndex = reader.ReadBits(6);
        auto localOrientation = reader.ReadBits(0x28);
    }

    static void OnCharacterIKBoneDelta(PacketReader &reader)  // 1710040
    {
        auto boneIndex = reader.ReadBits(6);
        auto localOrientation = reader.ReadBits(0x19);
    }

    static void OnObjectInteraction(PacketReader &reader)   // 1710260
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto objectId = reader.ReadUint32();
        auto targetedPosition = reader.ReadVectorF(3);
        auto targetedNormal = reader.ReadVectorF(3);
        auto origin = reader.ReadVectorF(3);
        auto controlPointType = reader.ReadBits(4);
    }

    static void OnObjectInteractionPromptUpdate(PacketReader &reader)   // 1710340
    {
        auto frame = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();
        auto objectId = reader.ReadUint32();
        auto prompt = reader.ReadString();

        printf("AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n",
            frame,
            clusterId,
            objectId,
            prompt.c_str()
        );
    }

    static void OnObjectInteractionCreate(PacketReader &reader)  // 17103B0
    {
        auto frame = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();
        auto objectId = reader.ReadUint32();
        auto prompt = reader.ReadString();
        auto enabled = reader.ReadUint8();

        printf("AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n  enabled = %u\n",
            frame,
            clusterId,
            objectId,
            prompt.c_str(),
            enabled
        );
    }

    static void OnRequestSitOnObject(PacketReader &reader)  // 1710420
    {
        OnSitOnObject(reader);
    }

    static void OnSitOnObject(PacketReader &reader)  // 1710490
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto componentId = reader.ReadUint64();
        auto ownershipWatermark = reader.ReadUint8();
        auto skipAnimation = reader.ReadUint8();

        printf("AgentControllerMessages::SitOnObject\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n  ownershipWatermark = %u\n  skipAnimation = %u\n",
            frame,
            agentControllerId,
            componentId,
            ownershipWatermark,
            skipAnimation
        );
    }

    static void OnExitSit(PacketReader &reader)  // 1710500
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto componentId = reader.ReadUint64();
        auto skipAnimation = reader.ReadBits(1);
        auto skipExitTeleport = reader.ReadBits(1);

        printf("AgentControllerMessages::ExitSit\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n",
            frame,
            agentControllerId,
            componentId
        );
    }

    static void OnSetAgentFiltersBody(PacketReader &reader) // 1710570
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto componentId = reader.ReadUint64();
        auto filterBody = reader.ReadUint8();

        printf("AgentControllerMessages::SetAgentFiltersBody\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n  filterBody = %u\n",
            frame,
            agentControllerId,
            componentId,
            filterBody
        );
    }

    static void OnRequestSetAgentfiltersBody(PacketReader &reader)  // 17105E0
    {
        OnSetAgentFiltersBody(reader);
    }

    static void OnSetCharacterUserProperty(PacketReader &reader)  // 1710650
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto value = reader.ReadFloat();
        auto propertyType = reader.ReadUint8();

        printf("AgentControllerMessages::SetCharacterUserProperty\n  frame = %llu\n  agentControllerId = %u\n  value = %f\n  propertyType = %u\n",
            frame,
            agentControllerId,
            value,
            propertyType
        );
    }

    static void OnRequestSpawnItem(PacketReader &reader) // 1710730
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto resourceId = reader.ReadUUID();
        auto attachmentNode = reader.ReadUint8();
        auto spawnPosition = reader.ReadBits(0x4E);
        auto spawnOrientation = reader.ReadBits(0x2B);

        printf("AgentControllerMessages::OnRequestSpawnItem\n  frame = %llu\n  agentControllerId = %u\n  resourceId = %s\n  attachmentNode = %u\n",
            frame,
            agentControllerId,
            resourceId.c_str(),
            attachmentNode
        );
    }

    static void OnRequestDeleteLatestSpawn(PacketReader &reader)  // 17107A0
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("AgentControllerMessages::RequestDeleteLatestSpawn\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }

    static void OnRequestDeleteAllSpawns(PacketReader &reader)  // 1710810
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("AgentControllerMessages::RequestDeleteAllSpawns\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }

    static void OnPlayAnimation(PacketReader &reader) // 1581210
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto resourceId = reader.ReadUUID();
        auto playbackSpeed = reader.ReadBits(16);
        auto skeletonType = reader.ReadBits(2);
        auto animationType = reader.ReadBits(3);
        auto playbackMode = reader.ReadBits(3);

        printf("AgentControllerMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
            frame,
            componentId,
            resourceId.c_str()
        );
    }
};

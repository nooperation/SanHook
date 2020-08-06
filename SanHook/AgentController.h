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
//void OnAgentControllerMessageAgentPlayanimation(PacketReader &reader);
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

class AgentController
{
public:
    static void OnCharacterControllerInput(PacketReader &reader)
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto jumpState = reader.ReadUint8();
        auto jumpBtnPressed = reader.ReadUint8();

        printf("OnCharacterControllerInput: Frame = %llu | Agent = %u | jumpState = %u | jump = %u\n",
            frame,
            agentControllerId,
            jumpState,
            jumpBtnPressed
        );
        /*
        auto moveRight = ;
        auto moveForward = ;
        auto cameraYaw = ;
        auto cameraPitch = ;
        auto cameraYaw = ;
        auto cameraYaw = ;
        auto cameraYaw = ;
        auto cameraYaw = ;
        ... TBD, weird packed float stuff again
        */
    }

    static void OnObjectInteractionUpdate(PacketReader &reader)
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

    static void OnCreateSpeechGraphicsPlayer(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();

        auto numElements = reader.ReadUint32();
        auto sinkConfigData = reader.ReadBytes(numElements);

        printf("OnCreateSpeechGraphicsPlayer:\n  sinkConfigData = Array[%u]\n",
            numElements
        );
    }

    static void OnCharacterIKPoseDelta(PacketReader &reader)
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

    static void OnCharacterIKPose(PacketReader &reader)
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

    static void OnCharacterControlPointInputReliable(PacketReader &reader)
    {
        printf("AgentControllerMessages::CharacterControlPointInputReliable\n");
        OnCharacterControlPointInput(reader);
    }

    static void OnCharacterControllerInputReliable(PacketReader &reader)
    {
        printf("AgentControllerMessages::CharacterControllerInputReliable\n");
        OnCharacterControllerInput(reader);
    }

    static void OnCharacterControlPointInput(PacketReader &reader)
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

    static void OnWarpCharacter(PacketReader &reader)
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


    static void OnControlPoint(PacketReader &reader)
    {
        auto position = reader.ReadBits(0x30);
        auto orientation = reader.ReadBits(0x28);
        auto enabled = reader.ReadBits(1);
        auto controlPointType = reader.ReadBits(4);
    }

    static void OnRequestWarpCharacter(PacketReader &reader)
    {
        // double check...
        OnWarpCharacter(reader);
    }

    static void OnAgentPlayanimation(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();
    }

    static void OnRequestAgentPlayAnimation(PacketReader &reader)
    {
        // double check...
        OnAgentPlayanimation(reader);
    }

    static void OnRequestBehaviorStateUpdate(PacketReader &reader)
    {
        // double check...
        AnimationComponent::OnBehaviorStateUpdate(reader);
    }

    static void OnAttachToCharacterNode(PacketReader &reader)
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

    static void OnDetachFromCharacterNode(PacketReader &reader)
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

    static void OnRequestDetachFromCharacterNode(PacketReader &reader)
    {
        OnDetachFromCharacterNode(reader);
    }

    static void OnSetCharacterNodePhysics(PacketReader &reader)
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto nodeType = reader.ReadUint8();
        auto collisionsEnabled = reader.ReadUint8();
        auto broadcastToSelf = reader.ReadUint8();
    }

    static void OnWarpCharacterNode(PacketReader &reader)
    {
        auto agentControllerId = reader.ReadUint32();
        auto nodeType = reader.ReadUint32(); /// why is this node type 4bytes?
    }

    static void OnCharacterIKBone(PacketReader &reader)
    {
        auto boneIndex = reader.ReadBits(6);
        auto localOrientation = reader.ReadBits(0x28);
    }

    static void OnCharacterIKBoneDelta(PacketReader &reader)
    {
        auto boneIndex = reader.ReadBits(6);
        auto localOrientation = reader.ReadBits(0x19);
    }

    static void OnObjectInteraction(PacketReader &reader)
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto objectId = reader.ReadUint32();
        auto targetedPosition = reader.ReadVectorF(3);
        auto targetedNormal = reader.ReadVectorF(3);
        auto origin = reader.ReadVectorF(3);
        auto controlPointType = reader.ReadBits(4);
    }

    static void OnObjectInteractionPromptUpdate(PacketReader &reader)
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

    static void OnObjectInteractionCreate(PacketReader &reader)
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

    static void OnRequestSitOnObject(PacketReader &reader)
    {
        OnSitOnObject(reader);
    }

    static void OnSitOnObject(PacketReader &reader)
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

    static void OnExitSit(PacketReader &reader)
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

    static void OnSetAgentFiltersBody(PacketReader &reader)
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

    static void OnRequestSetAgentfiltersBody(PacketReader &reader)
    {
        OnSetAgentFiltersBody(reader);
    }

    static void OnSetCharacterUserProperty(PacketReader &reader)
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

    static void OnRequestSpawnItem(PacketReader &reader)
    {

        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto resourceId = reader.ReadUUID();
        auto attachmentNode = reader.ReadUint8();
        auto spawnPosition = reader.ReadBits(0x4E);
        auto spawnOrientation = reader.ReadBits(0x2B);
    }

    static void OnRequestDeleteLatestSpawn(PacketReader &reader)
    {

        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("AgentControllerMessages::RequestDeleteLatestSpawn\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }

    static void OnRequestDeleteAllSpawns(PacketReader &reader)
    {
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("AgentControllerMessages::RequestDeleteAllSpawns\n  frame = %llu\n  agentControllerId = %u\n",
            frame,
            agentControllerId
        );
    }

    static void OnPlayAnimation(PacketReader &reader)
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


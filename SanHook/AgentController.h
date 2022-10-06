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
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case AgentControllerMessages::ControlPoint:  // TAG: 170F790
            {
                //this->OnControlPoint(reader);
                break;
            }
            case AgentControllerMessages::WarpCharacter: // TAG: 170F800
            {
                //this->OnWarpCharacter(reader);
                break;
            }
            case AgentControllerMessages::RequestWarpCharacter:  // TAG: 170F870
            {
                //this->OnRequestWarpCharacter(reader);
                break;
            }
            case AgentControllerMessages::CharacterControlPointInput:  // TAG: 170F8E0
            {
                //this->OnCharacterControlPointInput(reader);
                break;
            }
            case AgentControllerMessages::CharacterControlPointInputReliable:  // TAG: 170F950
            {
                //this->OnCharacterControlPointInputReliable(reader);
                break;
            }
            case AgentControllerMessages::CharacterControllerInput: // TAG: 170F9C0
            {
                //this->OnCharacterControllerInput(reader);
                break;
            }
            case AgentControllerMessages::CharacterControllerInputReliable: // TAG: 170FA30
            {
               // this->OnCharacterControllerInputReliable(reader);
                break;
            }
            case AgentControllerMessages::AgentPlayAnimation:  // TAG: 170FAA0
            {
                this->OnAgentPlayAnimation(reader);
                break;
            }
            case AgentControllerMessages::RequestAgentPlayAnimation:  // TAG: 170FB10
            {
               // this->OnRequestAgentPlayAnimation(reader);
                break;
            }
            case AgentControllerMessages::RequestBehaviorStateUpdate:  // TAG: 170FB80
            {
                this->OnRequestBehaviorStateUpdate(reader);
                break;
            }
            case AgentControllerMessages::AttachToCharacterNode:  // TAG: 170FBF0
            {
                this->OnAttachToCharacterNode(reader);
                break;
            }
            case AgentControllerMessages::DetachFromCharacterNode:  // TAG: 170FC60
            {
                this->OnDetachFromCharacterNode(reader);
                break;
            }
            case AgentControllerMessages::RequestDetachFromCharacterNode:  // TAG: 170FCD0
            {
                this->OnRequestDetachFromCharacterNode(reader);
                break;
            }
            case AgentControllerMessages::SetCharacterNodePhysics:  // TAG: 170FD40
            {
                this->OnSetCharacterNodePhysics(reader);
                break;
            }
            case AgentControllerMessages::WarpCharacterNode:  // TAG: 170FDB0
            {
                this->OnWarpCharacterNode(reader);
                break;
            }
            case AgentControllerMessages::CharacterIKBone:  // TAG: 170FE20
            {
                this->OnCharacterIKBone(reader);
                break;
            }
            case AgentControllerMessages::CharacterIKPose:  // TAG: 170FE90
            {
                this->OnCharacterIKPose(reader);
                break;
            }
            case AgentControllerMessages::CharacterIKBoneDelta:  // TAG: 1710040
            {
                this->OnCharacterIKBoneDelta(reader);
                break;
            }
            case AgentControllerMessages::CharacterIKPoseDelta:  // TAG: 17100B0
            {
                this->OnCharacterIKPoseDelta(reader);
                break;
            }
            case AgentControllerMessages::ObjectInteraction:  // TAG: 1710260
            {
                this->OnObjectInteraction(reader);
                break;
            }
            case AgentControllerMessages::ObjectInteractionUpdate:  // TAG: 17102D0
            {
                this->OnObjectInteractionUpdate(reader);
                break;
            }
            case AgentControllerMessages::ObjectInteractionPromptUpdate:  // TAG: 1710340
            {
                this->OnObjectInteractionPromptUpdate(reader);
                break;
            }
            case AgentControllerMessages::ObjectInteractionCreate:  // TAG: 17103B0
            {
                this->OnObjectInteractionCreate(reader);
                break;
            }
            case AgentControllerMessages::RequestSitOnObject:  // TAG: 1710420
            {
                this->OnRequestSitOnObject(reader);
                break;
            }
            case AgentControllerMessages::SitOnObject:  // TAG: 1710490
            {
                this->OnSitOnObject(reader);
                break;
            }
            case AgentControllerMessages::ExitSit:  // TAG: 1710500
            {
                this->OnExitSit(reader);
                break;
            }
            case AgentControllerMessages::SetAgentFiltersBody:  // TAG: 1710570
            {
                this->OnSetAgentFiltersBody(reader);
                break;
            }
            case AgentControllerMessages::RequestSetAgentFiltersBody:  // TAG: 17105E0
            {
                this->OnRequestSetAgentFiltersBody(reader);
                break;
            }
            case AgentControllerMessages::SetCharacterUserProperty:  // TAG: 1710650
            {
                this->OnSetCharacterUserProperty(reader);
                break;
            }
            // REMOVED 40.11.0.1810696  (2020-08-13)
            //case AgentControllerMessages::CreateSpeechGraphicsPlayer: // TAG: 17106C0
            //{
            //    this->OnCreateSpeechGraphicsPlayer(reader);
            //    break;
            //}
            case AgentControllerMessages::RequestSpawnItem:  // TAG: 1710730
            {
                this->OnRequestSpawnItem(reader);
                break;
            }
            case AgentControllerMessages::RequestDeleteLatestSpawn:  // TAG: 17107A0
            {
                this->OnRequestDeleteLatestSpawn(reader);
                break;
            }
            case AgentControllerMessages::RequestDeleteAllSpawns:  // TAG: 1710810
            {
                this->OnRequestDeleteAllSpawns(reader);
                break;
            }
            case AgentControllerMessages::UserReaction: // Tag: 16D52F0 
            {
                // Added 2020-09-09
                this->OnUserReaction(reader);
                break;
            }
            default:
            {
            return false;
            }
        }

        return true;
    }


    void OnCharacterControllerInput(PacketReader &reader) // TAG: 170F9C0
    {
        //if (myControllerId != 0xffffffff)
        //{
        //    auto buffer = reader.GetBuffer();

        //    auto pAgentControllerId = (uint32_t *)&buffer[12];
        //    *pAgentControllerId = myControllerId;
        //}

        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto jumpState = reader.ReadUint8();
        //auto jumpBtnPressed = reader.ReadUint8();

        //auto moveRight = reader.ReadBits(12); //  - 2047) * 0.0004885197850512946
        //auto moveForward = reader.ReadBits(12); // - 2047) * 0.0004885197850512946
        //auto cameraYaw = reader.ReadBits(13); // - 4095) * 0.0002442002442002442
        //auto cameraPitch = reader.ReadBits(13); // - 4095) * 0.0002442002442002442
        //auto behaviorYawDelta = reader.ReadBits(11); // - 1023) * 0.0009775171065493646
        //auto behaviorPitchDelta = reader.ReadBits(11); // - 1023) * 0.0009775171065493646
        //auto characterForward = reader.ReadBits(15); // - 0x3FFF) * 0.00006103888176768602
        //auto cameraForward = reader.ReadBits(28);

        

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

    void OnObjectInteractionUpdate(PacketReader &reader)  // TAG: 17102D0
    {
        //auto frame = reader.ReadUint64();
        //auto clusterId = reader.ReadUint32();
        //auto objectId = reader.ReadUint32();
        //auto enabled = reader.ReadUint8();

        //if (_isVerbose)
        //{
        //    printf("[%s] AnimationComponentMessages::BehaviorInitializationData:\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  enabled = %u\n",
        //        _isSender ? "OUT" : "IN",
        //        frame,
        //        clusterId,
        //        objectId,
        //        enabled
        //    );
        //}
    }

    void OnCreateSpeechGraphicsPlayer(PacketReader &reader) // TAG: 17106C0
    {
        // REMOVED 40.11.0.1810696  (2020-08-13)

       // auto agentControllerId = reader.ReadUint32();
       // auto sinkConfigData = reader.ReadArray();

        //printf("OnCreateSpeechGraphicsPlayer:\n  sinkConfigData = Array[%u]\n",
        //    numElements
        //);
    }

    void OnCharacterIKPoseDelta(PacketReader &reader)  // TAG: 17100B0
    {
       // auto agentControllerId = reader.ReadUint32();
       // auto frame = reader.ReadUint64();

        //auto boneRotationsLength = reader.ReadUint32();
        //for (size_t i = 0; i < boneRotationsLength; i++)
        //{
        //    auto boneIndex = reader.ReadBits(6);
        //    auto localOrientation = reader.ReadBits(25);
        //}

        //auto rootBoneTranslationDelta = reader.ReadBits(27);

        //printf("AgentControllerMessages::CharacterIKPoseDelta:\n  agentControllerId = %u\n  frame = %llu\n  boneRotationsLength = Array[%u]\n  rootBoneTranslationDelta = Array[27]\n",
        //    agentControllerId,
        //    frame,
        //    boneRotationsLength
        //);
    }

    void OnCharacterIKPose(PacketReader &reader)   // TAG: 170FE90
    {
        // TODO: Nope, not handling this one

        //auto agentControllerId = reader.ReadUint32();
        //auto frame = reader.ReadUint64();
        //auto boneRotationsLength = reader.ReadUint32();

        /*
        for (size_t i = 0; i < boneRotationsLength; i++)
        {
            auto boneIndex = reader.ReadBits(6);
            auto localOrientation = reader.ReadBits(40);
        }

        auto rootBoneTranslation = reader.ReadBits(42);
        */

        //printf("AgentControllerMessages::CharacterIKPose:\n  agentControllerId = %u\n  frame = %llu\n",
        //    agentControllerId,
        //    frame
        //);
    }

    void OnCharacterControlPointInputReliable(PacketReader &reader)  // TAG: 170F950
    {
        //OnCharacterControlPointInput(reader);
    }

    void OnCharacterControllerInputReliable(PacketReader &reader) // TAG: 170FA30
    {
       // OnCharacterControllerInput(reader);
    }

    void OnCharacterControlPointInput(PacketReader &reader)  // TAG: 170F8E0
    {
       // auto frame = reader.ReadUint64();
       // auto agentControllerId = reader.ReadUint32();
//
       // auto controlPointsLength = reader.ReadUint32();

        //auto leftIndexTrigger = reader.ReadBits(8);
        //auto rightIndexTrigger = reader.ReadBits(8);
        //auto leftGripTrigger = reader.ReadBits(8);
        //auto rightGripTrigger = reader.ReadBits(8);
        //auto leftTouches = reader.ReadBits(7);
        //auto rightTouches = reader.ReadBits(7);
        //auto indexTriggerControlsHand = reader.ReadBits(1);
        //auto leftHandIsHolding = reader.ReadBits(1);
        //auto rightHandIsHolding = reader.ReadBits(1);

        //printf("AgentControllerMessages::CharacterControlPointInput:\n  frame = %llu\n  agentControllerId = %u\n",
        //    frame,
        //    agentControllerId
        //);
    }

    void OnWarpCharacter(PacketReader &reader) // TAG: 170F800
    {
        //if (myControllerId != 0xffffffff)
        //{
        //    auto buffer = reader.GetBuffer();

        //    auto pAgentControllerId = (uint32_t *)&buffer[12];
        //    *pAgentControllerId = myControllerId;
        //}

        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto position_x = reader.ReadFloat();
        //auto position_y = reader.ReadFloat();
        //auto position_z = reader.ReadFloat();
        //auto rotation_x = reader.ReadFloat();
        //auto rotation_y = reader.ReadFloat();
        //auto rotation_z = reader.ReadFloat();
        //auto rotation_w = reader.ReadFloat();

        //printf("[%s] AgentControllerMessages::WarpCharacter:\n  frame = %llu\n  agentControllerId = %u\n  position = <%f, %f, %f>\n  rotation = <%f, %f, %f, %f>\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId,
        //    position_x,
        //    position_y,
        //    position_z,
        //    rotation_x,
        //    rotation_y,
        //    rotation_z,
        //    rotation_w
        //);
    }

    void OnControlPoint(PacketReader &reader)  // TAG: 170F790
    {
        //auto position = reader.ReadBits(0x30);
        //auto orientation = reader.ReadBits(0x28);
        //auto enabled = reader.ReadBits(1);
        //auto controlPointType = reader.ReadBits(4);
    }

    void OnRequestWarpCharacter(PacketReader &reader)  // TAG: 170F870
    {
        // double check...
        //OnWarpCharacter(reader);
    }

    void OnAgentPlayAnimation(PacketReader &reader)   // TAG: 170FAA0
    {
        auto agentControllerId = reader.ReadUint32();

        if (_isSender)
        {
            auto frame = reader.ReadUint64();
            auto componentId = reader.ReadUint64();
            auto resourceId = reader.ReadUUID();

            if (resourceId == "24995e5142b2d361ae898e497bad984f")
            {
                // This is our magical warp animation
                reader.Reset();

                auto buffer = reader.GetBuffer();

                auto animationControllerId = *((uint32_t *)&buffer[4]);
                auto animationFrame = *((uint64_t *)&buffer[8]);

                auto pMessageId = (uint32_t *)&buffer[0];
                auto pFrame = (uint64_t *)&buffer[4];              // these two are swapped between the two packets :(
                auto pAgentControllerId = (uint32_t *)&buffer[12]; // these two are swapped between the two packets :(
                auto pPosition_x = (float *)&buffer[16];
                auto pPosition_y = (float *)&buffer[20];
                auto pPosition_z = (float *)&buffer[24];
                auto pRotation_x = (float *)&buffer[28];
                auto pRotation_y = (float *)&buffer[32];
                auto pRotation_z = (float *)&buffer[36];
                auto pRotation_w = (float *)&buffer[40];

                *pMessageId = AgentControllerMessages::WarpCharacter;
                *pFrame = animationFrame;
                *pAgentControllerId = animationControllerId;
                *pPosition_x = CameraPositionOffset[0];
                *pPosition_y = CameraPositionOffset[1];
                *pPosition_z = CameraPositionOffset[2];
                *pRotation_x = 0.0f;
                *pRotation_y = 0.0f;
                *pRotation_z = 0.0f;
                *pRotation_w = 0.0f;

                OnWarpCharacter(reader);

                if (AvatarPositionOffset != nullptr && CameraPositionOffset != nullptr) 
                {
                    AvatarPositionOffset[0] = CameraPositionOffset[0];
                    AvatarPositionOffset[1] = CameraPositionOffset[1];
                    AvatarPositionOffset[2] = CameraPositionOffset[2];
                }
                else
                {
                    if (AvatarPositionOffset == nullptr)
                    {
                        printf("OnAgentPlayAnimation -> AvatarPositionOffset == nullptr!\n");
                    }
                    if (CameraPositionOffset == nullptr)
                    {
                        printf("OnAgentPlayAnimation -> CameraPositionOffset == nullptr!\n");
                    }
                }


                return;
            }

            reader.Reset();
            reader.ReadUint32(); // messageId
            reader.ReadUint32(); // agentControllerId
        }

        OnPlayAnimation(reader);
    }

    void OnRequestAgentPlayAnimation(PacketReader &reader)  // TAG: 170FB10
    {
        // double check...
       // OnAgentPlayAnimation(reader);
    }

    void OnRequestBehaviorStateUpdate(PacketReader &reader) // TAG: 170FB80
    {
        // double check...
        //AnimationComponent::BehaviorStateUpdate(reader);
    }

    void OnAttachToCharacterNode(PacketReader &reader)   // TAG: 170FBF0
    {
        //auto frame = reader.ReadUint64();
        //auto componentId = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto attachmentOffsetPosition = reader.ReadVectorF(3);
        //auto attachmentOffsetOrientation = reader.ReadVectorF(4);
        //auto nodeType = reader.ReadUint8();
        //auto ownershipWatermark = reader.ReadUint8();
        //auto broadcastToSelf = reader.ReadUint8();

        //auto buffer = reader.GetBuffer();
        //auto componentIdPtr = (uint64_t *)&buffer[4 + 8];
        //auto agentControllerIdPtr = (uint32_t *)&buffer[4 + 8 + 8];

        //if (_isVerbose)
        //{
        //    printf("[%s] AgentControllerMessages::AttachToCharacterNode\n  frame = %llu\n  componentId = %llu\n  agentControllerId = %u\n",
        //        _isSender ? "OUT" : "IN",
        //        frame,
        //        componentId,
        //        agentControllerId
        //    );
        //}
    }

    void OnDetachFromCharacterNode(PacketReader &reader)  // TAG: 170FC60
    {
        //auto frame = reader.ReadUint64();
        //auto componentId = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto bodyPosition = reader.ReadVectorF(3);
        //auto botyOrientation = reader.ReadVectorF(4);
        //auto bodyVelocity = reader.ReadVectorF(3);
        //auto bodyAngularVelocity = reader.ReadVectorF(3);
        //auto nodeType = reader.ReadUint8();

        //if (_isVerbose)
        //{
        //    printf("[%s] AgentControllerMessages::DetachFromCharacterNode\n  frame = %llu\n  componentId = %u\n  agentControllerId = %u\n",
        //        _isSender ? "OUT" : "IN",
        //        frame,
        //        componentId,
        //        agentControllerId
        //    );
        //}
    }

    void OnRequestDetachFromCharacterNode(PacketReader &reader)  // TAG: 170FCD0
    {
        //OnDetachFromCharacterNode(reader);
    }

    void OnSetCharacterNodePhysics(PacketReader &reader)  // TAG: 170FD40
    {
       /* auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto nodeType = reader.ReadUint8();
        auto collisionsEnabled = reader.ReadUint8();
       */ auto broadcastToSelf = reader.ReadUint8();
    }

    void OnWarpCharacterNode(PacketReader &reader) // TAG: 170FDB0
    {
     //   auto agentControllerId = reader.ReadUint32();
   //     auto nodeType = reader.ReadUint32(); /// why is this node type 4bytes?
    }

    void OnCharacterIKBone(PacketReader &reader) // TAG: 170FE20
    {
//        auto boneIndex = reader.ReadBits(6);
 //       auto localOrientation = reader.ReadBits(0x28);
    }

    void OnCharacterIKBoneDelta(PacketReader &reader)  // TAG: 1710040
    {
        //auto boneIndex = reader.ReadBits(6);
        //auto localOrientation = reader.ReadBits(0x19);
    }

    void OnObjectInteraction(PacketReader &reader)   // TAG: 1710260
    {
       // auto frame = reader.ReadUint64();
       // auto agentControllerId = reader.ReadUint32();
       // auto objectId = reader.ReadUint32();
       // auto targetedPosition = reader.ReadVectorF(3);
       // auto targetedNormal = reader.ReadVectorF(3);
       // auto origin = reader.ReadVectorF(3);
       //// auto controlPointType = reader.ReadBits(4);
       // auto controlPointType = reader.ReadUint8();

       // if (_isVerbose)
       // {
       //     printf("[%s] ObjectInteraction\n  agentControllerId = %u\n  objectId = %u\n  controlPointType = %u\n",
       //         _isSender ? "OUT" : "IN",
       //         agentControllerId,
       //         objectId,
       //         controlPointType
       //     );
       // }
    }

    void OnObjectInteractionPromptUpdate(PacketReader &reader)   // TAG: 1710340
    {
        //auto frame = reader.ReadUint64();
        //auto clusterId = reader.ReadUint32();
        //auto objectId = reader.ReadUint32();
        //auto prompt = reader.ReadString();

        //printf("[%s] AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    clusterId,
        //    objectId,
        //    prompt.c_str()
        //);
    }

    void OnObjectInteractionCreate(PacketReader &reader)  // TAG: 17103B0
    {
        //auto frame = reader.ReadUint64();
        //auto clusterId = reader.ReadUint32();
        //auto objectId = reader.ReadUint32();
        //auto prompt = reader.ReadString();
        //auto enabled = reader.ReadUint8();

        //if (_isVerbose)
        //{
        //    printf("[%s] AgentControllerMessages::ObjectInteractionCreate\n  frame = %llu\n  clusterId = %u\n  objectId = %u\n  prompt = %s\n  enabled = %u\n",
        //        _isSender ? "OUT" : "IN",
        //        frame,
        //        clusterId,
        //        objectId,
        //        prompt.c_str(),
        //        enabled
        //    );
        //}
    }

    void OnRequestSitOnObject(PacketReader &reader)  // TAG: 1710420
    {
       // OnSitOnObject(reader);
    }

    void OnSitOnObject(PacketReader &reader)  // TAG: 1710490
    {
        // CHANGED: 2020-12-10: 
        // TODO: Update

        //if (myControllerId != 0xffffffff)
        //{
        //    auto buffer = reader.GetBuffer();

        //    auto pAgentControllerId = (uint32_t *)&buffer[12];
        //    *pAgentControllerId = myControllerId;
        //}

        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto componentId = reader.ReadUint64();
        //
        ////auto ownershipWatermark = reader.ReadUint8(); // REMOVED 2020-12-10 - dynamic sitpoints update
        ////auto skipAnimation = reader.ReadUint8(); // REMOVED 2020-12-10 - dynamic sitpoints update

        //printf("[%s] AgentControllerMessages::SitOnObject\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId,
        //    componentId
        //);
    }

    void OnExitSit(PacketReader &reader)  // TAG: 1710500
    {
        //if (myControllerId != 0xffffffff)
        //{
        //    auto buffer = reader.GetBuffer();

        //    auto pAgentControllerId = (uint32_t *)&buffer[12];
        //    *pAgentControllerId = myControllerId;
        //}

        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto componentId = reader.ReadUint64();
        //// auto skipAnimation = reader.ReadBits(1); // REMOVED 2020-12-10 - dynamic sitpoints update
        //auto skipExitTeleport = reader.ReadBits(1);

        //printf("[%s] AgentControllerMessages::ExitSit\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId,
        //    componentId
        //);
    }

    void OnSetAgentFiltersBody(PacketReader &reader) // TAG: 1710570
    {
        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto componentId = reader.ReadUint64();
        //auto filterBody = reader.ReadUint8();

        //printf("[%s] AgentControllerMessages::SetAgentFiltersBody\n  frame = %llu\n  agentControllerId = %u\n  componentId = %llu\n  filterBody = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId,
        //    componentId,
        //    filterBody
        //);
    }

    void OnRequestSetAgentFiltersBody(PacketReader &reader)  // TAG: 17105E0
    {
       // OnSetAgentFiltersBody(reader);
    }

    void OnSetCharacterUserProperty(PacketReader &reader)  // TAG: 1710650
    {
        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();
        //auto value = reader.ReadFloat();
        //auto propertyType = reader.ReadUint8();

        //printf("[%s] AgentControllerMessages::SetCharacterUserProperty\n  frame = %llu\n  agentControllerId = %u\n  value = %f\n  propertyType = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId,
        //    value,
        //    propertyType
        //);
    }

    void OnRequestSpawnItem(PacketReader &reader) // TAG: 1710730
    {
        if (_isSender)
        {
            //  MsgId         Frame                     AgentId       ResourceId                            AttachmentNode   SpawnPosition(78bit)|height?(32bit)|SpawnOrientation(43bit)         Checksum
            // [31 0D 85 21] [C6 6D 00 00 00 00 00 00] [05 00 00 00] [37 7F 61 06 51 D3 9B EF 5F B0 FB 67 89 AD EE 79] [FF] [EB AE 00 F2 3A 02 48 2A A0 C0 FE BF FF F7 FE 51] [77 96]

            auto buffer = reader.GetBuffer();

           // Utils::DumpPacket((const char *)buffer, 0x2D, true);

            BitReader br((uint8_t *)&buffer[33], 10);
            auto positionX = br.ReadFloat(26, false);
            auto positionY = br.ReadFloat(26, false);
            auto positionZ = br.ReadFloat(26, true);

            //printf("[%s] AgentControllerMessages::OnRequestSpawnItem: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X [X=%f Y=%f Z=%f]\n",
            //    _isSender ? "OUT" : "IN",
            //    buffer[33 + 0],
            //    buffer[33 + 1],
            //    buffer[33 + 2],
            //    buffer[33 + 3],
            //    buffer[33 + 4],
            //    buffer[33 + 5],
            //    buffer[33 + 6],
            //    buffer[33 + 7],
            //    buffer[33 + 8],
            //    buffer[33 + 9],
            //    buffer[33 + 10],
            //    positionX,
            //    positionY,
            //    positionZ
            //);


            auto camrez_enabled = false;
            auto emote_rez_enabled = false;
            char emote_to_rez[13] = {};

            FILE *inFileCamRez = nullptr;
            fopen_s(&inFileCamRez, "r:\\sanhook_config_camrez.txt", "rb");
            if (inFileCamRez != nullptr)
            {
                camrez_enabled = fgetc(inFileCamRez);
                emote_rez_enabled = fgetc(inFileCamRez);
                fread(emote_to_rez, 1, sizeof(emote_to_rez), inFileCamRez);

                fclose(inFileCamRez);
            }

            if (isUsingTargetAsSpawnPosition) {
                printf("Spawning item at target position %f,%f,%f\n",currentTargetPosition[0],currentTargetPosition[1],currentTargetPosition[2]);
                positionX = currentTargetPosition[0];
                positionY = currentTargetPosition[1];
                positionZ = currentTargetPosition[2];

                br.Reset();
                br.WriteFloat(positionX, 26, false);
                br.WriteFloat(positionY, 26, false);
                br.WriteFloat(positionZ, 26, true);

                br.Reset();
                positionX = br.ReadFloat(26, false);
                positionY = br.ReadFloat(26, false);
                positionZ = br.ReadFloat(26, true);

                // do not attach this item...
                auto attachmentNodePtr = &buffer[32];
                *attachmentNodePtr = 255;
            }
            else if (camrez_enabled)
            {
                positionX = CameraPositionOffset[0];
                positionY = CameraPositionOffset[1];
                positionZ = CameraPositionOffset[2];

                br.Reset();
                br.WriteFloat(positionX, 26, false);
                br.WriteFloat(positionY, 26, false);
                br.WriteFloat(positionZ, 26, true);

                br.Reset();
                positionX = br.ReadFloat(26, false);
                positionY = br.ReadFloat(26, false);
                positionZ = br.ReadFloat(26, true);

                // do not attach this item...
                auto attachmentNodePtr = &buffer[32];
                *attachmentNodePtr = 255;
            }


            if (emote_rez_enabled)
            {
                // This is our magical warp animation
                reader.Reset();

                auto buffer = reader.GetBuffer();

                auto pMessageId = (uint32_t *)&buffer[0];
                auto pFrame = (uint64_t *)&buffer[4];
                auto pAgentControllerId = (uint32_t *)&buffer[12];
                auto pTypeLength = (uint32_t *)&buffer[16];
                auto pType = (char *)&buffer[20];

                // Sansar.Heart
                // Sansar.Stars
                // Sansar.Fire
                // Sansar.Lightning
                // Sansar.Exclamation
                // Sansar.WTF
                // Sansar.Panda
                // Sansar.Toast
                // Sansar.Boba
                std::string reactionName = std::string(emote_to_rez);// "Sansar.Heart";

                *pMessageId = AgentControllerMessages::UserReaction;
                *pTypeLength = reactionName.length();  // we only have 13 bytes to play with :(
                memcpy(pType, reactionName.c_str(), reactionName.length());
                memcpy(&buffer[20 + reactionName.length()], &buffer[33], reader.GetBufferSize() - 33);

                reader.ReadUint32();
                OnUserReaction(reader);

                return;
            }

            uint8_t newResourceId[16] = {};
            bool enabled = false;
            FILE *inFile = nullptr;
            fopen_s(&inFile, "r:\\sanhook_config_rez.txt", "rb");
            if (inFile != nullptr)
            {
                auto resourceIdPtr = &buffer[16];

                enabled = fgetc(inFile);

                auto bytesRead = fread_s(newResourceId, sizeof(newResourceId), sizeof(newResourceId[0]), 16, inFile);
                fclose(inFile);

                if (bytesRead == 16 && enabled)
                {
                    memcpy(resourceIdPtr, newResourceId, 16);
                }
            }
        }

        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto resourceId = reader.ReadUUID();
        auto attachmentNode = reader.ReadUint8();
        auto spawnPosition = reader.ReadBits(0x4E);
        auto spawnOrientation = reader.ReadBits(0x2B);

        printf("[%s] AgentControllerMessages::OnRequestSpawnItem\n  frame = %llu\n  agentControllerId = %u\n  resourceId = %s\n  attachmentNode = %u\n",
            _isSender ? "OUT" : "IN",
            frame,
            agentControllerId,
            resourceId.c_str(),
            attachmentNode
        );
    }

    void OnRequestDeleteLatestSpawn(PacketReader &reader)  // TAG: 17107A0
    {
        //auto frame = reader.ReadUint64();
        //auto agentControllerId = reader.ReadUint32();

        //printf("[%s] AgentControllerMessages::RequestDeleteLatestSpawn\n  frame = %llu\n  agentControllerId = %u\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    agentControllerId
        //);
    }

    void OnRequestDeleteAllSpawns(PacketReader &reader)  // TAG: 1710810
    {
    /*    auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();

        printf("[%s] AgentControllerMessages::RequestDeleteAllSpawns\n  frame = %llu\n  agentControllerId = %u\n",
            _isSender ? "OUT" : "IN",
            frame,
            agentControllerId
        );*/
    }

    void OnUserReaction(PacketReader &reader)  // TAG: 16D52F0 
    {
        // Added 2020-09-09
        auto frame = reader.ReadUint64();
        auto agentControllerId = reader.ReadUint32();
        auto type = reader.ReadString(); // this is actually ReadArray, but it just contains a string :s

        BitReader br(reader.GetCurrentPointer(), 10);
        auto positionX = br.ReadFloat(26, false);
        auto positionY = br.ReadFloat(26, false);
        auto positionZ = br.ReadFloat(26, true);

        //auto position = reader.ReadBits(78);
        //auto orientation = reader.ReadBits(43);

        printf("[%s] AgentControllerMessages::OnUserReaction:\n  frame = %llu\n  agentControllerId = %u\n  type = %s\n  position = <%f, %f, %f>\n",
            _isSender ? "OUT" : "IN",
            frame,
            agentControllerId,
            type.c_str(),
            positionX, positionY, positionZ
        );
    }

    void OnPlayAnimation(PacketReader &reader) // TAG: 1581210
    {
       /* auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto resourceId = reader.ReadUUID();*/
        #define _CRT_SECURE_NO_DEPRECATE
        //if (_isSender)
        //{
        //    auto buffer = reader.GetBuffer();
        //
        //    BitReader br((uint8_t *)&buffer[8+8+8+8+8], 2);
        //    auto speed = br.ReadFloatX(16, 10.0f);
        //    printf("SPEED= %f\n", speed);
        //
        //    auto anim_speed_enabled = false;
        //    auto anim_speed_override = speed;
        //
        //    FILE *inFileCamRez = nullptr;
        //    std::ifstream inFile("r:\\sanhook_config_anim.txt");
        //    if (inFile.is_open())
        //    {
        //        inFile >> anim_speed_enabled >> anim_speed_override;
        //        printf("anim_speed_enabled=%d anim_speed_override=%f\n", anim_speed_enabled, anim_speed_override);
        //        inFile.close();
        //    }
        //
        //    if (anim_speed_enabled)
        //    {
        //        br.Reset();
        //        br.WriteFloatX(anim_speed_override, 16, 10.0f);
        //    }
        //}

        //auto playbackSpeed = reader.ReadBits(16);
        //auto skeletonType = reader.ReadBits(2);
        //auto animationType = reader.ReadBits(3);
        //auto playbackMode = reader.ReadBits(3);

        //printf("[%s] AgentControllerMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    componentId,
        //    resourceId.c_str()
        //);
    }
};

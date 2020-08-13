#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include <iostream>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnAnimationComponentPlayAnimation(PacketReader &reader);
//void OnAnimationComponentCharacterSetPosition(PacketReader &reader);
//void OnAnimationComponentBehaviorStateUpdate(PacketReader &reader);
//void OnAnimationComponentAnimationOverride(PacketReader &reader);
//void OnAnimationComponentCharacterAnimationDestroyed(PacketReader &reader);
//void OnAnimationComponentBoolVariable(PacketReader &reader);
//void OnAnimationComponentInt8Variable(PacketReader &reader);
//void OnAnimationComponentQuaternionVariable(PacketReader &reader);
//void OnAnimationComponentVectorVariable(PacketReader &reader);
//void OnAnimationComponentFloatRangeNodeVariable(PacketReader &reader);
//void OnAnimationComponentFloatNodeVariable(PacketReader &reader);
//void OnAnimationComponentFloatVariable(PacketReader &reader);
//void OnAnimationComponentCharacterTransform(PacketReader &reader);
//void OnAnimationComponentCharacterTransformPersistent(PacketReader &reader);
//void OnAnimationComponentBehaviorInternalState(PacketReader &reader);
//void OnAnimationComponentBehaviorInitializationData(PacketReader &reader);


class AnimationComponent : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case AnimationComponentMessages::FloatVariable:  // TAG: 15809C0
            {
                this->OnFloatVariable(reader);
                break;
            }
            case AnimationComponentMessages::FloatNodeVariable:  // TAG: 1580A30
            {
                this->OnFloatNodeVariable(reader);
                break;
            }
            case AnimationComponentMessages::FloatRangeNodeVariable:  // TAG: 1580AA0
            {
                this->OnFloatRangeNodeVariable(reader);
                break;
            }
            case AnimationComponentMessages::VectorVariable:  // TAG: 1580B10
            {
                this->OnVectorVariable(reader);
                break;
            }
            case AnimationComponentMessages::QuaternionVariable:  // TAG: 1580B80
            {
                this->OnQuaternionVariable(reader);
                break;
            }
            case AnimationComponentMessages::Int8Variable:  // TAG: 1580BF0
            {
                this->OnInt8Variable(reader);
                break;
            }
            case AnimationComponentMessages::BoolVariable:  // TAG: 1580C60
            {
                this->OnBoolVariable(reader);
                break;
            }
            case AnimationComponentMessages::CharacterTransform: // TAG: 1580CD0
            {
                this->OnCharacterTransform(reader);
                break;
            }
            case AnimationComponentMessages::CharacterTransformPersistent: // TAG: 1580D40
            {
                this->OnCharacterTransformPersistent(reader);
                break;
            }
            case AnimationComponentMessages::CharacterAnimationDestroyed:  // TAG: 1580DB0
            {
                this->OnCharacterAnimationDestroyed(reader);
                break;
            }
            case AnimationComponentMessages::AnimationOverride:  // TAG: 1580F70
            {
                this->OnAnimationOverride(reader);
                break;
            }
            case AnimationComponentMessages::BehaviorInternalState: // TAG: 1580FE0
            {
                this->OnBehaviorInternalState(reader);
                break;
            }
            case AnimationComponentMessages::CharacterBehaviorInternalState:  // TAG: 1581050
            {
                this->OnCharacterBehaviorInternalState(reader);
                break;
            }
            case AnimationComponentMessages::BehaviorStateUpdate: // TAG: 15810C0
            {
                this->OnBehaviorStateUpdate(reader);
                break;
            }
            case AnimationComponentMessages::BehaviorInitializationData: // TAG: 1581130
            {
                this->OnBehaviorInitializationData(reader);
                break;
            }
            case AnimationComponentMessages::CharacterSetPosition:  // TAG: 15811A0
            {
                this->OnCharacterSetPosition(reader);
                break;
            }
            case AnimationComponentMessages::PlayAnimation:  // TAG: 1581210
            {
                this->OnPlayAnimation(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnFloatVariable(PacketReader &reader)  // TAG: 15809C0
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    void OnFloatNodeVariable(PacketReader &reader)  // TAG: 1580A30
    {
        auto nodeId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    void OnFloatRangeNodeVariable(PacketReader &reader)  // TAG: 1580AA0
    {
        auto nodeId = reader.ReadUint16();
        auto startValue = reader.ReadFloat();
        auto endValue = reader.ReadFloat();
    }

    void OnVectorVariable(PacketReader &reader)  // TAG: 1580B10
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadVectorF(3);
    }

    void OnQuaternionVariable(PacketReader &reader) // TAG: 1580B80
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadVectorF(4);
    }

    void OnInt8Variable(PacketReader &reader) // TAG: 1580BF0
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    void OnBoolVariable(PacketReader &reader) // TAG: 1580C60
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    void OnCharacterTransform(PacketReader &reader) // TAG: 1580CD0
    {
        auto componentId = reader.ReadUint64();
        auto serverFrame = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);
        auto orientationQuat = reader.ReadBits(0x28);


        //printf("[%s] OnCharacterTransform:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
        //_isSender ? "OUT" : "IN",
        //    componentId,
        //    serverFrame,
        //    groundComponentId
        //);
    }

    void OnCharacterTransformPersistent(PacketReader &reader) // TAG: 1580D40
    {
        OnCharacterTransform(reader);
    }

    void OnCharacterAnimationDestroyed(PacketReader &reader) // TAG: 1580DB0
    {
        auto componentId = reader.ReadUint64();
    }

    void OnAnimationOverride(PacketReader &reader)  // TAG: 1580F70
    {
        auto slotIndex = reader.ReadUint8();
    }

    void OnBehaviorInternalState(PacketReader &reader) // TAG: 1580FE0
    {
        /*
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        auto overrides = ; // this is something else, containing a call to OnPlayAnimation(reader);? or was that below. either way not handling this mess

        auto numSlotStates = reader.ReadUint32();
        for (size_t i = 0; i < numSlotStates; i++)
        {
            auto slotIndex = reader.ReadUint8();
        }

        auto stateData = reader.ReadArray();
        auto isPlaying = reader.ReadUint8();

        printf("[%s] AnimationComponentMessages::BehaviorInternalState:\n  componentId = %llu\n  frame = %llu\n",
            _isSender ? "OUT" : "IN",
            componentId,
            frame
        );
        */
    }

    void OnCharacterBehaviorInternalState(PacketReader &reader)  // TAG: 1581050
    {
        OnBehaviorInternalState(reader);
    }

    void OnBehaviorStateUpdate(PacketReader &reader) // TAG: 15810C0
    {
        //if (!_isSender)
        //{
        //    auto packet = reader.GetBuffer();
        //    auto pComponentId = (uint64_t *)&packet[12];
        //    auto pExceptAgentControllerId = (uint32_t *)&packet[20];

        //    *pComponentId = myComponentId;
        //    *pExceptAgentControllerId = myControllerId;
        //}

        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto exceptAgentControllerId = reader.ReadUint32();

        auto floatsLength = reader.ReadUint32();
        for (size_t i = 0; i < floatsLength; i++)
        {
            OnFloatNodeVariable(reader);
        }

        auto vectorsLength = reader.ReadUint32();
        for (size_t i = 0; i < vectorsLength; i++)
        {
            OnVectorVariable(reader);
        }

        auto quaternionsLength = reader.ReadUint32();
        for (size_t i = 0; i < quaternionsLength; i++)
        {
            OnQuaternionVariable(reader);
        }

        auto int8sLength = reader.ReadUint32();
        for (size_t i = 0; i < int8sLength; i++)
        {
            OnInt8Variable(reader);
        }

        auto boolsLength = reader.ReadUint32();
        for (size_t i = 0; i < boolsLength; i++)
        {
            OnBoolVariable(reader);
        }

       // auto internalEventIdsLength = reader.ReadList<uint16_t>(&PacketReader::ReadUint16);
       
        //auto internalEventIdsLength = reader.ReadList<uint16_t>([](PacketReader *reader) {
        //    return reader->ReadUint16(); 
        //});

        auto internalEventIdsLength = reader.ReadUint32();
        for (size_t i = 0; i < internalEventIdsLength; i++)
        {
            auto value = reader.ReadUint16();
        }

        auto animationAction = reader.ReadUint8();

        auto nodeLocalTimesLength = reader.ReadUint32();
        for (size_t i = 0; i < nodeLocalTimesLength; i++)
        {
            OnFloatNodeVariable(reader);
        }

        auto nodeCropValuesLength = reader.ReadUint32();
        for (size_t i = 0; i < nodeCropValuesLength; i++)
        {
            OnFloatRangeNodeVariable(reader);
        }

        //printf("[%s] AnimationComponentMessages::BehaviorStateUpdate:\n  frame = %llu\n  componentId = %llu\n  exceptAgentControllerId = %u\n",
        //_isSender ? "OUT" : "IN",
        //    frame,
        //    componentId,
        //    exceptAgentControllerId
        //);
    }

    void OnBehaviorInitializationData(PacketReader &reader)// TAG: 1581130
    {
        auto behaviorStateUpdatesLength = reader.ReadUint32();
        auto animationUpdatesLength = reader.ReadUint32();

        //printf("[%s] AnimationComponentMessages::BehaviorInitializationData:\n  behaviorStateUpdatesLength = %u\n  animationUpdatesLength = %u\n",
        //_isSender ? "OUT" : "IN",
        //    behaviorStateUpdatesLength,
        //    animationUpdatesLength
        //);
    }

    void OnCharacterSetPosition(PacketReader &reader)  // TAG: 15811A0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);

        //printf("[%s] OnCharacterSetPosition:\n  componentId = %d\n",
        //    _isSender ? "OUT" : "IN",
        //    componentId
        //);
    }

    std::set<std::string> savedAnimations;

    void OnPlayAnimation(PacketReader &reader)  // TAG: 1581210
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto resourceId = reader.ReadUUID();

        auto playbackSpeed = reader.ReadBits(0x10);
        //v10 = _mm_cvtpd_ps((__m128d)COERCE_UNSIGNED_INT64((double)((signed int)v16 - 0x7FFF) * 0.00003051850947599719));
        //v10.m128_f32[0] = v10.m128_f32[0] * 10.0;
        //*(__m128 *)(v5 + 48) = _mm_shuffle_ps(v10, v10, 0);

        auto skeletonType = reader.ReadBits(2);
        auto animationType = reader.ReadBits(3);
        auto playbackMode = reader.ReadBits(3);

        //printf("[%s] AnimationComponentMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
        //    _isSender ? "OUT" : "IN",
        //    frame,
        //    componentId,
        //    resourceId.c_str()
        //);

        if (!this->_isSender)
        {
            if (savedAnimations.find(resourceId) == savedAnimations.end())
            {
                auto resourceUuid = Utils::ToUUID(resourceId);

                std::filesystem::path userdumpPath = "R:\\dec\\new_sansar_dec\\animationdump.csv";

                auto now = std::chrono::system_clock::now();
                auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

                FILE *outFile = nullptr;
                fopen_s(&outFile, userdumpPath.string().c_str(), "a");
                fprintf(outFile, "\"%lld\",\"%s\"\n",
                    timestamp,
                    resourceUuid.c_str()
                );
                fclose(outFile);

                savedAnimations.insert(resourceId);
            }
        }
    }


};

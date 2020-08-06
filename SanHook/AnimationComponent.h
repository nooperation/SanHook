#pragma once

#include <cstdio>
#include <string>
#include <vector>

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


class AnimationComponent
{
public:
    static void OnFloatVariable(PacketReader &reader)  // 15809C0
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    static void OnFloatNodeVariable(PacketReader &reader)  // 1580A30
    {
        auto nodeId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    static void OnFloatRangeNodeVariable(PacketReader &reader)  // 1580AA0
    {
        auto nodeId = reader.ReadUint16();
        auto startValue = reader.ReadFloat();
        auto endValue = reader.ReadFloat();
    }

    static void OnVectorVariable(PacketReader &reader)  // 1580B10
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadVectorF(3);
    }

    static void OnQuaternionVariable(PacketReader &reader) // 1580B80
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadVectorF(4);
    }

    static void OnInt8Variable(PacketReader &reader) // 1580BF0
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    static void OnBoolVariable(PacketReader &reader) // 1580C60
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    static void OnCharacterTransform(PacketReader &reader) // 1580CD0
    {
        auto componentId = reader.ReadUint64();
        auto serverFrame = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);
        auto orientationQuat = reader.ReadBits(0x28);

        printf("OnCharacterTransform:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
            componentId,
            serverFrame,
            groundComponentId
        );
    }

    static void OnCharacterTransformPersistent(PacketReader &reader) // 1580D40
    {
        OnCharacterTransform(reader);
    }

    static void OnCharacterAnimationDestroyed(PacketReader &reader) // 1580DB0
    {
        auto componentId = reader.ReadUint64();
    }

    static void OnAnimationOverride(PacketReader &reader)  // 1580F70
    {
        auto slotIndex = reader.ReadUint8();
    }

    static void OnBehaviorInternalState(PacketReader &reader) // 1580FE0
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto overrides = reader.ReadArray();

        auto numSlotStates = reader.ReadUint32();
        for (size_t i = 0; i < numSlotStates; i++)
        {
            auto slotState = reader.ReadUint8();
        }

        auto stateData = reader.ReadArray();
        auto isPlaying = reader.ReadUint8();

        printf("AnimationComponentMessages::BehaviorInternalState:\n  componentId = %llu\n  frame = %llu\n",
            componentId,
            frame
        );
    }

    static void OnCharacterBehaviorInternalState(PacketReader &reader)  // 1581050
    {
        OnBehaviorInternalState(reader);
    }

    static void OnBehaviorStateUpdate(PacketReader &reader) // 15810C0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto exceptAgentControllerId = reader.ReadUint32();

        printf("AnimationComponentMessages::BehaviorStateUpdate:\n  frame = %llu\n  componentId = %llu\n  exceptAgentControllerId = %u\n",
            frame,
            componentId,
            exceptAgentControllerId
        );

        auto floatsLength = reader.ReadUint32();
        for (size_t i = 0; i < floatsLength; i++)
        {
            auto nodeId = reader.ReadUint16();
            auto value = reader.ReadFloat();
        }

        auto vectorsLength = reader.ReadUint32();
        for (size_t i = 0; i < vectorsLength; i++)
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadVectorF(3);
        }

        auto quaternionsLength = reader.ReadUint32();
        for (size_t i = 0; i < quaternionsLength; i++)
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadVectorF(4);
        }

        auto int8sLength = reader.ReadUint32();
        for (size_t i = 0; i < int8sLength; i++)
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadUint8();
        }

        auto boolsLength = reader.ReadUint32();
        for (size_t i = 0; i < boolsLength; i++)
        {
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadUint8();
        }

        auto internalEventIds = reader.ReadUint32();
        for (size_t i = 0; i < internalEventIds; i++)
        {
            // double check this one...
            auto internalId = reader.ReadUint16();
            auto value = reader.ReadUint8();
        }

        auto animationAction = reader.ReadUint8();

        auto nodeLocalTimesLength = reader.ReadUint32();
        for (size_t i = 0; i < nodeLocalTimesLength; i++)
        {
            auto nodeId = reader.ReadUint16();
            auto value = reader.ReadFloat();
        }

        auto nodeCropValuesLength = reader.ReadUint32();
        for (size_t i = 0; i < nodeCropValuesLength; i++)
        {
            auto nodeId = reader.ReadUint16();
            auto startValue = reader.ReadFloat();
            auto endValue = reader.ReadFloat();
        }
    }

    static void OnBehaviorInitializationData(PacketReader &reader)// 1581130
    {
        auto behaviorStateUpdatesLength = reader.ReadUint32();
        auto animationUpdatesLength = reader.ReadUint32();

        printf("AnimationComponentMessages::BehaviorInitializationData:\n  behaviorStateUpdatesLength = %u\n  animationUpdatesLength = %u\n",
            behaviorStateUpdatesLength,
            animationUpdatesLength
        );
    }

    static void OnCharacterSetPosition(PacketReader &reader)  // 15811A0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);
    }

    static void OnPlayAnimation(PacketReader &reader)  // 1581210
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

        printf("AnimationComponentMessages::PlayAnimation:\n  frame = %llu\n  componentId = %llu\n  resourceId = %s\n",
            frame,
            componentId,
            resourceId.c_str()
        );
    }
};

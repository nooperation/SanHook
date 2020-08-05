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
    static void OnPlayAnimation(PacketReader &reader)
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

    static void OnCharacterTransform(PacketReader &reader)
    {
        auto componentId = reader.ReadUint64();
        auto serverFrame = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        // auto position = ; // Weird packed float stuff again
        // auto orientationQuat = ; // Weird packed float stuff again

        printf("OnCharacterTransform:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
            componentId,
            serverFrame,
            groundComponentId
        );
    }

    static void OnCharacterTransformPersistent(PacketReader &reader)
    {
        auto componentId = reader.ReadUint64();
        auto serverFrame = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        // auto position = ; // Weird packed float stuff again
        // auto orientationQuat = ; // Weird packed float stuff again

        printf("OnCharacterTransformPersistent:\n  componentId = %llu\n  serverFrame = %llu\n  groundComponentId = %llu\n",
            componentId,
            serverFrame,
            groundComponentId
        );
    }

    static void OnBehaviorInternalState(PacketReader &reader)
    {
        // TODO: Nope, not handling this one

        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        auto overridesLength = reader.ReadUint32();
        /* auto overridesData = reader.ReadBytes(overridesLength);

         auto numSlotStates = reader.ReadUint32();
         for (size_t i = 0; i < numSlotStates; i++)
         {
             auto slotState = reader.ReadUint8();
         }

         auto stateDataLength = reader.ReadUint32();
         auto stateData = reader.ReadBytes(stateDataLength);

         auto isPlaying = reader.ReadUint8();*/

        printf("AnimationComponentMessages::BehaviorInternalState:\n  componentId = %llu\n  frame = %llu\n  overridesLength = %u\n",
            componentId,
            frame,
            overridesLength
        );
    }

    static void OnBehaviorInitializationData(PacketReader &reader)
    {
        auto behaviorStateUpdatesLength = reader.ReadUint32();
        auto animationUpdatesLength = reader.ReadUint32();

        printf("AnimationComponentMessages::BehaviorInitializationData:\n  behaviorStateUpdatesLength = %u\n  animationUpdatesLength = %u\n",
            behaviorStateUpdatesLength,
            animationUpdatesLength
        );
    }

    static void OnCharacterSetPosition(PacketReader &reader)
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);
    }

    static void OnBehaviorStateUpdate(PacketReader &reader)
    {

        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto groundComponentId = reader.ReadUint64();
        auto position = reader.ReadBits(0x48);
    }

    static void OnAnimationOverride(PacketReader &reader)
    {

        auto slotIndex = reader.ReadUint8();
    }

    static void OnCharacterAnimationDestroyed(PacketReader &reader)
    {

        auto componentId = reader.ReadUint64();
    }

    static void OnBoolVariable(PacketReader &reader)
    {

        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    static void OnInt8Variable(PacketReader &reader)
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadUint8();
    }

    static void OnQuaternionVariable(PacketReader &reader)
    {

        auto internalId = reader.ReadUint16();
        auto value_x = reader.ReadFloat();
        auto value_y = reader.ReadFloat();
        auto value_z = reader.ReadFloat();
        auto value_w = reader.ReadFloat();
    }

    static void OnVectorVariable(PacketReader &reader)
    {

        auto internalId = reader.ReadUint16();
        auto value_x = reader.ReadFloat();
        auto value_y = reader.ReadFloat();
        auto value_z = reader.ReadFloat();
    }

    static void OnFloatRangeNodeVariable(PacketReader &reader)
    {
        auto nodeId = reader.ReadUint16();
        auto startValue = reader.ReadFloat();
        auto endValue = reader.ReadFloat();
    }

    static void OnFloatNodeVariable(PacketReader &reader)
    {
        auto nodeId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }

    static void OnFloatVariable(PacketReader &reader)
    {
        auto internalId = reader.ReadUint16();
        auto value = reader.ReadFloat();
    }
};

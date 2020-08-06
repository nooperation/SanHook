#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnSimulationMessagesInitialTimestamp(PacketReader &reader);
//void OnSimulationMessagesTimestamp(PacketReader &reader);
//void OnSimulationMessagesSetWorldGravityMagnitude(PacketReader &reader);
//void OnSimulationMessagesActiveRigidBodyUpdate(PacketReader &reader);
//void OnSimulationMessagesRigidBodyDeactivated(PacketReader &reader);
//void OnSimulationMessagesRigidBodyPropertyChanged(PacketReader &reader);
//void OnSimulationMessagesRigidBodyDestroyed(PacketReader &reader);

class Simulation
{
public:
    static void OnInitialTimestamp(PacketReader &reader)
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        printf("OnInitialTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    static void OnTimestamp(PacketReader &reader)
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        printf("OnTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    static void OnSetWorldGravityMagnitude(PacketReader &reader)
    {

        auto frame = reader.ReadUint64();
        auto magnitude = reader.ReadFloat();
    }

    static void OnActiveRigidBodyUpdate(PacketReader &reader)
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto ownerId = reader.ReadUint32();
        auto ownershipWatermark = reader.ReadUint8();
        auto authority = reader.ReadUint8();
        // auto position // 78bit
        // auto orientationQuat // 43bit
        // auto linearVeolcity // 39bit
        // auto angularVelocity  // 36bit

        printf("SimulationMessages::ActiveRigidBodyUpdate:\n  componentId = %llu\n  frame = %llu\n  ownerId = %u\n  ownershipWatermark = %u\n  authority = %u\n",
            componentId,
            frame,
            ownerId,
            ownershipWatermark,
            authority
        );
    }


    static void OnRigidBodyDeactivated(PacketReader &reader)
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto ownershipWatermark = reader.ReadUint8();
        //position = reader.   78bit floats
        // orientationQuat  43bit floats

        printf("SimulationManager::OnRigidBodyDeactivated:\n  componentId = %llu\n  frame = %llu\n  ownershipWatermark = %u\n",
            componentId,
            frame,
            ownershipWatermark
        );
    }

    static void OnRigidBodyPropertyChanged(PacketReader &reader)
    {

        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto propertyData = reader.ReadBytes(16);
        //auto propertyType = reader.ReadBits(5);
    }

    static void OnRigidBodyDestroyed(PacketReader &reader)
    {

        auto componentId = reader.ReadUint64();
    }
};
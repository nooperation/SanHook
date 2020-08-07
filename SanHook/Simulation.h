#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnSimulationMessagesInitialTimestamp(PacketReader &reader);
//void OnSimulationMessagesTimestamp(PacketReader &reader);
//void OnSimulationMessagesSetWorldGravityMagnitude(PacketReader &reader);
//void OnSimulationMessagesActiveRigidBodyUpdate(PacketReader &reader);
//void OnSimulationMessagesRigidBodyDeactivated(PacketReader &reader);
//void OnSimulationMessagesRigidBodyPropertyChanged(PacketReader &reader);
//void OnSimulationMessagesRigidBodyDestroyed(PacketReader &reader);

class Simulation : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader)
    {
        switch (messageId)
        {
            case SimulationMessages::InitialTimestamp:  // TAG: 15733C0 
            {
                Simulation::OnInitialTimestamp(reader);
                break;
            }
            case SimulationMessages::Timestamp: // TAG: 1573430 
            {
                Simulation::OnTimestamp(reader);
                break;
            }
            case SimulationMessages::SetWorldGravityMagnitude:  // TAG: 15734A0
            {
                Simulation::OnSetWorldGravityMagnitude(reader);
                break;
            }
            case SimulationMessages::ActiveRigidBodyUpdate:  // TAG: 1573510
            {
                Simulation::OnActiveRigidBodyUpdate(reader);
                break;
            }
            case SimulationMessages::RigidBodyDeactivated:  // TAG: 1573580
            {
                Simulation::OnRigidBodyDeactivated(reader);
                break;
            }
            case SimulationMessages::RigidBodyPropertyChanged:  // TAG: 15735F0
            {
                Simulation::OnRigidBodyPropertyChanged(reader);
                break;
            }
            case SimulationMessages::RigidBodyDestroyed:  // TAG: 1573660
            {
                Simulation::OnRigidBodyDestroyed(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    static void OnInitialTimestamp(PacketReader &reader)  // TAG: 15733C0 
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        printf("OnInitialTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    static void OnTimestamp(PacketReader &reader) // TAG: 1573430 
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        //printf("OnTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    static void OnSetWorldGravityMagnitude(PacketReader &reader) // TAG: 15734A0
    {
        auto frame = reader.ReadUint64();
        auto magnitude = reader.ReadFloat();

        printf("OnSetWorldGravityMagnitude\n  frame = %llu\n  magnitude = %f\n",
            frame,
            magnitude
        );
    }

    static void OnActiveRigidBodyUpdate(PacketReader &reader)  // TAG: 1573510
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

        //printf("SimulationMessages::ActiveRigidBodyUpdate:\n  componentId = %llu\n  frame = %llu\n  ownerId = %u\n  ownershipWatermark = %u\n  authority = %u\n",
        //    componentId,
        //    frame,
        //    ownerId,
        //    ownershipWatermark,
        //    authority
        //);
    }

    static void OnRigidBodyDeactivated(PacketReader &reader) // TAG: 1573580
    {
        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto ownershipWatermark = reader.ReadUint8();
        //position = reader.   78bit floats
        // orientationQuat  43bit floats

        //printf("SimulationManager::OnRigidBodyDeactivated:\n  componentId = %llu\n  frame = %llu\n  ownershipWatermark = %u\n",
        //    componentId,
        //    frame,
        //    ownershipWatermark
        //);
    }

    static void OnRigidBodyPropertyChanged(PacketReader &reader) // TAG: 15735F0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto propertyData = reader.ReadBytes(16);
        //auto propertyType = reader.ReadBits(5);
    }

    static void OnRigidBodyDestroyed(PacketReader &reader)  // TAG: 1573660
    {
        auto componentId = reader.ReadUint64();
    }
};
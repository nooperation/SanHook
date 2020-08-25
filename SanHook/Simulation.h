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
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case SimulationMessages::InitialTimestamp:  // TAG: 15733C0 
            {
                this->OnInitialTimestamp(reader);
                break;
            }
            case SimulationMessages::Timestamp: // TAG: 1573430 
            {
                this->OnTimestamp(reader);
                break;
            }
            case SimulationMessages::SetWorldGravityMagnitude:  // TAG: 15734A0
            {
                this->OnSetWorldGravityMagnitude(reader);
                break;
            }
            case SimulationMessages::ActiveRigidBodyUpdate:  // TAG: 1573510
            {
                this->OnActiveRigidBodyUpdate(reader);
                break;
            }
            case SimulationMessages::RigidBodyDeactivated:  // TAG: 1573580
            {
                this->OnRigidBodyDeactivated(reader);
                break;
            }
            case SimulationMessages::RigidBodyPropertyChanged:  // TAG: 15735F0
            {
                this->OnRigidBodyPropertyChanged(reader);
                break;
            }
            case SimulationMessages::RigidBodyDestroyed:  // TAG: 1573660
            {
                this->OnRigidBodyDestroyed(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnInitialTimestamp(PacketReader &reader)  // TAG: 15733C0 
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        printf("OnInitialTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    void OnTimestamp(PacketReader &reader) // TAG: 1573430 
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        //printf("OnTimestamp: nanoseconds = %llu | frame = %llu\n", nanoseconds, frame);
    }

    void OnSetWorldGravityMagnitude(PacketReader &reader) // TAG: 15734A0
    {
        auto frame = reader.ReadUint64();
        auto magnitude = reader.ReadFloat();

        printf("OnSetWorldGravityMagnitude\n  frame = %llu\n  magnitude = %f\n",
            frame,
            magnitude
        );
    }

    void OnActiveRigidBodyUpdate(PacketReader &reader)  // TAG: 1573510
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

        if (false && _isVerbose)
        {
            printf("[%s] SimulationMessages::ActiveRigidBodyUpdate:\n  componentId = %llu\n  frame = %llu\n  ownerId = %u\n  ownershipWatermark = %u\n  authority = %u\n",
                _isSender ? "OUT" : "IN",
                componentId,
                frame,
                ownerId,
                ownershipWatermark,
                authority
            );
        }
    }

    void OnRigidBodyDeactivated(PacketReader &reader) // TAG: 1573580
    {
        auto buffer = reader.GetBuffer();
        auto componentIdPtr = (uint64_t *)&buffer[4];
        *componentIdPtr = UINT64_MAX;


        auto componentId = reader.ReadUint64();
        auto frame = reader.ReadUint64();
        auto ownershipWatermark = reader.ReadUint8();
        //position = reader.   78bit floats
        // orientationQuat  43bit floats



        if (_isVerbose)
        {
            printf("[%s] SimulationManager::RigidBodyDeactivated:\n  componentId = %llu\n  frame = %llu\n  ownershipWatermark = %u\n",
                _isSender ? "OUT" : "IN",
                componentId,
                frame,
                ownershipWatermark
            );
        }
    }

    void OnRigidBodyPropertyChanged(PacketReader &reader) // TAG: 15735F0
    {
        auto frame = reader.ReadUint64();
        auto componentId = reader.ReadUint64();
        auto propertyData = reader.ReadBytes(16);
        //auto propertyType = reader.ReadBits(5);

        if (_isVerbose)
        {
            printf("RigidBodyPropertyChanged\n  componentId = %llu\n", componentId);
        }
    }

    void OnRigidBodyDestroyed(PacketReader &reader)  // TAG: 1573660
    {
        auto componentId = reader.ReadUint64();

        if (_isVerbose)
        {
            printf("OnRigidBodyDestroyed\n  componentId = %llu\n", componentId);
        }
    }
};
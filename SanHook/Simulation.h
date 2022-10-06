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
               // this->OnInitialTimestamp(reader);
                break;
            }
            case SimulationMessages::Timestamp: // TAG: 1573430 
            {
              //  this->OnTimestamp(reader);
                break;
            }
            case SimulationMessages::SetWorldGravityMagnitude:  // TAG: 15734A0
            {
              //  this->OnSetWorldGravityMagnitude(reader);
                break;
            }
            case SimulationMessages::ActiveRigidBodyUpdate:  // TAG: 1573510
            {
               // this->OnActiveRigidBodyUpdate(reader);
                break;
            }
            case SimulationMessages::RigidBodyDeactivated:  // TAG: 1573580
            {
               // this->OnRigidBodyDeactivated(reader);
                break;
            }
            case SimulationMessages::RigidBodyPropertyChanged:  // TAG: 15735F0
            {
               // this->OnRigidBodyPropertyChanged(reader);
                break;
            }
            case SimulationMessages::RigidBodyDestroyed:  // TAG: 1573660
            {
               // this->OnRigidBodyDestroyed(reader);
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

        printf("[%s] SimulationMessages::InitialTimestamp\n  nanoseconds = %llu\n  frame = %llu\n",
            _isSender ? "OUT" : "IN", 
            nanoseconds, 
            frame
        );
    }

    void OnTimestamp(PacketReader &reader) // TAG: 1573430 
    {
        auto nanoseconds = reader.ReadUint64();
        auto frame = reader.ReadUint64();

        //printf("[%s] SimulationMessages::Timestamp\n  nanoseconds = %llu\n  frame = %llu\n",
        //    _isSender ? "OUT" : "IN",
        //    nanoseconds,
        //    frame
        //);
    }

    void OnSetWorldGravityMagnitude(PacketReader &reader) // TAG: 15734A0
    {
        auto frame = reader.ReadUint64();
        auto magnitude = reader.ReadFloat();

        printf("[%s] SimulationMessages::SetWorldGravityMagnitude\n  frame = %llu\n  magnitude = %f\n",
            _isSender ? "OUT" : "IN",
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
            printf("[%s] SimulationManager::RigidBodyPropertyChanged\n  componentId = %llu\n",
                _isSender ? "OUT" : "IN",
                componentId
            );
        }
    }

    void OnRigidBodyDestroyed(PacketReader &reader)  // TAG: 1573660
    {
        auto componentId = reader.ReadUint64();

        if (_isVerbose)
        {
            printf("[%s] SimulationManager::OnRigidBodyDestroyed\n  componentId = %llu\n",
                _isSender ? "OUT" : "IN",
                componentId
            );
        }
    }
};
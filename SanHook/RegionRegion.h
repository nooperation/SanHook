#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "MessageHandler.h"
#include "PacketReader.hpp"
#include "Utils.hpp"

//void OnRegionRegionDynamicSubscribe(PacketReader &reader);
//void OnRegionRegionDynamicPlayback(PacketReader &reader);
//void OnRegionRegionMasterFrameSync(PacketReader &reader);
//void OnRegionRegionAgentControllerMapping(PacketReader &reader);

class RegionRegion : public MessageHandler
{
public:
    bool OnMessage(uint32_t messageId, PacketReader &reader, bool isSending)
    {
        this->_isSender = isSending; // todo: get rid of this garbage

        switch (messageId)
        {
            case RegionRegionMessages::DynamicSubscribe: // TAG: 1BB19B0
            {
               // this->OnDynamicSubscribe(reader);
                break;
            }
            case RegionRegionMessages::DynamicPlayback: // TAG: 1BB1B00
            {
               // this->OnDynamicPlayback(reader);
                break;
            }
            case RegionRegionMessages::MasterFrameSync: // TAG: 1BB1C50
            {
               // this->OnMasterFrameSync(reader);
                break;
            }
            case RegionRegionMessages::AgentControllerMapping: // TAG: 1BB1E10
            {
               // this->OnAgentControllerMapping(reader);
                break;
            }
            default:
            {
                return false;
            }
        }

        return true;
    }

    void OnDynamicSubscribe(PacketReader &reader)  // TAG: 1BB19B0
    {
        // No payload
    }

    void OnDynamicPlayback(PacketReader &reader) // TAG: 1BB1B00
    {
        // No payload
    }

    void OnMasterFrameSync(PacketReader &reader) // TAG: 1BB1C50
    {
        auto masterFrame = reader.ReadUint64();
        printf("MasterFrameSync: %llu", masterFrame);
    }

    void OnAgentControllerMapping(PacketReader &reader)  // TAG: 1BB1E10
    {
        auto agentControllerId = reader.ReadUint32();
        auto animationComponentId = reader.ReadUint64();
        auto clusterId = reader.ReadUint32();

        //printf("OnAgentControllerMapping:\n  agentControllerId = %u\n  animationComponentId = %llu\n  clusterId = %llu\n",
        //    agentControllerId,
        //    animationComponentId,
        //    clusterId
        //);
    }
};
